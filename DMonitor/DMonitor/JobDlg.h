#pragma once
#include "afxcmn.h"
#include "afxwin.h"


// CJobDlg 对话框

class CJobDlg : public CDialog
{
	DECLARE_DYNAMIC(CJobDlg)
public:
	CString m_Path;
	CString m_Title;

public:
	CJobDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CJobDlg();

// 对话框数据
	enum { IDD = IDD_JOB };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnBnClickedNewJob();
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedButtonNewjob();
	afx_msg void OnBnClickedButtonClose();
	CButton btnNewJob;
	CButton btnClose;
	afx_msg void OnSize(UINT nType, int cx, int cy);
};
