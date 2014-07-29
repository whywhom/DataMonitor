
// MonitorDlg.cpp : 实现文件
//

#include "stdafx.h"
#include<iostream>
#include <fstream>
#include "DataMonitor.h"
#include "MonitorDlg.h"
#include "afxdialogex.h"
#include "PublicInterface.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif
using namespace std;
static bool bRunning;
static CTypedPtrList < CPtrList, SEND_COMMAND_LIST * >senCommandList;
//
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
public:

	afx_msg void OnMenuInstrument();
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


// CMonitorDlg 对话框




CMonitorDlg::CMonitorDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CMonitorDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	theApp.activeWnd = NULL;
	fp = NULL;
	initWndList();
}
CMonitorDlg::~CMonitorDlg()
{
	theApp.activeWnd = NULL;
	initWndList();
}
void CMonitorDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CMonitorDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDOK, &CMonitorDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CMonitorDlg::OnBnClickedCancel)
	ON_COMMAND(ID_MENU_ABOUT, &CMonitorDlg::OnMenuAbout)
	ON_UPDATE_COMMAND_UI(ID_MENU_ABOUT, &CMonitorDlg::OnUpdateMenuAbout)
	ON_COMMAND(ID_MENU_EXIT, &CMonitorDlg::OnMenuExit)
	ON_MESSAGE( WM_USER_RECEIVEDATA, OnCommReceive)//接收端口消息
	ON_WM_SHOWWINDOW()
	ON_WM_GETMINMAXINFO()
	ON_WM_CLOSE()
	ON_COMMAND(ID_MENU_NEW, &CMonitorDlg::OnMenuNew)
	ON_COMMAND(ID_MENU_OPEN, &CMonitorDlg::OnMenuOpen)
	ON_COMMAND(ID_MENU_WORK, &CMonitorDlg::OnMenuWork)
	ON_COMMAND(ID_MENU_INSTRUMENT, &CMonitorDlg::OnMenuInstrument)
	ON_WM_SIZE()
	ON_COMMAND(ID_TOOLBAR_CONNECT, &CMonitorDlg::OnToolbarConnect)
	ON_COMMAND(ID_TOOLBAR_DISCONNECT, &CMonitorDlg::OnToolbarDisconnect)
END_MESSAGE_MAP()


// CMonitorDlg 消息处理程序

BOOL CMonitorDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标
	// TODO: 在此添加额外的初始化代码
	if (!m_ToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP
		| CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		!m_ToolBar.LoadToolBar(IDR_TOOLBAR_MAIN))  //IDR_TOOLBAR为工具条资源的ID号
	{
		TRACE0("未能创建工具栏\n");
		return -1;      // 未能创建
	}
	m_ToolBar.ShowWindow(SW_SHOW);   
	RepositionBars(AFX_IDW_CONTROLBAR_FIRST,   AFX_IDW_CONTROLBAR_LAST,   0); 
	CWnd::SetWindowText(theApp.GetResString(IDS_PRODUCT_NAME));
	
	theApp.commLayer.SetConnectType(TYPE_NONE);

	theApp.commLayer.fatherHwnd = (AfxGetMainWnd()->GetSafeHwnd());//获取HWND，赋值给通信层进行消息传递
	bRunning = true;
	StartThread();//启动线程

	ChildWndInitialize();//初始化子窗体
	wndList.AddTail(childDrawWnd);
	ShowActiveDlg(childDrawWnd, 0);
	//设置主界面大小
	CWnd::SetWindowPos(NULL,0,0,GetSystemMetrics (SM_CXFULLSCREEN),GetSystemMetrics (SM_CYFULLSCREEN),0/*WS_SIZEBOX|SWP_NOZORDER|SWP_NOMOVE*/);
	CenterWindow();//窗体居中
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

bool CMonitorDlg::ChildWndInitialize()
{
	//创建子窗体
	childDrawWnd = new CChildDraw;
	childDrawWnd->Create(IDD_CHILD_DRAW);
	return true;
}

void CMonitorDlg::OnMenuAbout()
{
	// TODO: 在此添加命令处理程序代码
	CAboutDlg dlgAbout;
	dlgAbout.DoModal();
}


void CMonitorDlg::OnUpdateMenuAbout(CCmdUI *pCmdUI)
{
	// TODO: 在此添加命令更新用户界面处理程序代码
}

void CMonitorDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CMonitorDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CMonitorDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

bool CMonitorDlg::StartThread()
{
    if (!(m_Thread = AfxBeginThread(MainThread, this)))
    {
    	m_Thread=NULL;
        return FALSE;
    }
	TRACE(_T("Thread started\n"));
    return TRUE;
}

UINT CMonitorDlg::MainThread(LPVOID pParam)
{
	while(bRunning)
	{
		if(!senCommandList.IsEmpty())
		{
			SEND_COMMAND_LIST* pCurrent=NULL;
			pCurrent=senCommandList.RemoveHead();
			if(pCurrent->cmd == _T("connect"))
			{
				//connect device
				theApp.m_CommResault=theApp.commLayer.CreatConnect();
				if(theApp.m_CommResault==COMM_SUCCESS)
				{
					TRACE(_T("Communication OK!\n"));

				}
				if(theApp.m_CommResault==COMM_ERROE_HARDWARE_CONNECT_FAIL)
				{
					TRACE(_T("Communication Fail!\n"));

				}
			}
			else if(pCurrent->cmd == _T("senddata"))
			{
				//connect device
				theApp.m_CommResault=theApp.commLayer.SendCommand(pCurrent->buf,pCurrent->size);
			}
			delete pCurrent;
			pCurrent=NULL;

		}
		//TRACE(_T("MainThread is running \n"));
		Sleep(100);
	}
	TRACE(_T("Exit MainThread \n"));
	return 0;
}

void CMonitorDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	CDialogEx::OnOK();
}


void CMonitorDlg::OnBnClickedCancel()
{
	// TODO: 在此添加控件通知处理程序代码
	CDialogEx::OnCancel();
}

BOOL CMonitorDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: Add your specialized code here and/or call the base class
	switch (pMsg->message)
	{
	case WM_KEYDOWN:
		{
			switch (pMsg->wParam)
			{
			case VK_ESCAPE:
				return TRUE;
			case VK_UP://屏蔽上下方向键
			case VK_DOWN:	
			case VK_SPACE:
			case VK_RETURN://屏蔽回车
				return TRUE;
			default:
				break;
			}
		}
		break;
	default:
		break;
	}
	return CDialogEx::PreTranslateMessage(pMsg);
}



void CMonitorDlg::OnMenuExit()
{
	// TODO: 在此添加命令处理程序代码
	SendMessage(WM_CLOSE);
}
//===========================================================================
//void CreatConnect( )
// 函数：取得连接
// 参数：
// 返回值：void
// 功能描述：
// Author：wuhaoyong
// Date：20140708
//===========================================================================
DWORD CMonitorDlg::CreatConnect( )
{
	theApp.m_CommResault=theApp.commLayer.CreatConnect();
	if(theApp.m_CommResault==COMM_SUCCESS)
	{
		TRACE(_T("Communication OK!\n"));

	}
	if(theApp.m_CommResault==COMM_ERROE_HARDWARE_CONNECT_FAIL)
	{
		TRACE(_T("Communication Fail!\n"));

	}
	return 0;
}

bool isConnected()
{
	return theApp.commLayer.m_bConnectEffective;
}
//===========================================================================
//void DestroyConnect( )
// 函数：断开连接
// 参数：
// 返回值：DWORD
// 功能描述：
// Author：wuhaoyong
// Date：20140708
//===========================================================================
DWORD CMonitorDlg::DestroyConnect( )
{
	theApp.commLayer.m_SerialPort.ClosePort();//关闭串口
    if (theApp.commLayer.m_SerialPort.m_bThreadAlive)
    {
        if(theApp.commLayer.m_SerialPort.m_Thread->m_hThread!=NULL)
        {
            TerminateThread (theApp.commLayer.m_SerialPort.m_Thread->m_hThread, 0);
            theApp.commLayer.m_SerialPort.m_Thread=NULL;
            theApp.commLayer.m_SerialPort.m_bThreadAlive=FALSE;
        }
    }
    return 0;
}

