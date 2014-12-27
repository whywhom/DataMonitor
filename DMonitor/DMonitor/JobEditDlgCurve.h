#pragma once
#include "afxwin.h"
#include "ColorStatic.h"

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
	COLORREF curveSelectColor;
	CString m_Label;
	int m_unit;
	int m_filter;
	afx_msg void OnEnChangeLabel();
	CComboBox m_trackbox;
	CComboBox m_linetype;
	CColorStatic m_scolor;
	CButton m_btColor;
	CEdit nTestEditMin;
	CEdit nTestEditMax;
	CEdit m_edittitle;
	afx_msg void OnBnClickedButtonColor();
	CString m_title;
	CString m_minLimit;
	CString m_maxLimit;
	int m_line;
	int m_track;
};
