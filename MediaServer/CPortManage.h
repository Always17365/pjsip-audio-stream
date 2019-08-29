#pragma once
#include <netinet/in.h>
#include <vector>
/*
һ·rtp����Ҫ�����˿ںţ�rtp/rtcp
һ��Ϊ�����������˿ںţ�Сż��rtp��������rtcp
*/
typedef struct RTP_PORTS
{
	in_port_t uRTPPort;
	in_port_t uRTCPPort;
}RTP_PORTS;

typedef struct PM_DATA
{
	RTP_PORTS rtpPorts;		// RTP_PORTS�ڵ�һλ����֤����RTP_PORTS*����ǿתΪPM_DATA*
	bool bUse;
}PM_DATA;

class CPortManage
{
public:
	CPortManage();
	~CPortManage();
	// ֻ��ʹ��[start_port, end_port)��Χ�ڵĶ˿�
	bool InitPorts(in_port_t uStartPort, in_port_t uEndPort);
	RTP_PORTS* GetPort();
	void ReleasePort(RTP_PORTS* pPort);
private:
	std::vector<PM_DATA> m_vPorts;
};

extern CPortManage g_portManage;
