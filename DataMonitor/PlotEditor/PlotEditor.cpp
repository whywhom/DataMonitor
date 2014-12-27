
// PlotEditor.cpp : 定义应用程序的类行为。
//

#include "stdafx.h"
#include "afxwinappex.h"
#include "afxdialogex.h"
#include "PlotEditor.h"
#include "MainFrm.h"

#include "PlotEditorDoc.h"
#include "PlotEditorView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CPlotEditorApp

BEGIN_MESSAGE_MAP(CPlotEditorApp, CWinApp)
	ON_COMMAND(ID_APP_ABOUT, &CPlotEditorApp::OnAppAbout)
	// 基于文件的标准文档命令
	ON_COMMAND(ID_FILE_NEW, &CWinApp::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, &CWinApp::OnFileOpen)
	// 标准打印设置命令
	ON_COMMAND(ID_FILE_PRINT_SETUP, &CWinApp::OnFilePrintSetup)
END_MESSAGE_MAP()


// CPlotEditorApp 构造

CPlotEditorApp::CPlotEditorApp()
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
	SetAppID(_T("PlotEditor.AppID.NoVersion"));

	// TODO: 在此处添加构造代码，
	// 将所有重要的初始化放置在 InitInstance 中
	
}

// 唯一的一个 CPlotEditorApp 对象

CPlotEditorApp theApp;


// CPlotEditorApp 初始化

BOOL CPlotEditorApp::InitInstance()
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
	SetRegistryKey(_T("PlotEditor"));
	LoadStdProfileSettings(2);  // 加载标准 INI 文件选项(包括 MRU)
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
	GetModulePath();
	// 注册应用程序的文档模板。文档模板
	// 将用作文档、框架窗口和视图之间的连接
	CSingleDocTemplate* pDocTemplate;
	pDocTemplate = new CSingleDocTemplate(
		IDR_MAINFRAME,
		RUNTIME_CLASS(CPlotEditorDoc),
		RUNTIME_CLASS(CMainFrame),       // 主 SDI 框架窗口
		RUNTIME_CLASS(CPlotEditorView));
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
	m_pMainWnd->ShowWindow(SW_SHOW);
	m_pMainWnd->UpdateWindow();
	// 仅当具有后缀时才调用 DragAcceptFiles
	//  在 SDI 应用程序中，这应在 ProcessShellCommand 之后发生
	// 启用拖/放
	m_pMainWnd->DragAcceptFiles();
	return TRUE;
}
//根据_hLangDLL是否加载语言dll来判断使用语言库资源还是默认资源，默认为中文简体
CString CPlotEditorApp::GetResString(UINT uStringID)
{
	CString resString;
	if (resString.IsEmpty())
		resString.LoadString(theApp.exe_hInstance, uStringID);
	return resString;
}

void CPlotEditorApp::GetModulePath()
{
	DWORD dwAttr = 0;
	TCHAR szModPath[_MAX_PATH];
	GetModuleFileName(m_hInstance, szModPath, _MAX_PATH);
    szExePath = szModPath;
    ModuleFilePath = szExePath.Left(szExePath.ReverseFind('\\'));

	IniFilePath = ModuleFilePath + _T("\\Config\\");
	//若文件夹不存在，创建文件夹
	dwAttr=GetFileAttributes(IniFilePath);
	if(dwAttr==0xFFFFFFFF)
	{
		CreateDirectory(IniFilePath,NULL);
	}
	
	//绘图模板
	strCurveFile = IniFilePath + _T("Curve.xml");
	dwAttr=GetFileAttributes(strCurveFile);
	if(dwAttr==0xFFFFFFFF)
	{
		bCurveFileExist = false;
	}
	else
	{
		bCurveFileExist = true;
	}
	return;
}

