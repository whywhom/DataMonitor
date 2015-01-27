
// PlotEditor.h : PlotEditor Ӧ�ó������ͷ�ļ�
//
#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"       // ������
#include "CurveInfo.h"
// CPlotEditorApp:
// �йش����ʵ�֣������ PlotEditor.cpp
//

class CPlotEditorApp : public CWinApp
{
public:
	CPlotEditorApp();
	CString GetResString(UINT uStringID);
	void GetModulePath();
public:
	HINSTANCE exe_hInstance;
	HANDLE hSem;//�ж���������Ƿ��Ѵ���
	CString strSemaphore;
    CString strSemaphoreExist;
    CString strSemaphoreCreatFail;
	CTypedPtrList < CPtrList, CCurveInfo * >curveList;
	bool bCurveFileExist;

	CString szExePath;//��ִ���ļ�����λ�þ���·��
	CString ModuleFilePath;
	CString TempPath;//��ʱ�ļ��о���·��
	CString LogPath;//log��־����λ�þ���·��
	CString IniFilePath;//ini�ļ��о���·��
	CString strIniFilePath;//ini�ļ�����λ�þ���·��
	CString DataPath;//��ʱ�ļ��о���·��
	CString JobPath;//��ҵ�ļ��еľ���·��
	CString ToolPath;//�����ļ��о���·��
	CString strUnitsFile;//��¼������λ�������ļ�
	CString strCurveFile;//��¼��ͼģ��������ļ�
	CString strSettingFile;//��¼�û����õĳ�����ֵ�����ļ�

// ��д
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	char *FromUNICODEToANSI(CString str);
	short FromUnicodeToUTF8 (LPSTR utf8Str, short utf8StrLen, WORD * unStr, unsigned short unMaxLen);
	short FromUTF8ToUnicode (unsigned char *utf8Str, short utf8StrLen, WORD * unStr, unsigned short unMaxLen);
	short Utf8TextBytes (const unsigned char *mstr);
// ʵ��
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CPlotEditorApp theApp;
