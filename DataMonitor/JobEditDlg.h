#pragma once
#include "afxcmn.h"
#include "jobeditdlgcable.h"
#include "jobeditdlgtool.h"
#include "afxwin.h"


// CJobEditDlg 对话框

class CJobEditDlg : public CDialog
{
	DECLARE_DYNAMIC(CJobEditDlg)

public:
	CJobEditDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CJobEditDlg();

// 对话框数据
	enum { IDD = IDD_JOBEDIT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CString m_jobName;
	bool m_Open;
	CTreeCtrl m_jeTree;
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangedJobeditTree(NMHDR *pNMHDR, LRESULT *pResult);
public:	
    CJobEditDlgCable m_Cable;//缆芯提示对话框所对应的类
	CJobEditDlgTool m_Tool;//仪器信息对话框对应的类
	afx_msg void OnBnClickedOk();
private:
	void CableInit();
	void CableAdd();
	void CableUpdate();
	void ToolAdd();
	void DisplayTree();
	void JobInit();//初始化作业数据库

public:
	afx_msg void OnBnClickedAdd();
	int m_editTable;
	afx_msg void OnBnClickedDelete();
};