/**************************************************************************/
/***  specifications;              ***/
/***  NAME   = FromUNICODEToANSI;       ***/
/***  FUNCTION  = UNICODE转换为ASCII码   ***/
/***  DATE   = 2008/11/18;           ***/
/***  AUTHOR  = wuhaoyong;              ***/
/***  INPUT   = *pData-UNICODE字符串,*nwDataLen-UNICODE字符串有效长度,*pDataLen-转换为ASCII的长度;               ***/
/***  OUTPUT  = *pBuffer-ASCII字符串;               ***/
/***  END OF SPECIFICATIONS;            ***/
/**************************************************************************/
char *CPlotEditorApp::FromUNICODEToANSI(CString str)
{
    UINT nCodePage = CP_ACP; //ASCII
	int strLen = str.GetLength();
    int len = WideCharToMultiByte(nCodePage,0,str,strLen,NULL,0,NULL,NULL);
    char * pFileName = new char[len+1];   //以字节为单位
	memset(pFileName,0,len+1);
	//宽字节编码转换成多字节编码
    WideCharToMultiByte(CP_ACP,0,str,str.GetLength(),pFileName,len,NULL,NULL);
    return pFileName;
}

/*===========================================================================
   Function :Utf8 to Unicode
   Param    :   utf8Str        --UTF8 dest code
                    utf8strlen     --max utf8 length
                    unStr          -- Unicode Str Source
                    unMaxlen     --Unicode 最大个数 
   return   :  --实际转化的长度
 
   Author   : 
   Data     : 2002-10-24
===========================================================================*/
short CPlotEditorApp::FromUnicodeToUTF8 (LPSTR utf8Str, short utf8StrLen, WORD * unStr, unsigned short unMaxLen)
{
    //short len_dest=0;
    short unlen = 0;            //转化的unicode 长度
    short utflen = 0;           //转化的utf8 长度

    WORD *src;
    src = unStr;
    for (; (unlen <= unMaxLen - 1) && (*src != 0x00) && (utflen <= utf8StrLen);)
    {                           //按照长度、0x00结束
        if (((*src) & 0xff80) == 0) //1byte
        {
            *utf8Str = (char) (*src);
            utflen += 1;
        }
        else if (((*src) & 0xf800) == 0) //2bytes
        {
            *utf8Str = (char) ((((*src) >> 6) & 0x1f) | 0xc0);
            utf8Str++;
            *utf8Str = (char) (((*src) & 0x003f) | 0x80);
            utflen += 2;
        }
        else                    //3bytes
        {
            *utf8Str = (char) ((((*src) >> 12) & 0x000f) | 0xe0);
            utf8Str++;
            *utf8Str = (char) ((((*src) >> 6) & 0x3f) | 0x80);
            utf8Str++;
            *utf8Str = (char) (((*src) & 0x003f) | 0x80);
            utflen += 3;
        }
        unlen++;                
        src++;
        utf8Str++;
    }
    *utf8Str = 0x00;
    return utflen;

}

