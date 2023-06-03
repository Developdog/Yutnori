
// SocketClientDlg.cpp: 구현 파일
//

#include "stdafx.h"
#include "SocketClient.h"
#include "SocketClientDlg.h"
#include "CAlias.h"
#include "afxdialogex.h"
#include "CConnect.h"
#include <stdlib.h>
#include <time.h>
#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 응용 프로그램 정보에 사용되는 CAboutDlg 대화 상자입니다.

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

// 구현입니다.
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CSocketClientDlg 대화 상자



CSocketClientDlg::CSocketClientDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_SOCKETCLIENT_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	yoot = 5;
	Myturn = FALSE;
	Start_Game = FALSE;

	int Board_Location1[23][2];
	int Board_Location2[7][2];
	int Board_Location3[7][2];

	struct Dol dol[4];

	for (int i = 0; i < 4; i++)
	{
		dol[i].array = 0;
		dol[i].array_value = 0;
		dol[i].enable = 0;
		dol[i].combine = false;

	}

}

void CSocketClientDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_List);
	DDX_Control(pDX, IDC_BUTTON1, Send_Button);
	DDX_Control(pDX, IDC_BUTTON4, m_Start);
	DDX_Control(pDX, IDC_BUTTON5, m_Dol1);
	DDX_Control(pDX, IDC_BUTTON6, m_Dol2);
	DDX_Control(pDX, IDC_BUTTON3, m_YootThrow);
}

BEGIN_MESSAGE_MAP(CSocketClientDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, &CSocketClientDlg::OnBnClickedButton1)
	//ON_BN_CLICKED(IDC_BUTTON2, &CSocketClientDlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, &CSocketClientDlg::OnBnClickedButton3)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON4, &CSocketClientDlg::Start)
	ON_BN_CLICKED(IDC_BUTTON5, &CSocketClientDlg::OnBnClickedFirstDol)
	ON_BN_CLICKED(IDC_BUTTON6, &CSocketClientDlg::OnBnClickedSecondDol)
END_MESSAGE_MAP()


// CSocketClientDlg 메시지 처리기

BOOL CSocketClientDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 시스템 메뉴에 "정보..." 메뉴 항목을 추가합니다.

	// IDM_ABOUTBOX는 시스템 명령 범위에 있어야 합니다.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 이 대화 상자의 아이콘을 설정합니다.  응용 프로그램의 주 창이 대화 상자가 아닐 경우에는
	//  프레임워크가 이 작업을 자동으로 수행합니다.
	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.


	GotoDlgCtrl(GetDlgItem(IDC_INPUT));


	SetTimer(1, 100, NULL);
	CConnect dlg;
	if (dlg.DoModal() == IDOK)
	{
		m_Socket.Create();
		if (m_Socket.Connect(dlg.IPADDRESS, 21000) == FALSE) {
			AfxMessageBox(_T("ERROR : Failed to connect Server"));
			PostQuitMessage(0);
			return FALSE;
		}
	}
	CAlias dialog1 = new CAlias();
	dialog1.DoModal();
	// TODO: 여기에 추가 초기화 작업을 추가합니다.
	BOOL bopen = m_db.OpenEx(_T("DSN=mydb;SERVER=155.230.235.248;PORT=3405;UID=cduser01;PWD=user01@swdb;DATABASE=swdb;"));
	if (bopen)
		m_prs = new CRecordset(&m_db);
	return FALSE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

void CSocketClientDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 대화 상자에 최소화 단추를 추가할 경우 아이콘을 그리려면
//  아래 코드가 필요합니다.  문서/뷰 모델을 사용하는 MFC 응용 프로그램의 경우에는
//  프레임워크에서 이 작업을 자동으로 수행합니다.

void CSocketClientDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 그리기를 위한 디바이스 컨텍스트입니다.

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 클라이언트 사각형에서 아이콘을 가운데에 맞춥니다.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 아이콘을 그립니다.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{

		CFont editFont;
		editFont.CreatePointFont(500, TEXT("굴림"));
		GetDlgItem(IDC_EDIT1)->SetFont(&editFont);
		CClientDC dc(this);
		
		CBrush a = { RGB(242,203,97) }, b = { RGB(204,166,61) }, c = { RGB(0,0,0) }, d = { RGB(255,255,255) }, e = { RGB(0,0,255) }, f = { RGB(255,0,0) }, y = { RGB(255, 255, 0) };
		

		CRect rect;
		GetClientRect(&rect);
		CDC memDC;
		CBitmap* pOldBitmap, bitmap;

		memDC.CreateCompatibleDC(&dc);
		bitmap.CreateCompatibleBitmap(&dc, rect.Width(), rect.Height());

		pOldBitmap = memDC.SelectObject(&bitmap);
		memDC.PatBlt(0, 0, rect.Width(), rect.Height(), PATCOPY);

		memDC.SelectObject(&a);
		memDC.Rectangle(50, 50, 650, 650);
		memDC.SelectObject(&b);
		
		memDC.Ellipse(350 - 30, 350 - 30, 350 + 30, 350 + 30);
		for (int i = 0; i < 6; i++)
			for (int j = 0; j < 6; j++) {
				if ((i == 1 && j == 2) || (i == 1 && j == 3) || (i == 4 && j == 2) || (i == 4 && j == 3) ||
					(i == 2 && j == 1) || (i == 3 && j == 1) || (i == 2 && j == 4) || (i == 3 && j == 4)) {
					int k = 1;
				}
				else
				{
					CString make;

					memDC.Ellipse(70 + i * 100, 70 + j * 100, 130 + i * 100, 130 + j * 100);
				}
			}
		
		// 윷 판 비어있게 하기
		memDC.SelectObject(&d);
		memDC.Rectangle(700, 50, 700 + 30, 50 + 100);
		memDC.Rectangle(750, 50, 750 + 30, 50 + 100);
		memDC.Rectangle(800, 50, 800 + 30, 50 + 100);
		memDC.Rectangle(850, 50, 850 + 30, 50 + 100);


		// 내 이름 승리, 패배 횟수 확인하기

		CString alpha;
		CString beta;
		CString theta;

		alpha.Format(L"닉네임");
		beta.Format(L"승리");
		theta.Format(L"패배");

		memDC.SelectObject(&d);
		memDC.Rectangle(50, 20, 150, 40);
		memDC.SetBkMode(TRANSPARENT);
		memDC.DrawText(name, CRect(50, 20, 150, 40), DT_CENTER | DT_VCENTER | DT_SINGLELINE);
		memDC.DrawText(alpha, CRect(50, 0, 150, 20), DT_CENTER | DT_VCENTER | DT_SINGLELINE);

		memDC.SelectObject(&d);
		memDC.Rectangle(200, 20, 300, 40);
		memDC.SetBkMode(TRANSPARENT);
		memDC.DrawText(Win, CRect(200, 20, 300, 40), DT_CENTER | DT_VCENTER | DT_SINGLELINE);
		memDC.DrawText(beta, CRect(200, 0, 300, 20), DT_CENTER | DT_VCENTER | DT_SINGLELINE);

		memDC.SelectObject(&d);
		memDC.Rectangle(350, 20, 450, 40);
		memDC.SetBkMode(TRANSPARENT);
		memDC.DrawText(Lose, CRect(350, 20, 450, 40), DT_CENTER | DT_VCENTER | DT_SINGLELINE);
		memDC.DrawText(theta, CRect(350, 0, 450, 20), DT_CENTER | DT_VCENTER | DT_SINGLELINE);


		// 게임 시작 시 오른쪽 위 돌 표시 
		if (Start_Game == 1)
		{
			if (dol[0].enable == 0) 
				{
					memDC.SelectObject(&f);
				}
			else 
				{
					memDC.SelectObject(&d);
				}
			memDC.Ellipse(775 - 30, 230 - 30, 775 + 30, 230 + 30);

			if (dol[1].enable == 0)
				{
					memDC.SelectObject(&f);
				}
			else
				{
					memDC.SelectObject(&d);
				}
			memDC.Ellipse(945 - 30, 230 - 30, 945 + 30, 230 + 30);
		}

		// 도착지점에 왔는가 확인하는 함수


		CString Goal;
		Goal.Format(_T("GOAL!"));

		if(dol[0].array_value == 22)
		{
			memDC.SelectObject(&y);
			memDC.Ellipse(775 - 30, 230 - 30, 775 + 30, 230 + 30);
			memDC.SetBkMode(TRANSPARENT);
			memDC.DrawText(Goal, CRect(775 - 30, 230 - 30, 775 + 30, 230 + 30), DT_CENTER | DT_VCENTER | DT_SINGLELINE);
		}

		if (dol[1].array_value == 22)
		{
			memDC.SelectObject(&y);
			memDC.Ellipse(945 - 30, 230 - 30, 945 + 30, 230 + 30);
			memDC.SetBkMode(TRANSPARENT);
			memDC.DrawText(Goal, CRect(945 - 30, 230 - 30, 945 + 30, 230 + 30), DT_CENTER | DT_VCENTER | DT_SINGLELINE);
		}

		/*
		CString str, str2;

		str.Format(L"%d, %d, %d", dol[0].array, dol[0].array_value, dol[1].combine);
		str2.Format(L"%d, %d, %d", dol[1].array, dol[1].array_value, dol[1].combine);
		memDC.TextOut(775, 230, str);
		memDC.TextOut(945, 230, str2);
		*/
		// 돌 위치값 테스트하기 


		if (yoot == 0) {
			memDC.SelectObject(&c);
			memDC.Rectangle(850, 50, 850 + 30, 50 + 100);
			SetDlgItemText(IDC_EDIT1, L"도");
		}
		if (yoot == 1) {
			memDC.SelectObject(&c);
			memDC.Rectangle(800, 50, 800 + 30, 50 + 100);
			memDC.Rectangle(850, 50, 850 + 30, 50 + 100);
			SetDlgItemText(IDC_EDIT1, L"개");
		}
		if (yoot == 2) {
			memDC.SelectObject(&c);
			memDC.Rectangle(750, 50, 750 + 30, 50 + 100);
			memDC.Rectangle(800, 50, 800 + 30, 50 + 100);
			memDC.Rectangle(850, 50, 850 + 30, 50 + 100);
			SetDlgItemText(IDC_EDIT1, L"걸");
		}
		if (yoot == 3) {
			memDC.SelectObject(&c);
			memDC.Rectangle(700, 50, 700 + 30, 50 + 100);
			memDC.Rectangle(750, 50, 750 + 30, 50 + 100);
			memDC.Rectangle(800, 50, 800 + 30, 50 + 100);
			memDC.Rectangle(850, 50, 850 + 30, 50 + 100);
			SetDlgItemText(IDC_EDIT1, L"윷");
		}
		if (yoot == 4) {
			SetDlgItemText(IDC_EDIT1, L"모");
		}

		// 내 아이디 출력하기
		

		// 내 턴인지 빨간색 값으로 출력하기

		if (Start_Game == TRUE && Myturn == TRUE)
		{
			CString turn;
			turn.Format(_T("MyTurn!"));

			memDC.SelectObject(&f);
			memDC.Rectangle(700, 20, 720, 40);
			
			memDC.SelectObject(&d);
			memDC.Rectangle(730, 20, 900, 40);
			memDC.SetBkMode(TRANSPARENT);
			memDC.DrawText(turn, CRect(730, 20, 900, 40), DT_CENTER | DT_VCENTER | DT_SINGLELINE);
		}
		else if (Start_Game == TRUE && Myturn == FALSE)
		{
			CString turn;
			turn.Format(_T("Opponent Turn!"));

			memDC.SelectObject(&d);
			memDC.Rectangle(700, 20, 720, 40);

			memDC.SelectObject(&d);
			memDC.Rectangle(730, 20, 900, 40);
			memDC.SetBkMode(TRANSPARENT);
			memDC.DrawText(turn, CRect(730, 20, 900, 40), DT_CENTER | DT_VCENTER | DT_SINGLELINE);
		}


		// 현재 돌과 돌 위의 번호판 배치하기

		for (int i = 0; i < 4; i++) {

			if (dol[i].enable == 0 || (dol[i].array == 0 && dol[i].array_value == 0) || dol[i].array_value == 22); // 활성화가 되지 않거나, 첫번째 배열에서 처음이거나 끝 값일 경우
			else {
				if (i == 0 || i == 1)
				{
					memDC.SelectObject(&f);
				}
				else
				{
					memDC.SelectObject(&e);
				}
				
				if (dol[i].array == 0) { // 돌 배열 1일 경우
					memDC.Ellipse(Board_Location1[dol[i].array_value][0] - 30, Board_Location1[dol[i].array_value][1] - 30, 
						Board_Location1[dol[i].array_value][0] + 30, Board_Location1[dol[i].array_value][1] + 30);

					CString dol_name;
					if (dol[i].combine == TRUE) 
					{
						dol_name.Format(_T("합"));
					}
					else if (i < 2)
					{
						dol_name.Format(L"%d", i + 1);
					}
					else
					{
						dol_name.Format(L"%d", i - 1);
					}
					memDC.SetBkMode(TRANSPARENT);
					memDC.SetTextColor(RGB(255, 255, 255));
					memDC.DrawText(dol_name, CRect(Board_Location1[dol[i].array_value][0] - 30, Board_Location1[dol[i].array_value][1] - 30, Board_Location1[dol[i].array_value][0] + 30, Board_Location1[dol[i].array_value][1] + 30), DT_CENTER | DT_VCENTER | DT_SINGLELINE);
				}
				else if (dol[i].array == 1) { // 돌 배열 2일 경우
					memDC.Ellipse(Board_Location2[dol[i].array_value][0] - 30, Board_Location2[dol[i].array_value][1] - 30,
						Board_Location2[dol[i].array_value][0] + 30, Board_Location2[dol[i].array_value][1] + 30);

					CString dol_name;
					if (dol[i].combine == TRUE)
					{
						dol_name.Format(_T("합"));
					}
					else if (i < 2)
					{
						dol_name.Format(L"%d", i + 1);
					}
					else
					{
						dol_name.Format(L"%d", i - 1);
					}
					memDC.SetBkMode(TRANSPARENT);
					memDC.SetTextColor(RGB(255, 255, 255));
					memDC.DrawText(dol_name, CRect(Board_Location2[dol[i].array_value][0] - 30, Board_Location2[dol[i].array_value][1] - 30, Board_Location2[dol[i].array_value][0] + 30, Board_Location2[dol[i].array_value][1] + 30), DT_CENTER | DT_VCENTER | DT_SINGLELINE);
				}
				else if (dol[i].array == 2) { // 돌 배열 3일 경우
					memDC.Ellipse(Board_Location3[dol[i].array_value][0] - 30, Board_Location3[dol[i].array_value][1] - 30,
						Board_Location3[dol[i].array_value][0] + 30, Board_Location3[dol[i].array_value][1] + 30);

					CString dol_name;
					if (dol[i].combine == TRUE)
					{
						dol_name.Format(_T("%합"));
					}
					else if (i < 2)
					{
						dol_name.Format(L"%d", i + 1);
					}
					else
					{
						dol_name.Format(L"%d", i - 1);
					}
					memDC.SetBkMode(TRANSPARENT);
					memDC.SetTextColor(RGB(255, 255, 255));
					memDC.DrawText(dol_name, CRect(Board_Location3[dol[i].array_value][0] - 30, Board_Location3[dol[i].array_value][1] - 30, Board_Location3[dol[i].array_value][0] + 30, Board_Location3[dol[i].array_value][1] + 30), DT_CENTER | DT_VCENTER | DT_SINGLELINE);
				}
			}
		}

		dc.BitBlt(0, 0, rect.Width(), rect.Height(), &memDC, 0, 0, SRCCOPY);
		memDC.SelectObject(pOldBitmap);

		memDC.DeleteDC();
		bitmap.DeleteObject();

		CDialogEx::OnPaint();
	}
}

