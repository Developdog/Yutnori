#include "stdafx.h"
#include "CConnectSocket.h"
#include "SocketClientDlg.h"


CConnectSocket::CConnectSocket()
{
}


CConnectSocket::~CConnectSocket()
{
}

void CConnectSocket::OnClose(int nErrorCode)
{
	ShutDown();
	Close();

	CSocket::OnClose(nErrorCode);

	AfxMessageBox(_T("ERROR:Disconnected from server!"));
	::PostQuitMessage(0);
}

void CConnectSocket::OnReceive(int nErrorCode)
{
	TCHAR szBuffer[1024];
	::ZeroMemory(szBuffer, sizeof(szBuffer));

	if (Receive(szBuffer, sizeof(szBuffer)) > 0) {
		CSocketClientDlg* pMain = (CSocketClientDlg*)AfxGetMainWnd();
		int cnt = pMain->m_List.GetCount();
		CString originText = szBuffer;

		Wait(200);

		if (originText == "●") {
				pMain->yoot = 0;
		}
		else if (originText == "▲") {
				pMain->yoot = 1;
		}
		else if (originText == "■") {
				pMain->yoot = 2;
		}
		else if (originText == "▼") {
				pMain->yoot = 3;
		}
		else if (originText == "◆") {
				pMain->yoot = 4;
		}
		else if (originText == "▶") {
			pMain->Start_Game = 1; // 게임 시작을 알립니다.
			pMain->Start_Hide();

			for (int i = 0; i < 4; i++)
			{
				pMain->dol[i].array = 0;
				pMain->dol[i].array_value = 0;
				pMain->dol[i].enable = FALSE;
				pMain->dol[i].combine = FALSE;
			}
			pMain->Dol_Check = 0;
			pMain->Yoot_Check = 0;
			pMain->Invalidate(FALSE);
		}
		else if (originText == "◀") {
			pMain->Dol_Check = 0;
			pMain->Yoot_Check = 0;
			pMain->Myturn = !(pMain->Myturn); // 턴 변경을 알립니다.
			pMain->Invalidate(FALSE);
		}
		else if (originText == "♥") { // 첫번째 말을 움직입니다.
			if (pMain->Myturn == 1) {
				pMain->Dol_Move(0);
			}
			else if(pMain->Myturn == 0){
				pMain->Dol_Move(2);
			}
		}
		else if (originText == "♡") { // 두번째 말을 움직입니다.
			if (pMain->Myturn == 1) {
				pMain->Dol_Move(1);
			}
			else if(pMain->Myturn == 0){
				pMain->Dol_Move(3);
			}
		}

		else {
			CString RecName = pMain->name;
			RecName = L"[" + RecName + L"]:";


			if (originText.Find(RecName) != -1) {
				originText = L"★ " + originText;
			}
			else {
				originText = L"   " + originText;
			}
			pMain->m_List.InsertString(cnt, originText);
			pMain->checknew = 1;
		}
	}
	CSocket::OnReceive(nErrorCode);
}


/////////////////////////////////////////////////////////////////
// 수행 대기 delay 함수(instead sleep function)
/////////////////////////////////////////////////////////////////
void CConnectSocket::Wait(DWORD dwMillisecond)
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