/*===========================================================================
    Function :Unicode to UTF8
    Param : utf8Str     --UTF8 dest code
    utf8strlen    --max utf8 length
    unStr     -- Unicode Str Source
    unMaxlen  --Unicode 最大个数 
    return :  --实际转化的长度
  
    Author : 
    Data  : 2002-10-24
===========================================================================*/
short CPlotEditorApp::FromUTF8ToUnicode (unsigned char *utf8Str, short utf8StrLen, WORD * unStr, unsigned short unMaxLen)
{

    short zen_han, len_dest;
    unsigned char *p_src;
    unsigned char utf8_len = 0;
    len_dest = 0;
    p_src = (unsigned char *) (utf8Str);

    /* limit the total bytes of the *mstr */
    if (utf8StrLen <= 0)
    {
        return (0);
    }

    /* converting at here utf8_len <= utf8StrLen 改为 utf8_len < utf8StrLen*/
    for (; (*p_src != 0x00) && (len_dest <= unMaxLen - 1) && (utf8_len < utf8StrLen);)
    {
        zen_han = Utf8TextBytes ((const unsigned char *) p_src);
        if (zen_han == 1)       //ASCII, just add 0x00 at beginning
        {
            (*unStr) = *(p_src);
            unStr++;
            p_src++;
            utf8_len++;
            len_dest += 1;
        }
        else if (zen_han == 2)
        {
            *unStr = (((((*p_src) >> 2) & (unsigned char) 0x07)) & 0x00ff) << 8;
            *unStr |= ((((*p_src) << 6) | ((*(p_src + 1)) & (unsigned char) 0x3f))) & 0x00ff;
            unStr++;
            p_src += 2;
            utf8_len += 2;
            len_dest += 1;

        }
        else if (zen_han == 3)
        {
            *(unStr) = ((((*(p_src)) << 4) | (((*(p_src + 1)) >> 2) & 0x0f)) & 0x00ff) << 8;

            *unStr |= ((((*(p_src + 1)) << 6) | ((*(p_src + 2)) & (unsigned char) 0x3f))) & 0x00ff;
            unStr++;
            p_src += 3;
            utf8_len += 3;
            len_dest += 1;
        }
        else if (zen_han == 4)  //
        {
            *unStr = (*(p_src)) & 0x7;
            for (int i=1; i<4;i++)
            {
                *unStr <<= 6; // 左移6位后与后续字节的有效位值"位或"赋值
                *unStr = *unStr | ((*(p_src + i)) & 0x3f);//先与后或
            }
            unStr++;
            p_src += 4;
            utf8_len += 4;
            len_dest += 1;
        }
        else if (zen_han == 5)  //
        {
            *unStr = (*(p_src)) & 0x3;
            for (int i=1; i<5;i++)
            {
                *unStr <<= 6; // 左移6位后与后续字节的有效位值"位或"赋值
                *unStr = *unStr | ((*(p_src + i)) & 0x3f);//先与后或
            }
            unStr++;
            p_src += 5;
            utf8_len += 5;
            len_dest += 1;
        }
        else if (zen_han == 6)  //
        {
            *unStr = (*(p_src)) & 0x1;
            for (int i=1; i<6;i++)
            {
                *unStr <<= 6; // 左移6位后与后续字节的有效位值"位或"赋值
                *unStr = *unStr | ((*(p_src + i)) & 0x3f);//先与后或
            }
            unStr++;
            p_src += 6;
            utf8_len += 6;
            len_dest += 1;
        }
        else                    //treated as illegal character, search the next character
        {
            p_src++;
            utf8_len++;
        }
    }
    *(unStr) = 0x0000;
    return (len_dest);

}

/* ===============================================
  Function:  Returns the number of bytes used by the first character of the
 
  string 'mstr'.  For ASCII text this will always return 1.
  当mstr指向0时，或*mstr的第一个字符编码不是有效的UTF-8编码时，返回0
   The table below summarizes the format of these different octet types.
   The letter x indicates bits available for encoding bits of the UCS-4
   character value.(get this table from rfc2279)
 UCS-4 range (hex.)    UTF-8 octet sequence (binary)
 0000 0000-0000 007F   0xxxxxxx
 0000 0080-0000 07FF   110xxxxx 10xxxxxx
 0000 0800-0000 FFFF   1110xxxx 10xxxxxx 10xxxxxx
 0001 0000-001F FFFF   11110xxx 10xxxxxx 10xxxxxx 10xxxxxx
 0020 0000-03FF FFFF   111110xx 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx
 0400 0000-7FFF FFFF   1111110x 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx
 
 非法UTF-8初始序列(get this table from internet,this is right?)
 UTF-8序列   非法的原因 
 10xxxxxx   作为字符的起始字节非法(80..BF) 
 1100000x   非法，过长(C0 80..BF) 
 11100000 100xxxxx 非法，过长(E0 80..9F) 
 11110000 1000xxxx 非法，过长(F0 80..8F) 
 11111000 10000xxx 非法，过长(F8 80..87) 
 11111100 100000xx 非法，过长(FC 80..83) 
 1111111x   非法；为规格说明所禁止 
====================================================== */

