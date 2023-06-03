// ClientSocket.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "SocketServer.h"
#include "CClientSocket.h"
#include "CListenSocket.h"
#include "SocketServerDlg.h"


// CClientSocket
int index = 0;
CString alias[100][2];

CClientSocket::CClientSocket()
{
}

CClientSocket::~CClientSocket()
{
}


// CClientSocket 멤버 함수
void CClientSocket::SetListenSocket(CAsyncSocket* pSocket)
{
	m_pListenSocket = pSocket;
}
// CClientSocket 멤버 함수


void CClientSocket::OnClose(int nErrorCode)
{
	CSocket::OnClose(nErrorCode);

	CListenSocket* pServerSocket = (CListenSocket*)m_pListenSocket;
	pServerSocket->CloseClientSocket(this);
}

void CClientSocket::OnReceive(int nErrorCode)
{
	CSocketServerDlg* pMain = (CSocketServerDlg*)AfxGetMainWnd();

	int i, check = 0;
	CString strTmp = _T(""), strIPAddress = _T("");
	UINT uPortNumber = 0;
	TCHAR strBuffer[1024];
	::ZeroMemory(strBuffer, sizeof(strBuffer));


		GetPeerName(strIPAddress, uPortNumber);
		if (Receive(strBuffer, sizeof(strBuffer)) > 0) {

			CString portStr;
			portStr.Format(L"%d", uPortNumber);

			for (i = index - 1; i >= 0; i--) {
				if (portStr == alias[i][0]) {

					strTmp.Format(_T("%s"), strBuffer);

					Wait(200);

					CListenSocket* pServerSocket = (CListenSocket*)m_pListenSocket;
					TCHAR* tChr = (TCHAR*)(LPCTSTR)strTmp;

					if(strTmp == "●"){
						pServerSocket->SendAllMessage(tChr);
						strTmp.Format(L"%s님이 도를 던졌습니다!", alias[i][1]);
					}
					else if (strTmp == "▲"){
						pServerSocket->SendAllMessage(tChr);
						strTmp.Format(L"%s님이 개를 던졌습니다!", alias[i][1]);
					}
					else if (strTmp == "■"){
						pServerSocket->SendAllMessage(tChr);
						strTmp.Format(L"%s님이 걸을 던졌습니다!", alias[i][1]);
					}
					else if (strTmp == "▼"){
						pServerSocket->SendAllMessage(tChr);
						strTmp.Format(L"%s님이 윷을 던졌습니다! 한번 더 던지세요!", alias[i][1]);
					}
					else if (strTmp == "◆"){
						pServerSocket->SendAllMessage(tChr);
						strTmp.Format(L"%s님이 모를 던졌습니다! 한번 더 던지세요!", alias[i][1]);
					}
					else if (strTmp == "▶") {
						pServerSocket->SendAllMessage(tChr);
						strTmp.Format(L"게임이 시작됩니다!, %s님 차례입니다.", alias[i][1]);
					}
					else if (strTmp == "◀") {
						pServerSocket->SendAllMessage(tChr);
						strTmp.Format(L"%s님 차례입니다.", alias[i][1]);

						//만약 본인의 차례가 아니면 윷을 던질 수 없다.
						//게임 시작 변수값을 ON한다.
						//시작 버튼을 누른 사람의 차례값은 1로, 아닌 사람의 차례값은 0으로 바뀐다.
						// 해당 차례값은 윷을 던지고, 돌을 옮겨야만 값이 바뀐다.
					}
					else if (strTmp == "♥") {
						pServerSocket->SendAllMessage(tChr);
						strTmp.Format(L"%s님이 첫번째 말을 움직였습니다.", alias[i][1]);
					}
					else if (strTmp == "♡") {
						pServerSocket->SendAllMessage(tChr);
						strTmp.Format(L"%s님이 두번째 말을 움직였습니다.", alias[i][1]);
					}
					else if (strTmp == "♬") {
						Wait(200);
						strTmp.Format(L"%s님 승리! 게임을 종료합니다.", alias[i][1]);
						Wait(200);
					}
					else {
						strTmp.Format(_T("[%s]: %s"), alias[i][1], strBuffer);
					}
					break;
				}
			}

			Wait(200);

			if (i == -1) {
				strTmp.Format(_T("[%s:%d]: %s"), strIPAddress, uPortNumber, strBuffer);
			}

			if (strTmp.Find(L"alias:") != -1) {
				alias[index][0] = portStr;
				CString temp = strBuffer;
				temp.Delete(0, 6);
				alias[index][1] = temp;
				index++;
				check = 1;
			}

			if (check == 0) {
				int cnt = pMain->m_List.GetCount();
				pMain->m_List.InsertString(cnt, strTmp);

				CListenSocket* pServerSocket = (CListenSocket*)m_pListenSocket;

				TCHAR* tChr = (TCHAR*)(LPCTSTR)strTmp;
				pServerSocket->SendAllMessage(tChr);
			}
			else {
				check = 0;
			}
		}

		CSocket::OnReceive(nErrorCode);

	

}


/////////////////////////////////////////////////////////////////
// 수행 대기 delay 함수(instead sleep function)
/////////////////////////////////////////////////////////////////
void CClientSocket::Wait(DWORD dwMillisecond)
{
	MSG msg;
	DWORD dwStart;
	dwStart = GetTickCount();

	while (GetTickCount() - dwStart < dwMillisecond)
	{
		while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}
}