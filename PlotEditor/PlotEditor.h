
// PlotEditor.h : PlotEditor 应用程序的主头文件
//
#pragma once

#ifndef __AFXWIN_H__
	#error "在包含此文件之前包含“stdafx.h”以生成 PCH 文件"
#endif

#include "resource.h"       // 主符号
#include "CurveInfo.h"
// CPlotEditorApp:
// 有关此类的实现，请参阅 PlotEditor.cpp
//

class CPlotEditorApp : public CWinApp
{
public:
	CPlotEditorApp();
	CString GetResString(UINT uStringID);
public:
	HINSTANCE exe_hInstance;
	HANDLE hSem;//判断软件市立是否已存在
	CString strSemaphore;
    CString strSemaphoreExist;
    CString strSemaphoreCreatFail;
	CTypedPtrList < CPtrList, CCurveInfo * >curveList;
// 重写
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// 实现
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CPlotEditorApp theApp;