//===========================================================================
// void DataReceive(BYTE* inbuff, DWORD* dwSize)
// 函数：接收数据外部接口
// 参数： [in] inbuff:接收数据buf;dwSize:接收数据大小
// 返回值：void
// 功能描述：交互过程中的数据传递。
// Author：wuhaoyong
// Date：20140708
//===========================================================================
void CMonitorDlg::DataReceive(BYTE* inbuff, DWORD* dwSize)
{
    TRACE(_T("DataReceive\n"));

    return;
}
//===========================================================================
// WORD DataSend(BYTE* outbuff, DWORD dwSize)
// 函数：发送数据外部接口
// 参数： [in] outbuff:发送数据buf;dwSize:发送数据大小
// 返回值：void
// 功能描述：交互过程中的数据传递。
// Author：wuhaoyong
// Date：20140708
//===========================================================================
WORD CMonitorDlg::DataSend(BYTE* outbuff, DWORD dwSize)
{
    TRACE(_T("DataTrans\n"));
    return theApp.commLayer.TransData(outbuff,dwSize);
}

/**************************************************************************/
/***  specifications;              ***/
/***  NAME   = OnCommReceive;       ***/
/***  FUNCTION  = 接收手机端数据的主处理函数 ***/
/***  DATE   = 2014/07/11;           ***/
/***  AUTHOR  = wuhaoyong;              ***/
/***  INPUT   = ;               ***/
/***  OUTPUT  = ;               ***/
/***  END OF SPECIFICATIONS;            ***/
/**************************************************************************/
LRESULT CMonitorDlg::OnCommReceive(WPARAM wParam, LPARAM lParam)
{
    TRACE(_T("Communication Receive!\n"));
	TRACE0("RX = ");
	TRACE(_T(" %02X\n"),wParam);
    for(WORD cont = 0; cont < wParam ; cont++)
    {
        TRACE(_T(" %02X"),theApp.commLayer.m_ReceiveBuff[cont]);
    }
    TRACE0("\n");
	writeDataFile(&theApp.commLayer.m_ReceiveBuff[0],wParam);
    return 0;
}


void CMonitorDlg::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialogEx::OnShowWindow(bShow, nStatus);

	// TODO: 在此处添加消息处理程序代码
	
}

void CMonitorDlg::startWork()
{
	//open and write file
	//执行导出操作
	CString sGetFileName;
	CString strTime;
	//获取系统时间
	CTime tm;
	tm=CTime::GetCurrentTime();
	strTime=tm.Format(_T("(%Y-%m-%d-%H-%M-%S)"));
	CString fileName = _T("export ")+strTime;
	CFileDialog dlg (FALSE, _T("dmor"), fileName, OFN_HIDEREADONLY | OFN_EXPLORER | OFN_OVERWRITEPROMPT, NULL);
	if (dlg.DoModal () == IDOK)
	{
		sGetFileName = dlg.GetPathName ();
		openDataFile(sGetFileName);
	}
		
	sendConnectCmd();
}

void CMonitorDlg::stopWork()
{
	//close file
	closeDataFile(sGetFileName);
}

void CMonitorDlg::sendConnectCmd()
{
	SEND_COMMAND_LIST* pCurrent= new SEND_COMMAND_LIST;
	pCurrent->cmd = _T("connect");
	pCurrent->buf = theApp.sendCmd;
	pCurrent->size = 0;
	senCommandList.AddTail(pCurrent);
	pCurrent = NULL;
}

void CMonitorDlg::openDataFile(CString strFile)  
{   
#ifdef FEATURE_C_LANGUAGE
	if(fp != NULL)
	{
		fclose(fp);  
	}
	fp = _wfopen(strFile, _T("w")); 
#else
	if (!fAddressTransOut.Open (strFile, CFile::modeCreate | CFile::modeWrite, &eFileException))
	{
		MessageBox (_T("Create file fail"), _T("Error"), MB_OK + MB_ICONERROR);
		bFileOk = false;
	}
	bFileOk = true;
#endif
}  

