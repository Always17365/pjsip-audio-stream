#pragma once
#include <string>
#include <vector>
#include "CMediaSession.h"
#define RTP_BUFFER_SIZE 1500
#define	US_TIMEOUT		10000		// ��ʱû��rtp�������Ƴ�����λms
//#define US_TIMEOUT_DELETE_SESSION	// ��ʱ�Զ�ɾ���ջỰ
// TODO������

class CUdpServer
{
public:
	CUdpServer(char* pszIP);
	~CUdpServer();
	int CreateSession();

	void Loop();
	void StopLoop();
private:
	// �Ự����30s�Զ�ɾ��,�˽ӿڲ���¶�����߸�Ϊ�첽��ȫ��
	void DestroySession(int nSessionID);
	bool AddReadEvent(CMediaSession* pms);
	bool RemoveReadEvent(CMediaSession* pms);
	void TimerEvent();
	int	m_epfd;
	bool m_bRuning;
	std::string m_strIP;
	std::vector<CMediaSession*> m_vpMediaSessions;
	char m_buffer[RTP_BUFFER_SIZE];
	// ���ڻ���epoll_wait
	int m_pipefd[2];
};

