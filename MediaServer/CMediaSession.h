#pragma once
#include "CPortManage.h"
#include <vector>
#define MS_USE_RTCP			// �Ƿ�ʹ��rtcp(TODO)
#define MS_AUTO_JOIN		// �Զ�����Ự
typedef struct MS_CLIENT_DATA
{
	sockaddr_in addr;
	bool bVaild;
}MS_CLIENT_DATA;
// TODO������
class CMediaSession
{
public:
	CMediaSession();
	~CMediaSession();

	bool InitSession(const char* pszIP);
	void CloseSession();
	int GetRTPfd();
	void Add(int fd, sockaddr_in addr);
	void Remove(int fd, sockaddr_in addr);
	void OnRecvRTP(int fd, sockaddr_in& addr, char* cRtpData, size_t szDataSize);
	// ��ʱ�Ƴ���Ч�ͻ��ˣ����̰߳�ȫ
	void TimerRemoveInvaildClient();
	int GetClientNum();
private:
	RTP_PORTS* m_pPort;
	int m_fdRTP;
#ifdef MS_USE_RTCP
	int m_fdRTCP;
#endif
	// �洢��ǰý��Ự�����еĿͻ�����
	std::vector<MS_CLIENT_DATA> m_vClientData;
};