void CMonitorDlg::writeDataFile(BYTE* tmp, WPARAM wParam)  
{     
#ifdef FEATURE_C_LANGUAGE
    if(fp != NULL)
	{ 
		fseek(fp,0L,2);
		for(unsigned int i=0; i < wParam; ++i)
		{
			unsigned int nSize = fwrite( &tmp[i], 1, 1, fp);  
		}
    }   
#else 
	if(bFileOk)
	{
		fAddressTransOut.SeekToEnd ();
		fAddressTransOut.Write (tmp, wParam);
	}
#endif
}  
void CMonitorDlg::closeDataFile(CString strFile)  
{    
#ifdef FEATURE_C_LANGUAGE
	if(fp != NULL)
	{
		fclose(fp);   
	}
#else
	if(bFileOk)
	{
		TRY
		{
			fAddressTransOut.Close ();
		}
		CATCH (CFileException, eFileException)
		{
			TRACE0 ("File close Failed!\n");
		}
		END_CATCH
		bFileOk = false;
	}
#endif
}  
void CMonitorDlg::OnToolbarConnect()
{
	// TODO: 在此添加命令处理程序代码
	startWork();
}


void CMonitorDlg::OnToolbarDisconnect()
{
	// TODO: 在此添加命令处理程序代码
	stopWork();
}

void CMonitorDlg::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	theApp.commLayer.m_SerialPort.ClosePort();//关闭串口
	bRunning = false;
	Sleep(500);
	initCmdList();
	CDialogEx::OnClose();
}

void CMonitorDlg::initCmdList()
{
	SEND_COMMAND_LIST* pCurrent=NULL;
	while(senCommandList.IsEmpty()==false)
    {
        pCurrent=senCommandList.RemoveHead();
        delete pCurrent;
        pCurrent=NULL;
    }
}
void CMonitorDlg::initWndList()
{
	CDialog* pCurrent=NULL;
	while(wndList.IsEmpty()==false)
    {
        pCurrent=wndList.RemoveHead();
        delete pCurrent;
        pCurrent=NULL;
    }
}
void CMonitorDlg::OnMenuNew()
{
	// TODO: 在此添加命令处理程序代码
}


void CMonitorDlg::OnMenuOpen()
{
	// TODO: 在此添加命令处理程序代码
	
}

void CMonitorDlg::OnMenuWork()
{
	// TODO: 在此添加命令处理程序代码
		CJobDlg m_jDlg;
		m_jDlg.m_Path=theApp.JobPath;
		m_jDlg.m_Title=_T("力擎作业管理");
		m_jDlg.m_TreeTitle=_T("作业列表");
		m_jDlg.DoModal();
}
void CMonitorDlg::OnMenuInstrument()
{
	// TODO: 在此添加命令处理程序代码
		CJobDlg m_jDlg;
		m_jDlg.m_Path=theApp.ToolPath;
		m_jDlg.m_Title=_T("力擎仪器管理");
		m_jDlg.m_TreeTitle=_T("仪器列表");
		m_jDlg.DoModal();
}


bool CMonitorDlg::ShowActiveDlg(CWnd* activeWnd,int id)
{
	if(activeWnd == theApp.activeWnd)
	{
		return false;
	}
	if(theApp.activeWnd != NULL)
	{
		theApp.activeWnd->ShowWindow(SW_HIDE);
	}
	theApp.activeWnd = activeWnd;
	
	theApp.activeWnd->ShowWindow(SW_SHOW);
	theApp.activeWnd->SetActiveWindow();
	theApp.activeWnd->SetFocus();
	return true;
}

void CMonitorDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	// TODO: 在此处添加消息处理程序代码
	CRect rect,rectMain;
	GetClientRect(&rect);
	rectMain.left = rect.left + 3;
	rectMain.right = rect.right - 3;
	rectMain.top = rect.top + 30;
	rectMain.bottom = rect.bottom - 30;
	if(wndList.GetCount()>0)
	{
		POSITION pos = wndList.GetHeadPosition();
		while(pos)
		{
			CDialog * pCurrent = wndList.GetNext(pos);
			if(pCurrent != NULL)
			{
				pCurrent->SetWindowPos(NULL,rectMain.left,rectMain.top,rectMain.Width(),rectMain.Height(),0);
				//TRACE(_T("Child Dialog OnSize\n"));
			}
		}
	}
}
void CMonitorDlg::OnGetMinMaxInfo(MINMAXINFO* lpMMI)
{
	// TODO: Add your message handler code here and/or call default
	lpMMI-> ptMinTrackSize.x = 800 ; //宽 
	lpMMI-> ptMinTrackSize.y = 600 ; //高 
	CDialogEx::OnGetMinMaxInfo(lpMMI);
}



