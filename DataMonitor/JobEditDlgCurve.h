#pragma once
#include "afxwin.h"


// CJobEditDlgCurve 对话框

class CJobEditDlgCurve : public CDialog
{
	DECLARE_DYNAMIC(CJobEditDlgCurve)

public:
	CJobEditDlgCurve(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CJobEditDlgCurve();

// 对话框数据
	enum { IDD = IDD_JOBEDIT_CURVE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	CComboBox m_unitbox;
	CComboBox m_filterbox;
	CString m_Label;
	int m_unit;
	int m_filter;
	afx_msg void OnEnChangeLabel();
};
