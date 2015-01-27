#pragma once
#include "jobeditdlg.h"
#include "dirtreectrl.h"
#include "afxwin.h"

// CJobDlg �Ի���

class CJobDlg : public CDialog
{
	DECLARE_DYNAMIC(CJobDlg)

public:
	CJobDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CJobDlg();

// �Ի�������
	enum { IDD = IDD_JOB };
public:
	int receiveFlag;
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

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
	afx_msg void OnJobLoad();
	void LoadFile(CString strPath);
	afx_msg void OnBnClickedButton1();
};
