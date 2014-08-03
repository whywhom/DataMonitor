#pragma once
#include "jobeditdlg.h"
#include "dirtreectrl.h"

// CJobDlg 对话框

class CJobDlg : public CDialog
{
	DECLARE_DYNAMIC(CJobDlg)

public:
	CJobDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CJobDlg();

// 对话框数据
	enum { IDD = IDD_JOB };
public:
	int receiveFlag;
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnJobNew();
	afx_msg void OnJobOpen();
	virtual BOOL OnInitDialog();
	CDirTreeCtrl m_treeCtrl;
	afx_msg void OnJobDel();
	afx_msg void OnJobCopy();
	CString m_Path;
	CString m_Title;
	CString m_TreeTitle;
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
};
