#pragma once
#include "afxwin.h"


// CTargetDepth 对话框

class CTargetDepth : public CDialog
{
	DECLARE_DYNAMIC(CTargetDepth)

public:
	CTargetDepth(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CTargetDepth();
	void SaveParameter();
// 对话框数据
	enum { IDD = IDD_TARGETDEPTH };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	CStatic staticTip;
	CStatic staticOne;
	CStatic staticTwo;
	CString strEditOne;
	CString strEditTwo;
	CEdit edit1;
	CEdit edit2;
	virtual BOOL OnInitDialog();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
};