// 사용자가 최소화된 창을 끄는 동안에 커서가 표시되도록 시스템에서
//  이 함수를 호출합니다.
HCURSOR CSocketClientDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CSocketClientDlg::OnBnClickedButton1()
{
	checknew = 0;
	CString str;

	UpdateData(TRUE);
	GetDlgItemText(IDC_INPUT, str);
	m_Socket.Send((LPVOID)(LPCTSTR)str, str.GetLength() * 2);
	str.Format(_T(""));
	SetDlgItemText(IDC_INPUT, str);
	UpdateData(FALSE);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}

/*
void CSocketClientDlg::OnBnClickedButton2()
{
	CAlias dialog1 = new CAlias();
	dialog1.DoModal();
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}
*/

void CSocketClientDlg::OnTimer(UINT_PTR nIDEvent)
{
	if (checknew == 1) {
		int line = m_List.GetCount(); //추가한 갯수를 얻어옴
		m_List.SetTopIndex(line - 1); //얻어온 갯수로 스크롤 시킴
		checknew = 0;
	}
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	CDialogEx::OnTimer(nIDEvent);
}


BOOL CSocketClientDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	if (pMsg->message == WM_KEYDOWN)
	{
		if (pMsg->wParam == VK_RETURN || pMsg->wParam == VK_ESCAPE)
		{   // 위 VK_RETURN은 Enter, VK_ESCAPE는 ESC을 의미함. 필요시 하나만 사용.
			OnBnClickedButton1();
			return true;
		}
	}
	return CDialogEx::PreTranslateMessage(pMsg);
}


void CSocketClientDlg::OnBnClickedButton3()
{
	if (Myturn == FALSE) {
		AfxMessageBox(_T("상대방 차례입니다."));
		return;
	}
	if (Yoot_Check == TRUE) {
		AfxMessageBox(_T("이미 윷을 던졌습니다!"));
		return;
	}

	CString str;
	srand((unsigned int)time(NULL));
	

	int yoot_alpha = rand() % 16+1; // 상대적으로 모와 윷의 값이 적게 나오도록 설정하기 위해 0부터 4까지가 아닌 0부터 49까지의 랜덤 값을 가져온다.

	
	if(yoot_alpha <= 4 && yoot_alpha >= 1)
	{
		yoot = 0;
	}
	else if (yoot_alpha <= 10  && yoot_alpha >= 5)
	{
		yoot = 1;
	}
	else if (yoot_alpha <= 14 && yoot_alpha >= 11)
	{
		yoot = 2;
	}
	else if (yoot_alpha == 15)
	{
		yoot = 3;
	}
	else if (yoot_alpha == 16)
	{
		yoot = 4;
	}

	switch (yoot) {
	case 0:
		str.Format(_T("●"));
		break;
	case 1:
		str.Format(_T("▲"));
		break;
	case 2:
		str.Format(_T("■"));
		break;
	case 3:
		str.Format(_T("▼"));
		break;
	case 4:
		str.Format(_T("◆"));
		break;
	}

	Yoot_Check = 1; // 윷 던짐 확인

	UpdateData(TRUE);
	m_Socket.Send((LPVOID)(LPCTSTR)str, str.GetLength() * 2);
	UpdateData(FALSE);

	Invalidate(FALSE);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void CSocketClientDlg::Start()
{
	CString str;

	Myturn = TRUE; // 내 차례 시작

	str.Format(L"▶");

	UpdateData(TRUE);
	m_Socket.Send((LPVOID)(LPCTSTR)str, str.GetLength() * 2);
	UpdateData(FALSE);

	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void CSocketClientDlg::Start_Hide() // 시작 시 버튼 사라지고 나타나는거 설정하기
{
	UpdateData(TRUE);
	GetDlgItem(IDC_BUTTON3)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_BUTTON5)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_BUTTON6)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_BUTTON4)->ShowWindow(SW_HIDE);
	UpdateData(FALSE);
	// TODO: 여기에 구현 코드 추가.
}

