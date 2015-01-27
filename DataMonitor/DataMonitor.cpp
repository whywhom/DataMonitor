
// DMonitor.cpp : ����Ӧ�ó��������Ϊ��
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
	// �����ļ��ı�׼�ĵ�����
	ON_COMMAND(ID_FILE_NEW, &CWinApp::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, &CWinApp::OnFileOpen)
	// ��׼��ӡ��������
	ON_COMMAND(ID_FILE_PRINT_SETUP, &CWinApp::OnFilePrintSetup)
END_MESSAGE_MAP()


// CDataMonitorApp ����
static HINSTANCE _hLangDLL = NULL;
CDataMonitorApp::CDataMonitorApp()
{
	// ֧����������������
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_ALL_ASPECTS;
#ifdef _MANAGED
	// ���Ӧ�ó��������ù�����������ʱ֧��(/clr)�����ģ���:
	//     1) �����д˸������ã�������������������֧�ֲ�������������
	//     2) ��������Ŀ�У������밴������˳���� System.Windows.Forms ������á�
	System::Windows::Forms::Application::SetUnhandledExceptionMode(System::Windows::Forms::UnhandledExceptionMode::ThrowException);
#endif

	// TODO: ������Ӧ�ó��� ID �ַ����滻ΪΨһ�� ID �ַ�����������ַ�����ʽ
	//Ϊ CompanyName.ProductName.SubProduct.VersionInformation
	SetAppID(_T("DMonitor.AppID.NoVersion"));

	// TODO: �ڴ˴���ӹ�����룬
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��
}

// Ψһ��һ�� CDataMonitorApp ����

CDataMonitorApp theApp;


// CDataMonitorApp ��ʼ��

BOOL CDataMonitorApp::InitInstance()
{
	// ���һ�������� Windows XP �ϵ�Ӧ�ó����嵥ָ��Ҫ
	// ʹ�� ComCtl32.dll �汾 6 ����߰汾�����ÿ��ӻ���ʽ��
	//����Ҫ InitCommonControlsEx()�����򣬽��޷��������ڡ�
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// ��������Ϊ��������Ҫ��Ӧ�ó�����ʹ�õ�
	// �����ؼ��ࡣ
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();


	// ��ʼ�� OLE ��
	if (!AfxOleInit())
	{
		AfxMessageBox(IDP_OLE_INIT_FAILED);
		return FALSE;
	}

	AfxEnableControlContainer();

	EnableTaskbarInteraction(FALSE);

	// ʹ�� RichEdit �ؼ���Ҫ  AfxInitRichEdit2()	
	// AfxInitRichEdit2();

	// ��׼��ʼ��
	// ���δʹ����Щ���ܲ�ϣ����С
	// ���տ�ִ���ļ��Ĵ�С����Ӧ�Ƴ�����
	// ����Ҫ���ض���ʼ������
	// �������ڴ洢���õ�ע�����
	// TODO: Ӧ�ʵ��޸ĸ��ַ�����
	// �����޸�Ϊ��˾����֯��
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
	exe_hInstance = GetModuleHandle(NULL);//��ȡhInstance wuhaoyong add
	if(0 == exe_hInstance)
	{
		return FALSE;
	}
	GetModulePath();//��Ҫ�ļ��г�ʼ��
	LoadStdProfileSettings(1);  // ���ر�׼ INI �ļ�ѡ��(���� MRU)


	// ע��Ӧ�ó�����ĵ�ģ�塣�ĵ�ģ��
	// �������ĵ�����ܴ��ں���ͼ֮�������
	CSingleDocTemplate* pDocTemplate;
	pDocTemplate = new CSingleDocTemplate(
		IDR_MAINFRAME,
		RUNTIME_CLASS(CDataMonitorDoc),
		RUNTIME_CLASS(CMainFrame),       // �� SDI ��ܴ���
		RUNTIME_CLASS(CDataMonitorView));
	if (!pDocTemplate)
		return FALSE;
	AddDocTemplate(pDocTemplate);


	// ������׼ shell ���DDE�����ļ�������������
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);

	// ���á�DDE ִ�С�
	EnableShellOpen();
	RegisterShellFileTypes(TRUE);


	// ��������������ָ����������
	// �� /RegServer��/Register��/Unregserver �� /Unregister ����Ӧ�ó����򷵻� FALSE��
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;

	// Ψһ��һ�������ѳ�ʼ���������ʾ����������и���
	m_pMainWnd->ShowWindow(SW_SHOWMAXIMIZED);
	m_pMainWnd->UpdateWindow();
	// �������к�׺ʱ�ŵ��� DragAcceptFiles
	//  �� SDI Ӧ�ó����У���Ӧ�� ProcessShellCommand ֮����
	// ������/��
	m_pMainWnd->DragAcceptFiles();
	return TRUE;
}

