#pragma once
#include "afxwin.h"



// CPanelView 窗体视图

class CPanelView : public CFormView
{
	DECLARE_DYNCREATE(CPanelView)

protected:
	CPanelView();           // 动态创建所使用的受保护的构造函数
	virtual ~CPanelView();
public:
CStatic cs01;
	CString value01;
	CStatic cs02;
	CString value02;
	CStatic cs03;
	CString value03;
	CStatic cs04;
	CString value04;
	CEdit Edit01;
	CString strEdit01;
	CEdit Edit02;
	CString strEdit02;
	CEdit Edit03;
	CString strEdit03;
	CEdit Edit04;
	CString strEdit04;
	CButton btSetup;
	CListCtrl listView;
public:
	enum { IDD = IDD_PANEL_VIEW };
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	virtual void OnInitialUpdate();
};