void CSocketClientDlg::End_Show()
{
	UpdateData(TRUE);
	GetDlgItem(IDC_BUTTON3)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_BUTTON5)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_BUTTON6)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_BUTTON4)->ShowWindow(SW_SHOW);
	UpdateData(FALSE);
	// TODO: 여기에 구현 코드 추가.
}

void CSocketClientDlg::Dol_Move(int dol_num)
{
	for (int i = 0; i <= yoot; i++) 
	{
		if (dol[dol_num].array == 0 && dol[dol_num].array_value == 22)
		{
			break;
		}

		if(dol[dol_num].enable == 0)
		{
			dol[dol_num].enable = 1;
			dol[dol_num].array_value = 1;
		}
		// 돌이 활성화 되어있지 않은 경우, 돌을 활성화하기

		dol[dol_num].array_value = dol[dol_num].array_value + 1;

		if (dol[dol_num].array == 1 && dol[dol_num].array_value == 6)
		{
			dol[dol_num].array = 0;
			dol[dol_num].array_value = 16;
		}

		// 돌의 진행방향이 왼쪽 아래 대각선, 값이 3번째 꼭짓점일 경우, 테두리배열의 값으로 바꿔준다. 


		if (dol[dol_num].array == 2 && dol[dol_num].array_value == 6)
		{
			dol[dol_num].array = 0;
			dol[dol_num].array_value = 21;
		}

		// 돌의 진행방향이 오른쪽 아래 대각선, 값이 4번째 꼭짓점일 경우, 테두리배열의 값으로 바꿔준다. 




	}

	if (dol[dol_num].array == 0  && dol[dol_num].array_value == 6)
	{
		dol[dol_num].array = 1;
		dol[dol_num].array_value = 0;
	}

	// 돌의 진행방향이 테두리, 값이 1번째 꼭짓점일 경우, 왼쪽 아래 진행방향 값으로 바꿔준다. 

	if (dol[dol_num].array == 0 && dol[dol_num].array_value == 11)
	{
		dol[dol_num].array = 2;
		dol[dol_num].array_value = 0;
	}

	// 돌의 진행방향이 테두리, 값이 2번째 꼭짓점일 경우, 오른쪽 아래 진행방향 값으로 바꿔준다.

	if (dol[dol_num].array == 1 && dol[dol_num].array_value == 3)
	{
		dol[dol_num].array = 2;
		dol[dol_num].array_value = 3;
	}

	// 돌의 진행방향이 왼쪽 아래, 값이 중간 점일 경우, 오른쪽 아래 진행방향 값으로 바꿔준다.


	if(dol[dol_num].combine == true)
	{
		if (dol_num == 0)
		{
			dol[1].array = dol[0].array;
			dol[1].array_value = dol[0].array_value;
		}
		
		if (dol_num == 1)
		{
			dol[0].array = dol[1].array;
			dol[0].array_value = dol[1].array_value;
		}

		if (dol_num == 2)
		{
			dol[3].array = dol[2].array;
			dol[3].array_value = dol[2].array_value;
		}
		
		if (dol_num == 3)
		{
			dol[2].array = dol[3].array;
			dol[2].array_value = dol[3].array_value;
		}
	}

	//만약 겹치기 값이 활성화되어있을 경우


	if (dol[dol_num].array_value != 22)
	{
		if (dol_num < 2) // 2 이하일 경우 아군의 돌이 상대편의 돌을 잡는다.
		{
			if (dol[dol_num].array == dol[2].array && dol[dol_num].array_value == dol[2].array_value)
			{
				dol[2].array = 0;
				dol[2].array_value = 0;
				dol[2].enable = 0;
			
				if (dol[2].combine == true) {
					dol[2].combine = false;
					dol[3].combine = false;
					dol[3].array = 0;
					dol[3].array_value = 0;
					dol[3].enable = 0;
				}
			
			}
			if (dol[dol_num].array == dol[3].array && dol[dol_num].array_value == dol[3].array_value)
			{
				dol[3].array = 0;
				dol[3].array_value = 0;
				dol[3].enable = 0;

				if (dol[3].combine == true) {
					dol[3].combine = false;
					dol[2].combine = false;
					dol[2].array = 0;
					dol[2].array_value = 0;
					dol[2].enable = 0;
				}

			}
		}

		else // 2 이상일 경우, 상대편의 돌이 아군의 돌을 잡는다.
		{
			if (dol[dol_num].array == dol[0].array && dol[dol_num].array_value == dol[0].array_value)
			{
				dol[0].array = 0;
				dol[0].array_value = 0;
				dol[0].enable = 0;

				if (dol[0].combine == true) {
					dol[0].combine = false;
					dol[1].combine = false;
					dol[1].array = 0;
					dol[1].array_value = 0;
					dol[1].enable = 0;
				}

			}
			if (dol[dol_num].array == dol[1].array && dol[dol_num].array_value == dol[1].array_value)
			{
				dol[1].array = 0;
				dol[1].array_value = 0;
				dol[1].enable = 0;

				if (dol[1].combine == true) {
					dol[1].combine = false;
					dol[0].combine = false;
					dol[0].array = 0;
					dol[0].array_value = 0;
					dol[0].enable = 0;
				}
			
			}
		}
	}

	// 상대방의 돌 잡아먹기
	// 일단 value의 값이 22(종료지점)이 아니고, 
	// i의 값이 0 1 일 경우 상대방의 돌을 먹는다. 2 3 일 경우 내 돌이 먹힌다.
	// 각각의 돌의 값을 비교한다.
	// 만약 해당 값이 같을 경우 array와 value의 값을 둘다 0으로 만든다.


	if (!(dol[dol_num].array_value == 0 && dol[dol_num].array == 0) && dol[dol_num].array_value != 22)
	{
		if (dol_num == 0 || dol_num == 1) // 만약 아군의 돌 위치가 같을 경우에 돌 합치기 값을 활성화시킨다.
		{
			if (dol[0].array == dol[1].array && dol[0].array_value == dol[1].array_value) 
			{
				dol[0].combine = true;
				dol[1].combine = true;
			}
		}
		else // 만약 아군의 돌 위치가 같을 경우에 돌 합치기 값을 활성화시킨다.
		{
			if (dol[2].array == dol[3].array && dol[2].array_value == dol[3].array_value)
			{
				dol[2].combine = true;
				dol[3].combine = true;
			}
		}
	}


	// 승리 조건
	
	if (dol[0].array_value == 22 && dol[1].array_value == 22)
	{
		// 승리할 시 따로 행동 만들어주기
		Start_Game = 0;

		CString str;

		str.Format(L"♬");

		UpdateData(TRUE);
		m_Socket.Send((LPVOID)(LPCTSTR)str, str.GetLength() * 2);
		UpdateData(FALSE);

		Invalidate(FALSE);

		AfxMessageBox(L"승리하셨습니다!");
		
		CString str3;
		str3.Format(L"UPDATE yut_table set win = win+1 where name='%s'", name);

		m_db.BeginTrans();
		try {
			m_db.ExecuteSQL(str3);
		}
		catch (CException* e) {
			e->ReportError();
		}
		m_db.CommitTrans();

		int win = _ttoi(Win);
		win = win + 1;
		Win.Format(_T("%d"), win);

		End_Show();

		return;
	}

	// 패배 조건

	if (dol[2].array_value == 22 && dol[3].array_value == 22)
	{
		Start_Game = 0;
	
		Invalidate(FALSE);

		AfxMessageBox(L"패배하셨습니다!");

		CString str4;
		str4.Format(L"UPDATE yut_table set lose = lose+1 where name='%s'", name);
		m_db.BeginTrans();
		try {
			m_db.ExecuteSQL(str4);
		}
		catch (CException* e) {
			e->ReportError();
		}
		m_db.CommitTrans();

		int lose = _ttoi(Lose);
		lose = lose + 1;
		Lose.Format(_T("%d"), lose);

		End_Show();



		return;
	}

	// 모, 윷이 나올 경우 한턴 더 주기

	if (yoot == 3 || yoot == 4)
	{
		Dol_Check = 0;
		Yoot_Check = 0;
	}

	// 턴 변경

	else if (Myturn == FALSE)
	{
		CString str;
		str.Format(L"◀");

		UpdateData(TRUE);
		m_Socket.Send((LPVOID)(LPCTSTR)str, str.GetLength() * 2);
		UpdateData(FALSE);

	}


	Invalidate(false);

	// 구현해야할 것
	// 맨 처음 int 값을 받아와서 어떤 말의 value인지 확인하기
	// 만약 해당 돌이 비활성화됬다면, 해결해주기
	// 해당 말의 value 확인하기, 이후 해당 돌이 보드의 마지막 부분을 나갔는지 확인하기(대각선에서 마지막 부분에 들어오면, 테두리 마지막 값으로 바꾸기)
	// 돌이 상대 돌을 잡아먹었는지 확인하기
	// 마지막으로 승리조건 확인하기
	// TODO: 여기에 구현 코드 추가.
}


void CSocketClientDlg::OnBnClickedFirstDol() // 첫번째 돌
{
	if (Myturn == FALSE) {
		AfxMessageBox(_T("상대방 차례입니다!"));
		return;
	}
	if (Yoot_Check == FALSE) {
		AfxMessageBox(_T("윷을 던진 후 사용 가능합니다."));
		return;
	}
	if (dol[0].array_value >= 22)
	{
		AfxMessageBox(_T("이미 결승점에 도달 했습니다!"));
		return;
	}

	CString str;
	str.Format(L"♥");



	UpdateData(TRUE);
	m_Socket.Send((LPVOID)(LPCTSTR)str, str.GetLength() * 2);
	UpdateData(FALSE);

	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.


}


void CSocketClientDlg::OnBnClickedSecondDol() // 두번째 돌
{
	if (Myturn == FALSE) {
		AfxMessageBox(_T("상대방 차례입니다!"));
		return;
	}
	if (Yoot_Check == FALSE) {
		AfxMessageBox(_T("윷을 던진 후 사용 가능합니다."));
		return;
	}

	if (dol[1].array_value >= 22)
	{
		AfxMessageBox(_T("이미 결승점에 도달 했습니다!"));
		return;
	}

	CString str;
	str.Format(L"♡");

	UpdateData(TRUE);
	m_Socket.Send((LPVOID)(LPCTSTR)str, str.GetLength() * 2);
	UpdateData(FALSE);

	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


