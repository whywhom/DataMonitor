#pragma once
#pragma warning(disable : 4995)
#include "afxcmn.h"
#include "jobeditdlgtool.h"
#include "jobeditdlgcurve.h"
#include "jobeditdlgorigin.h"
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
	CDaoDatabase m_DataBase;
	bool m_Open;
	CTreeCtrl m_jeTree;
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangedJobeditTree(NMHDR *pNMHDR, LRESULT *pResult);
public:	
   	CJobEditDlgTool m_Tool;//仪器信息对话框对应的类
	CJobEditDlgCurve m_Curve;//曲线信息对话框对应的类
	CJobEditDlgOrigin m_Origin;//原始信息对话框对应的类
	afx_msg void OnBnClickedOk();
private:	
	void ToolAdd();
	void ToolInit(CString Label);
	void ToolInit();
	void ToolDel(CString Label);
	void ToolUpdate(CString Label);
	void CurveAdd();
	void CurveInit(CString Label);
	void CurveInit();
	void CurveDel(CString Label);
	void CurveUpdate(CString Label);
	void OriginAdd();
	void OriginInit(CString Label);
	void OriginInit();
	void OriginDel(CString Label);
	void OriginUpdate(CString Label);
	void DisplayTree();
	void JobInit();//初始化作业数据库

public:
	afx_msg void OnBnClickedAdd();
	int m_editTable;
	afx_msg void OnBnClickedDelete();
	afx_msg void OnBnClickedUpdate();
};