short CPlotEditorApp::Utf8TextBytes (const unsigned char *mstr)
{
    short textbytes = 0;
    if (mstr == 0)
    {                           //Returns 0 if mstr contains a null string or if there is
        return 0;               //no valid character in mstr.
    }
    /* code checking.
       because all unicode used now is lower than 0xffff, only 1~3 bytes UTF-8 has means, 
       the other length will not be dealed with in other functions */
    if (((*mstr) & 0x80) == 0)
    {
        textbytes = 1;          //0xxxxxxx, normal ASCII code
    }
    else if ((((*mstr) & 0xe0) == 0xc0) && (((*(mstr + 1)) & 0xc0) == 0x80))
    {
        if (((*mstr) & 0xfe) == 0xc0) //1100,000x is illegal data,
        {
            textbytes = 0;
        }
        else
        {
            textbytes = 2;      //110xxxxx 10xxxxxx
        }
    }
    else if ((((*mstr) & 0xf0) == 0xe0) && (((*(mstr + 1)) & 0xc0) == 0x80) && (((*(mstr + 2)) & 0xc0) == 0x80))
    {
        if ((*mstr == (char) 0xe0) && (((*(mstr + 1)) & 0xe0) == 0x80)) //1110,0000,100x,xxxx is illegal data
        {
            textbytes = 0;
        }
        else
        {
            textbytes = 3;      //1110xxxx 10xxxxxx 10xxxxxx
        }
    }
    else if ((((*mstr) & 0xf8) == 0xf0) && (((*(mstr + 1)) & 0xc0) == 0x80) && (((*(mstr + 2)) & 0xc0) == 0x80)
             && (((*(mstr + 3)) & 0xc0) == 0x80))
    {
        if (((*mstr) == (char) 0xf0) && (((*(mstr + 1)) & 0xf0) == 0x80)) // 1111,0000,1000,xxxx is illegal data
        {
            textbytes = 0;
        }
        else
        {
            textbytes = 4;      // 11110xxx 10xxxxxx 10xxxxxx 10xxxxxx
        }
    }
    else if ((((*mstr) & 0xfc) == 0xf8) && (((*(mstr + 1)) & 0xc0) == 0x80) &&
             (((*(mstr + 2)) & 0xc0) == 0x80) && (((*(mstr + 3)) & 0xc0) == 0x80) && (((*(mstr + 4)) & 0xc0) == 0x80))
    {
        if ((*mstr == (char) 0xf8) && (((*(mstr + 1)) & 0xf8) == 0x80)) //1111,1000,1000,0xxx is illegal data
        {
            textbytes = 0;
        }
        else
        {
            textbytes = 5;      //111110xx 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx
        }
    }
    else if ((((*mstr) & 0xfe) == 0xfc) && (((*(mstr + 1)) & 0xc0) == 0x80) &&
             (((*(mstr + 2)) & 0xc0) == 0x80) && (((*(mstr + 3)) & 0xc0) == 0x80) && (((*(mstr + 4)) & 0xc0) == 0x80) &&
             (((*(mstr + 5)) & 0xc0) == 0x80))
    {
        if ((*mstr == (char) 0xfc) && (((*(mstr + 1)) & 0xfc) == 0x80)) //1111,1100,1000,00xx is illegal data
        {
            textbytes = 0;
        }
        else
        {
            textbytes = 6;      //1111110x 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx
        }
    }
    else
    {
        textbytes = 0;          //illegal formal, return 0
    }

    return textbytes;
}

int CPlotEditorApp::ExitInstance()
{
	//TODO: 处理可能已添加的附加资源
	AfxOleTerm(FALSE);

	return CWinApp::ExitInstance();
}

// CPlotEditorApp 消息处理程序


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
void CPlotEditorApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}


// CPlotEditorApp 消息处理程序



