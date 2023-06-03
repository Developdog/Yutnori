// CConnect.cpp: 구현 파일
//

#include "stdafx.h"
#include "SocketClient.h"
#include "CConnect.h"
#include "afxdialogex.h"


// CConnect 대화 상자

IMPLEMENT_DYNAMIC(CConnect, CDialogEx)

CConnect::CConnect(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG2, pParent)
	, IPADDRESS(_T(""))
{

}

CConnect::~CConnect()
{
}

void CConnect::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_IPADDRESS1, IPADDRESS);
}


BEGIN_MESSAGE_MAP(CConnect, CDialogEx)
	ON_BN_CLICKED(IDOK, &CConnect::OnBnClickedOk)
END_MESSAGE_MAP()


// CConnect 메시지 처리기


void CConnect::OnBnClickedOk()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	GetDlgItemText(IDC_IPADDRESS1, IPADDRESS);
	CDialogEx::OnOK();
}


BOOL CConnect::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	SetDlgItemText(IDC_IPADDRESS1, _T("127.0.0.1"));


	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}