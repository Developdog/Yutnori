#pragma once
#include <afxsock.h>
class CConnectSocket :
	public CSocket
{
public:
	CConnectSocket();
	~CConnectSocket();

	virtual void OnClose(int nErrorCode);
	virtual void OnReceive(int nErrorCode);
	void CConnectSocket::Wait(DWORD dwMillisecond);
};