//����_hLangDLL�Ƿ��������dll���ж�ʹ�����Կ���Դ����Ĭ����Դ��Ĭ��Ϊ���ļ���
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
		//��ʱ�ļ���·��
		TempPath = szPath;
		CString backupDir = _T("\\Temp\\");
		TempPath += backupDir;
		dwAttr=GetFileAttributes(TempPath);
		//���ļ��в����ڣ������ļ���
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
	//���ļ��в����ڣ������ļ���
	if(dwAttr==0xFFFFFFFF)
	{
		CreateDirectory(LogPath,NULL);
	}

	TempPath = ModuleFilePath + _T("\\Temp\\");
	//���ļ��в����ڣ������ļ���
	dwAttr=GetFileAttributes(TempPath);
	if(dwAttr==0xFFFFFFFF)
	{
		CreateDirectory(TempPath,NULL);
	}

	DataPath = ModuleFilePath + _T("\\Data\\");
	//���ļ��в����ڣ������ļ���
	dwAttr=GetFileAttributes(DataPath);
	if(dwAttr==0xFFFFFFFF)
	{
		CreateDirectory(DataPath,NULL);
	}

	JobPath = ModuleFilePath + _T("\\Jobs\\");
	//���ļ��в����ڣ������ļ���
	dwAttr=GetFileAttributes(JobPath);
	if(dwAttr==0xFFFFFFFF)
	{
		CreateDirectory(JobPath,NULL);
	}

	ToolPath = ModuleFilePath + _T("\\Tools\\");
	//���ļ��в����ڣ������ļ���
	dwAttr=GetFileAttributes(ToolPath);
	if(dwAttr==0xFFFFFFFF)
	{
		CreateDirectory(ToolPath,NULL);
	}

	IniFilePath = ModuleFilePath + _T("\\Config\\");
	//���ļ��в����ڣ������ļ���
	dwAttr=GetFileAttributes(IniFilePath);
	if(dwAttr==0xFFFFFFFF)
	{
		CreateDirectory(IniFilePath,NULL);
	}
	strUnitsFile = IniFilePath + _T("Units.xml");
	dwAttr=GetFileAttributes(strUnitsFile);
	if(dwAttr==0xFFFFFFFF)
	{
		CreateDmFile(strUnitsFile);
	}
	strIniFilePath = IniFilePath + _T("config.ini");
	dwAttr=GetFileAttributes(strIniFilePath);
	if(dwAttr==0xFFFFFFFF)
	{
		CreateIniFile(strIniFilePath);
	}
#ifdef FEATURE_LOG
	LogFileName = LogPath + _T("log.txt");
#endif
	//��ͼģ��
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

