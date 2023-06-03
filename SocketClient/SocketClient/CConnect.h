#pragma once


// CConnect 대화 상자

class CConnect : public CDialogEx
{
	DECLARE_DYNAMIC(CConnect)

public:
	CConnect(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CConnect();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG2 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	CString IPADDRESS;

	afx_msg void OnBnClickedOk();
	virtual BOOL OnInitDialog();
};
