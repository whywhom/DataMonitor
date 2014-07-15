#pragma once


// CJobEditDlgCable 对话框

class CJobEditDlgCable : public CDialog
{
	DECLARE_DYNAMIC(CJobEditDlgCable)

public:
	CJobEditDlgCable(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CJobEditDlgCable();

// 对话框数据
	enum { IDD = IDD_JOBEDIT_CABLE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CString m_Wire1;
	CString m_Panel1;
	CString m_Wire2;
	CString m_Panel2;
	CString m_Wire3;
	CString m_Wire4;
	CString m_Wire5;
	CString m_Wire6;
	CString m_Wire7;
	CString m_Wire8;
	CString m_Wire9;
	CString m_Wire10;
	CString m_Panel3;
	CString m_Panel4;
	CString m_Panel5;
	CString m_Panel6;
	CString m_Panel7;
	CString m_Panel8;
	CString m_Panel9;
	CString m_Panel10;
	afx_msg void OnEnChangeEdit1();
	afx_msg void OnEnChangeEdit2();
	afx_msg void OnEnChangeEdit3();
	afx_msg void OnEnChangeEdit4();
	afx_msg void OnEnChangeEdit5();
	afx_msg void OnEnChangeEdit6();
	afx_msg void OnEnChangeEdit7();
	afx_msg void OnEnChangeEdit8();
	afx_msg void OnEnChangeEdit9();
	afx_msg void OnEnChangeEdit10();
	afx_msg void OnEnChangeEdit11();
	afx_msg void OnEnChangeEdit12();
	afx_msg void OnEnChangeEdit13();
	afx_msg void OnEnChangeEdit14();
	afx_msg void OnEnChangeEdit15();
	afx_msg void OnEnChangeEdit16();
	afx_msg void OnEnChangeEdit17();
	afx_msg void OnEnChangeEdit18();
	afx_msg void OnEnChangeEdit19();
	afx_msg void OnEnChangeEdit20();
};