int CDataMonitorApp::CreateIniFile(CString strFile)
{
	::WritePrivateProfileString(_T("TargetDepth"),_T("Depth1"),_T("100"),strFile); 
	::WritePrivateProfileString(_T("TargetDepth"),_T("Depth2"),_T("100"),strFile);
	return 0;
}
int CDataMonitorApp::CreateDmFile(CString strFile)
{
	/*
	CFile downloadFile;
	CFileException eFileException;

	if(downloadFile != INVALID_HANDLE_VALUE)
	{
		downloadFile.Close();
	}
	if (!downloadFile.Open (strFile, CFile::modeCreate | CFile::modeWrite , &eFileException))
	{
		return 0;
	}
	*/
	TiXmlDocument doc;
	TiXmlDeclaration * decl = new TiXmlDeclaration( "1.0", "UTF-8", "yes" );
	doc.LinkEndChild( decl ); 
	TiXmlElement * element = new TiXmlElement( "Units" );
	////
	LinkElementeFuns(element,_T("mV"));
	LinkElementeFuns(element,_T("GAPI"));
	LinkElementeFuns(element,_T("CPS"));
	LinkElementeFuns(element,_T("us/m"));
	LinkElementeFuns(element,_T("m"));
	LinkElementeFuns(element,_T("cm"));
	LinkElementeFuns(element,_T("mm"));
	LinkElementeFuns(element,_T("uS"));
	LinkElementeFuns(element,_T("mS"));
	LinkElementeFuns(element,_T("S"));
	LinkElementeFuns(element,_T("M��"));
	LinkElementeFuns(element,_T("k��"));
	LinkElementeFuns(element,_T("��"));
	LinkElementeFuns(element,_T("��.m"));
    LinkElementeFuns(element,_T("s"));
    LinkElementeFuns(element,_T("S/m"));
    LinkElementeFuns(element,_T("mS/m"));
    LinkElementeFuns(element,_T("m/h"));
    LinkElementeFuns(element,_T("Kg"));
    LinkElementeFuns(element,_T("Kn"));
    LinkElementeFuns(element,_T("N"));
    LinkElementeFuns(element,_T("��"));
    LinkElementeFuns(element,_T("��/m"));
    LinkElementeFuns(element,_T("kg/m^3"));
    LinkElementeFuns(element,_T("g/cm^3"));
    LinkElementeFuns(element,_T("km"));
    LinkElementeFuns(element,_T("km^3"));
    LinkElementeFuns(element,_T("m^3"));
    LinkElementeFuns(element,_T("dm^3"));
    LinkElementeFuns(element,_T("m/s^2"));
    LinkElementeFuns(element,_T("mm/s^2"));
    LinkElementeFuns(element,_T("rad"));
    LinkElementeFuns(element,_T("mrad"));
    LinkElementeFuns(element,_T("urad"));
    LinkElementeFuns(element,_T("��"));
    LinkElementeFuns(element,_T("km^2"));
    LinkElementeFuns(element,_T("ha"));
    LinkElementeFuns(element,_T("dm^2"));
    LinkElementeFuns(element,_T("cm^2"));
    LinkElementeFuns(element,_T("mm^2"));
    LinkElementeFuns(element,_T("kg/s"));
    LinkElementeFuns(element,_T("m^3/s"));
    LinkElementeFuns(element,_T("m^3/min"));
    LinkElementeFuns(element,_T("m^3/d"));
    LinkElementeFuns(element,_T("L/s"));
    LinkElementeFuns(element,_T("Std.m3/m3"));
    LinkElementeFuns(element,_T("t"));
    LinkElementeFuns(element,_T("Mg/m^3"));
    LinkElementeFuns(element,_T("pu"));
    LinkElementeFuns(element,_T("J"));
    LinkElementeFuns(element,_T("Mev"));
    LinkElementeFuns(element,_T("MW"));
    LinkElementeFuns(element,_T("KW"));
    LinkElementeFuns(element,_T("W"));
    LinkElementeFuns(element,_T("Pa"));
    LinkElementeFuns(element,_T("kPa"));
    LinkElementeFuns(element,_T("MPa"));
    LinkElementeFuns(element,_T("ATM"));
    LinkElementeFuns(element,_T("PSI"));
    LinkElementeFuns(element,_T("Bq"));
    LinkElementeFuns(element,_T("r/s"));
    LinkElementeFuns(element,_T("min"));
    LinkElementeFuns(element,_T("h"));
    LinkElementeFuns(element,_T("d"));
    LinkElementeFuns(element,_T("a"));
    LinkElementeFuns(element,_T("m/s"));
    LinkElementeFuns(element,_T("Pa.s"));
    LinkElementeFuns(element,_T("mPa.s"));
    LinkElementeFuns(element,_T("N.s/m^2"));
    LinkElementeFuns(element,_T("%"));
    LinkElementeFuns(element,_T("NAPI"));
    LinkElementeFuns(element,_T("inch"));
    LinkElementeFuns(element,_T("API"));
    LinkElementeFuns(element,_T("V"));
    LinkElementeFuns(element,_T("��"));
    LinkElementeFuns(element,_T("uS/m"));
    LinkElementeFuns(element,_T("ppm"));
    LinkElementeFuns(element,_T("I"));

	doc.LinkEndChild( element );
	////
	char * pFileName = FromUNICODEToANSI(strFile);
	doc.SaveFile(pFileName);

	delete []pFileName;
	pFileName = NULL;
	return 0;
}
void CDataMonitorApp::LinkElementeFuns(TiXmlElement * element,CString str)
{
	TiXmlElement* msgs = new TiXmlElement( "Unit" ); 
	element->LinkEndChild( msgs );
	TiXmlElement* msg = new TiXmlElement( "Label" ); 
	////********/////transfer//////********//////
	const int maxBufferSize = 64;
	unsigned short UnicodeStr[maxBufferSize];
	unsigned short utf8StrLen;
	DWORD dwLength;//ת�����UTF��8����ĳ���in BYTEs
	char utf8Str[64];
	memset(utf8Str,0,maxBufferSize*sizeof(char));
	memset(UnicodeStr,0,maxBufferSize*sizeof(unsigned short));
	short UnicodeStrLen=2*str.GetLength();
	memcpy(UnicodeStr,str,UnicodeStrLen);
	utf8StrLen=6*UnicodeStrLen;
	memset(utf8Str,0,maxBufferSize*sizeof(char));
	dwLength=theApp.FromUnicodeToUTF8( utf8Str, utf8StrLen, UnicodeStr, UnicodeStrLen);//Unicode to UTF-8
	//////////********///////////
	msg->LinkEndChild( new TiXmlText( utf8Str )); 
	msgs->LinkEndChild( msg ); 
}
/**************************************************************************/
/***  specifications;              ***/
/***  NAME   = FromUNICODEToANSI;       ***/
/***  FUNCTION  = UNICODEת��ΪASCII��   ***/
/***  DATE   = 2008/11/18;           ***/
/***  AUTHOR  = wuhaoyong;              ***/
/***  INPUT   = *pData-UNICODE�ַ���,*nwDataLen-UNICODE�ַ�����Ч����,*pDataLen-ת��ΪASCII�ĳ���;               ***/
/***  OUTPUT  = *pBuffer-ASCII�ַ���;               ***/
/***  END OF SPECIFICATIONS;            ***/
/**************************************************************************/
char *CDataMonitorApp::FromUNICODEToANSI(CString str)
{
    UINT nCodePage = CP_ACP; //ASCII
	int strLen = str.GetLength();
    int len = WideCharToMultiByte(nCodePage,0,str,strLen,NULL,0,NULL,NULL);
    char * pFileName = new char[len+1];   //���ֽ�Ϊ��λ
	memset(pFileName,0,len+1);
	//���ֽڱ���ת���ɶ��ֽڱ���
    WideCharToMultiByte(CP_ACP,0,str,str.GetLength(),pFileName,len,NULL,NULL);
    return pFileName;
}

/*===========================================================================
   Function :Utf8 to Unicode
   Param    :   utf8Str        --UTF8 dest code
                    utf8strlen     --max utf8 length
                    unStr          -- Unicode Str Source
                    unMaxlen     --Unicode ������ 
   return   :  --ʵ��ת���ĳ���
 
   Author   : 
   Data     : 2002-10-24
===========================================================================*/
short CDataMonitorApp::FromUnicodeToUTF8 (LPSTR utf8Str, short utf8StrLen, WORD * unStr, unsigned short unMaxLen)
{
    //short len_dest=0;
    short unlen = 0;            //ת����unicode ����
    short utflen = 0;           //ת����utf8 ����

    WORD *src;
    src = unStr;
    for (; (unlen <= unMaxLen - 1) && (*src != 0x00) && (utflen <= utf8StrLen);)
    {                           //���ճ��ȡ�0x00����
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

int CDataMonitorApp::ExitInstance()
{
	//TODO: �����������ӵĸ�����Դ
	AfxOleTerm(FALSE);

	return CWinApp::ExitInstance();
}

// CDataMonitorApp ��Ϣ�������


// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
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

// �������жԻ����Ӧ�ó�������
void CDataMonitorApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

// CDataMonitorApp ��Ϣ�������



