#include "CUdpServer.h"
#include <sys/epoll.h>
#include <unistd.h>
#include <assert.h>

CUdpServer::CUdpServer(char* pszIP)
	: m_bRuning(false)
	, m_strIP(pszIP)
{
	m_epfd = epoll_create(1024);
	// �����ܵ�
	assert(0 == pipe(m_pipefd));
	// ע����¼�
	struct epoll_event ev;
	ev.events = EPOLLIN;
	ev.data.ptr = NULL;
	epoll_ctl(m_epfd, EPOLL_CTL_ADD, m_pipefd[0], &ev);
}


CUdpServer::~CUdpServer()
{
	std::vector<CMediaSession*>::iterator it;
	for (it = m_vpMediaSessions.begin(); it != m_vpMediaSessions.end();)
	{
		RemoveReadEvent(*it);
		(*it)->CloseSession();
		delete (*it);
		it = m_vpMediaSessions.erase(it);
	}
	close(m_pipefd[0]);
	close(m_pipefd[1]);
	if (-1 != m_epfd)
	{
		close(m_epfd);
		m_epfd = -1;
	}
}

int CUdpServer::CreateSession()
{
	CMediaSession* pms = new CMediaSession;
	if (!pms->InitSession(m_strIP.c_str()))
	{
		delete pms;
		return -1;
	}
	m_vpMediaSessions.push_back(pms);
	AddReadEvent(pms);
	return m_vpMediaSessions.size() - 1;
}

void CUdpServer::DestroySession(int nSessionID)
{
	if (nSessionID < m_vpMediaSessions.size())
	{
		std::vector<CMediaSession*>::iterator it = m_vpMediaSessions.begin() + nSessionID;
		RemoveReadEvent(*it);
		(*it)->CloseSession();
		delete (*it);
		m_vpMediaSessions.erase(it);
	}
}

bool CUdpServer::AddReadEvent(CMediaSession* pms)
{
	if (-1 == m_epfd)
	{
		return false;
	}
	int fd = pms->GetRTPfd();
	bool bRet = false;
	struct epoll_event ev;
	ev.events = EPOLLIN;
	ev.data.ptr = pms;
	if (epoll_ctl(m_epfd, EPOLL_CTL_ADD, fd, &ev) < 0)
	{
		bRet = true;
	}
	return bRet;
}

bool CUdpServer::RemoveReadEvent(CMediaSession* pms)
{
	if (-1 == m_epfd)
	{
		return false;
	}
	int fd = pms->GetRTPfd();
	epoll_ctl(m_epfd, EPOLL_CTL_DEL, fd, NULL);
	return true;
}

void CUdpServer::TimerEvent()
{
	std::vector<CMediaSession*>::iterator it;
	for (it = m_vpMediaSessions.begin(); it != m_vpMediaSessions.end();)
	{
		// �Ƴ���ʱ�ͻ���
		(*it)->TimerRemoveInvaildClient();
		if (0 == (*it)->GetClientNum())
		{
#ifdef US_TIMEOUT_DELETE_SESSION
			// �Զ�ɾ���ջỰ
			RemoveReadEvent(*it);
			(*it)->CloseSession();
			delete (*it);
			it = m_vpMediaSessions.erase(it);
#endif // US_TIMEOUT_DELETE_SESSION
		}
		else
		{
			++it;
		}
	}
}

void CUdpServer::Loop()
{
	struct epoll_event events[1024];
	int nfds = 0;

	if (m_bRuning)
		return;
	m_bRuning = true;
	int fdRTP;
	sockaddr_in addr;
	socklen_t slLen;
	size_t szRecv;

	int timeout = US_TIMEOUT;
	time_t start;
	time_t end;
	while (m_bRuning)
	{
		start = time(NULL);
		nfds = epoll_wait(m_epfd, events, 1024, timeout);
		if ((nfds < 0) && (errno != EINTR))
		{
			break;
		}
		// ��ʱʱ�䵽������ʱ�������ö�ʱ��
		if (0 == nfds)
		{
			// ����ʱ����
			TimerEvent();
			// ���ö�ʱ��
			timeout = US_TIMEOUT;
			continue;
		}
		// ��ʱʱ��δ�������ʣ��˯��ʱ��
		end = time(NULL);
		timeout -= (end - start) * 1000;
		// ʣ��˯��ʱ��<=0˵������ʱ���пɶ��¼�����
		if (timeout <= 0)
		{
			// ����ʱ����
			TimerEvent();
			// ���ö�ʱ��
			timeout = US_TIMEOUT;
		}
		for (int i = 0; i < nfds; i++)
		{
			CMediaSession* pms = (CMediaSession*)events[i].data.ptr;
			if (NULL == pms)
			{
				// ����pipeû��pms
				continue;
			}
			if (events[i].events & EPOLLIN)
			{
				// TODO ����fdrtp��fdrtcp
				fdRTP = pms->GetRTPfd();
				slLen = sizeof(addr);
				szRecv = recvfrom(fdRTP, m_buffer, RTP_BUFFER_SIZE, 0,
					(sockaddr*)&addr, &slLen);
				if (szRecv > 0)
				{
					pms->OnRecvRTP(fdRTP, addr, m_buffer, szRecv);
				}
			}
		}
	}
}

void CUdpServer::StopLoop()
{
	m_bRuning = false;
	// pipe�ö�дread write������send recv
	write(m_pipefd[1], "1", 1);
}
