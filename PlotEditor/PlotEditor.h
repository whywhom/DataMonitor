
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
	void GetModulePath();
public:
	HINSTANCE exe_hInstance;
	HANDLE hSem;//判断软件市立是否已存在
	CString strSemaphore;
    CString strSemaphoreExist;
    CString strSemaphoreCreatFail;
	CTypedPtrList < CPtrList, CCurveInfo * >curveList;
	bool bCurveFileExist;

	CString szExePath;//可执行文件所在位置绝对路径
	CString ModuleFilePath;
	CString TempPath;//临时文件夹绝对路径
	CString LogPath;//log日志所在位置绝对路径
	CString IniFilePath;//ini文件夹绝对路径
	CString strIniFilePath;//ini文件所在位置绝对路径
	CString DataPath;//临时文件夹绝对路径
	CString JobPath;//作业文件夹的绝对路径
	CString ToolPath;//仪器文件夹绝对路径
	CString strUnitsFile;//记录度量单位的配置文件
	CString strCurveFile;//记录绘图模板的配置文件
	CString strSettingFile;//记录用户设置的常用数值配置文件

// 重写
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	char *FromUNICODEToANSI(CString str);
	short FromUnicodeToUTF8 (LPSTR utf8Str, short utf8StrLen, WORD * unStr, unsigned short unMaxLen);
	short FromUTF8ToUnicode (unsigned char *utf8Str, short utf8StrLen, WORD * unStr, unsigned short unMaxLen);
	short Utf8TextBytes (const unsigned char *mstr);
// 实现
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CPlotEditorApp theApp;
