#include "CConnectSocket.h"

// SocketClientDlg.h: 헤더 파일
//

#pragma once
#include <afxdb.h>

struct Dol // 돌 구조체 (돌의 순서는 구조체 배열로 사용 예정)
{
	int array; // 돌의 배열
	int array_value; // 돌의 배열 위치	
	int enable; // 활성화 여부 0 = 비활성화, 1 활성화
	bool combine; // 돌이 합쳐지는 경우
}; 

// CSocketClientDlg 대화 상자
class CSocketClientDlg : public CDialogEx
{
// 생성입니다.
public:
	CSocketClientDlg(CWnd* pParent = nullptr);	// 표준 생성자입니다.

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SOCKETCLIENT_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원입니다.


// 구현입니다.
protected:
	HICON m_hIcon;

	// 생성된 메시지 맵 함수
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:	 
	CDatabase m_db;
	CRecordset* m_prs, * m_pRs;
	int Board_Location1[23][2] = {
		{0, 0},
		{600, 600},
		{600, 500},
		{600, 400},
		{600, 300},
		{600, 200},
		{600, 100}, // 6번째 1꼭짓점
		{500, 100},
		{400, 100},
		{300, 100},
		{200, 100},
		{100, 100}, // 11번째 2꼭짓점
		{100, 200},
		{100, 300},
		{100, 400},
		{100, 500},
		{100, 600}, // 16번째 3꼭짓점
		{200, 600},
		{300, 600},
		{400, 600},
		{500, 600},
		{600, 600}, // 21번째 4꼭짓점
		{0, 0}
	};
	int Board_Location2[7][2] = {
		{600, 100},
		{500, 200},
		{400, 300},
		{350, 350},// 중간값
		{300, 400},
		{200, 500},
		{100, 600},
	};
	int Board_Location3[7][2] = {
		{100, 100},
		{200, 200},
		{300, 300},
		{350, 350},// 중간값
		{400, 400},
		{500, 500},
		{600, 600},
	};		// 첫번째 배열 , 각 배열의 위치, 테두리는 22개 대각선은 7개씩 사용 가능하다.
			// 두번째 배열 : 위치의 x y 값 넣기
	
	CString name; // 이름
	CString Win; // 승리
	CString Lose; // 패배

	CConnectSocket m_Socket;
	CListBox m_List;
	int checknew;
	int yoot; //윷의 값
	bool Myturn; // 내 턴인지 확인하는 값 0 상대턴, 1 내턴
	bool Dol_Check; // 돌을 던졌는지 확인
	bool Yoot_Check; // 윷을 던졌는지 확인
	bool Start_Game; // 시작 변수

	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedButton1();
	//afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButton3();
	afx_msg void Start();
	CButton Send_Button;
	void Dol_Move(int dol_num);
	void Start_Hide(); // 게임 시작 후 버튼 나타내기
	void End_Show(); // 게임 종료 후 버튼 사라지기
	struct Dol dol[4]; // 0 1 아군 돌 값, 2 3 적군 돌 값

	CButton m_Start;
	CButton m_Dol1;
	CButton m_Dol2;
	CButton m_YootThrow;
	afx_msg void OnBnClickedFirstDol();
	afx_msg void OnBnClickedSecondDol();
};
