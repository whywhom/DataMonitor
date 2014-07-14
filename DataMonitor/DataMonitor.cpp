
// DataMonitor.cpp : 定义应用程序的类行为。
//

#include "stdafx.h"
#include "DataMonitor.h"
#include "MonitorDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// 唯一的一个 CMonitorApp 对象

CMonitorApp theApp;

// CMonitorApp

BEGIN_MESSAGE_MAP(CMonitorApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()

static HINSTANCE _hLangDLL = NULL;

// CMonitorApp 构造

CMonitorApp::CMonitorApp()
{
	// 支持重新启动管理器
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_RESTART;

	// TODO: 在此处添加构造代码，
	// 将所有重要的初始化放置在 InitInstance 中
}


// CMonitorApp 初始化

BOOL CMonitorApp::InitInstance()
{
	// 如果一个运行在 Windows XP 上的应用程序清单指定要
	// 使用 ComCtl32.dll 版本 6 或更高版本来启用可视化方式，
	//则需要 InitCommonControlsEx()。否则，将无法创建窗口。
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// 将它设置为包括所有要在应用程序中使用的
	// 公共控件类。
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();


	AfxEnableControlContainer();

	// 创建 shell 管理器，以防对话框包含
	// 任何 shell 树视图控件或 shell 列表视图控件。
	CShellManager *pShellManager = new CShellManager;

	// 标准初始化
	// 如果未使用这些功能并希望减小
	// 最终可执行文件的大小，则应移除下列
	// 不需要的特定初始化例程
	// 更改用于存储设置的注册表项
	// TODO: 应适当修改该字符串，
	// 例如修改为公司或组织名
	SetRegistryKey(_T("DataMonitor"));
	/*wuhaoyong add CMutex*/
    //Creat CMutex
    hSem=NULL;
	strSemaphore.LoadString( IDS_SEMAPHORE );
    hSem=CreateSemaphore(NULL,1,1,strSemaphore);
    if(hSem)  //Creat CMutex success
    {
        //the CMutex exist
        if(ERROR_ALREADY_EXISTS==GetLastError())
        {
			strSemaphoreExist.LoadString( IDS_SEMAPHORE_ALREADY_EXISTS );
            AfxMessageBox(strSemaphoreExist);
            CloseHandle(hSem);      //Close CMutex
            return FALSE;           //Exit App
        }
    }
    else
    {
		strSemaphoreCreatFail.LoadString(IDS_SEMAPHORE_CREATE_FAIL);
        AfxMessageBox(strSemaphoreCreatFail);
        return FALSE;
    }
	exe_hInstance = GetModuleHandle(NULL);//获取hInstance wuhaoyong add
	if(0 == exe_hInstance)
	{
		return FALSE;
	}
	GetModulePath();//必要文件夹初始化
	CMonitorDlg dlg;
	m_pMainWnd = &dlg;
	INT_PTR nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: 在此放置处理何时用
		//  “确定”来关闭对话框的代码
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: 在此放置处理何时用
		//  “取消”来关闭对话框的代码
	}

	// 删除上面创建的 shell 管理器。
	if (pShellManager != NULL)
	{
		delete pShellManager;
	}

	// 由于对话框已关闭，所以将返回 FALSE 以便退出应用程序，
	//  而不是启动应用程序的消息泵。
	return FALSE;
}

//根据_hLangDLL是否加载语言dll来判断使用语言库资源还是默认资源，默认为中文简体
CString CMonitorApp::GetResString(UINT uStringID)
{
	CString resString;
	if (_hLangDLL)
		resString.LoadString(_hLangDLL, uStringID);
	if (resString.IsEmpty())
		resString.LoadString(theApp.exe_hInstance, uStringID);
	return resString;
}

void CMonitorApp::GetModulePath()
{
	DWORD dwAttr = 0;
	/******
	TCHAR szPath[MAX_PATH];
	if(SUCCEEDED(SHGetFolderPath(NULL, 
									CSIDL_APPDATA, 
									NULL, 
									0, 
									szPath))) 
	{
		//临时文件夹路径
		TempPath = szPath;
		CString backupDir = _T("\\Temp\\");
		TempPath += backupDir;
		dwAttr=GetFileAttributes(TempPath);
		//若文件夹不存在，创建文件夹
		if(dwAttr==0xFFFFFFFF)
		{
			CreateDirectory(TempPath,NULL);
		}
	}
	******/
	TCHAR szModPath[_MAX_PATH];
	GetModuleFileName(m_hInstance, szModPath, _MAX_PATH);
    szExePath = szModPath;
    ModuleFilePath = szExePath.Left(szExePath.ReverseFind('\\'));
	LogPath = ModuleFilePath + _T("\\Log\\");
	dwAttr=GetFileAttributes(LogPath);
	//若文件夹不存在，创建文件夹
	if(dwAttr==0xFFFFFFFF)
	{
		CreateDirectory(LogPath,NULL);
	}

	TempPath = ModuleFilePath + _T("\\Temp\\");
	//若文件夹不存在，创建文件夹
	dwAttr=GetFileAttributes(TempPath);
	if(dwAttr==0xFFFFFFFF)
	{
		CreateDirectory(TempPath,NULL);
	}

	IniFilePath = ModuleFilePath + _T("\\Config\\");
	//若文件夹不存在，创建文件夹
	dwAttr=GetFileAttributes(IniFilePath);
	if(dwAttr==0xFFFFFFFF)
	{
		CreateDirectory(IniFilePath,NULL);
	}
	strIniFilePath = IniFilePath + _T("config.ini");
#ifdef FEATURE_LOG
	LogFileName = LogPath + _T("log.txt");
#endif
	return;
}

