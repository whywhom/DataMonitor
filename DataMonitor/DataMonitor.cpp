
// DMonitor.cpp : 定义应用程序的类行为。
//

#include "stdafx.h"
#include "afxwinappex.h"
#include "afxdialogex.h"
#include "DataMonitor.h"
#include "MainFrm.h"

#include "DataMonitorDoc.h"
#include "DataMonitorView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CDataMonitorApp

BEGIN_MESSAGE_MAP(CDataMonitorApp, CWinApp)
	ON_COMMAND(ID_APP_ABOUT, &CDataMonitorApp::OnAppAbout)
	// 基于文件的标准文档命令
	ON_COMMAND(ID_FILE_NEW, &CWinApp::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, &CWinApp::OnFileOpen)
	// 标准打印设置命令
	ON_COMMAND(ID_FILE_PRINT_SETUP, &CWinApp::OnFilePrintSetup)
END_MESSAGE_MAP()


// CDataMonitorApp 构造
static HINSTANCE _hLangDLL = NULL;
CDataMonitorApp::CDataMonitorApp()
{
	// 支持重新启动管理器
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_ALL_ASPECTS;
#ifdef _MANAGED
	// 如果应用程序是利用公共语言运行时支持(/clr)构建的，则:
	//     1) 必须有此附加设置，“重新启动管理器”支持才能正常工作。
	//     2) 在您的项目中，您必须按照生成顺序向 System.Windows.Forms 添加引用。
	System::Windows::Forms::Application::SetUnhandledExceptionMode(System::Windows::Forms::UnhandledExceptionMode::ThrowException);
#endif

	// TODO: 将以下应用程序 ID 字符串替换为唯一的 ID 字符串；建议的字符串格式
	//为 CompanyName.ProductName.SubProduct.VersionInformation
	SetAppID(_T("DMonitor.AppID.NoVersion"));

	// TODO: 在此处添加构造代码，
	// 将所有重要的初始化放置在 InitInstance 中
}

// 唯一的一个 CDataMonitorApp 对象

CDataMonitorApp theApp;


// CDataMonitorApp 初始化

BOOL CDataMonitorApp::InitInstance()
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


	// 初始化 OLE 库
	if (!AfxOleInit())
	{
		AfxMessageBox(IDP_OLE_INIT_FAILED);
		return FALSE;
	}

	AfxEnableControlContainer();

	EnableTaskbarInteraction(FALSE);

	// 使用 RichEdit 控件需要  AfxInitRichEdit2()	
	// AfxInitRichEdit2();

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
	LoadStdProfileSettings(1);  // 加载标准 INI 文件选项(包括 MRU)


	// 注册应用程序的文档模板。文档模板
	// 将用作文档、框架窗口和视图之间的连接
	CSingleDocTemplate* pDocTemplate;
	pDocTemplate = new CSingleDocTemplate(
		IDR_MAINFRAME,
		RUNTIME_CLASS(CDataMonitorDoc),
		RUNTIME_CLASS(CMainFrame),       // 主 SDI 框架窗口
		RUNTIME_CLASS(CDataMonitorView));
	if (!pDocTemplate)
		return FALSE;
	AddDocTemplate(pDocTemplate);


	// 分析标准 shell 命令、DDE、打开文件操作的命令行
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);

	// 启用“DDE 执行”
	EnableShellOpen();
	RegisterShellFileTypes(TRUE);


	// 调度在命令行中指定的命令。如果
	// 用 /RegServer、/Register、/Unregserver 或 /Unregister 启动应用程序，则返回 FALSE。
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;

	// 唯一的一个窗口已初始化，因此显示它并对其进行更新
	m_pMainWnd->ShowWindow(SW_SHOWMAXIMIZED);
	m_pMainWnd->UpdateWindow();
	// 仅当具有后缀时才调用 DragAcceptFiles
	//  在 SDI 应用程序中，这应在 ProcessShellCommand 之后发生
	// 启用拖/放
	m_pMainWnd->DragAcceptFiles();
	return TRUE;
}

//根据_hLangDLL是否加载语言dll来判断使用语言库资源还是默认资源，默认为中文简体
CString CDataMonitorApp::GetResString(UINT uStringID)
{
	CString resString;
	if (_hLangDLL)
		resString.LoadString(_hLangDLL, uStringID);
	if (resString.IsEmpty())
		resString.LoadString(theApp.exe_hInstance, uStringID);
	return resString;
}

void CDataMonitorApp::GetModulePath()
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

	DataPath = ModuleFilePath + _T("\\Data\\");
	//若文件夹不存在，创建文件夹
	dwAttr=GetFileAttributes(DataPath);
	if(dwAttr==0xFFFFFFFF)
	{
		CreateDirectory(DataPath,NULL);
	}

	JobPath = ModuleFilePath + _T("\\Jobs\\");
	//若文件夹不存在，创建文件夹
	dwAttr=GetFileAttributes(JobPath);
	if(dwAttr==0xFFFFFFFF)
	{
		CreateDirectory(JobPath,NULL);
	}

	ToolPath = ModuleFilePath + _T("\\Tools\\");
	//若文件夹不存在，创建文件夹
	dwAttr=GetFileAttributes(ToolPath);
	if(dwAttr==0xFFFFFFFF)
	{
		CreateDirectory(ToolPath,NULL);
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

int CDataMonitorApp::ExitInstance()
{
	//TODO: 处理可能已添加的附加资源
	AfxOleTerm(FALSE);

	return CWinApp::ExitInstance();
}

// CDataMonitorApp 消息处理程序


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()

// 用于运行对话框的应用程序命令
void CDataMonitorApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

// CDataMonitorApp 消息处理程序



