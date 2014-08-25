
// MainFrm.cpp : CMainFrame 类的实现
//

#include "stdafx.h"
#include "DataMonitor.h"

#include "MainFrm.h"
#include "DataMonitorView.h"
#include "PanelView.h"
#include "ScaleView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

static bool bRunning;
static CTypedPtrList < CPtrList, SEND_COMMAND_LIST * >senCommandList;
// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	ON_WM_CREATE()
	ON_COMMAND(ID_TOOLBAR_CONNECT, &CMainFrame::OnToolbarConnect)
	ON_COMMAND(ID_TOOLBAR_DISCONNECT, &CMainFrame::OnToolbarDisconnect)
	ON_MESSAGE( WM_USER_RECEIVEDATA, OnCommReceive)//接收端口消息
	ON_WM_CLOSE()
	ON_COMMAND(ID_MENU_WORK, &CMainFrame::OnMenuWork)
	ON_COMMAND(ID_MENU_INSTRUMENT, &CMainFrame::OnMenuInstrument)
	ON_UPDATE_COMMAND_UI(ID_TOOLBAR_CONNECT, &CMainFrame::OnUpdateToolbarConnect)
	ON_UPDATE_COMMAND_UI(ID_TOOLBAR_DISCONNECT, &CMainFrame::OnUpdateToolbarDisconnect)
	ON_WM_SIZE()
	ON_WM_GETMINMAXINFO()
	ON_UPDATE_COMMAND_UI(ID_INDICATOR_INFO, &CMainFrame::OnUpdateIndicatorInfo)
	ON_COMMAND(ID_MENU_NEWJOB, &CMainFrame::OnMenuNewjob)
	ON_COMMAND(ID_MENU_JOBLOAD, &CMainFrame::OnMenuJobload)
	ON_COMMAND(ID_MENU_MEASUREUP, &CMainFrame::OnMenuMeasureup)
	ON_COMMAND(ID_MENU_MEASUREDOWN, &CMainFrame::OnMenuMeasuredown)
	ON_COMMAND(ID_MENU_DRAWMODEL, &CMainFrame::OnMenuDrawmodel)
	ON_COMMAND(ID_MENU_DEMO, &CMainFrame::OnMenuDemo)
	ON_COMMAND(ID_MENU_DEBUGCMD, &CMainFrame::OnMenuDebugcmd)
	ON_COMMAND(ID_EDIT_COPY, &CMainFrame::OnEditCopy)
	ON_COMMAND(ID_EDIT_CUT, &CMainFrame::OnEditCut)
	ON_COMMAND(ID_EDIT_PASTE, &CMainFrame::OnEditPaste)
	ON_COMMAND(ID_MENU_TARGETDEEPTH, &CMainFrame::OnMenuTargetdeepth)
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // 状态行指示器
	ID_INDICATOR_INFO,
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};

// CMainFrame 构造/析构

CMainFrame::CMainFrame()
{
	// TODO: 在此添加成员初始化代码
	bConnect = false;
	parameterFlag = 0;
}

CMainFrame::~CMainFrame()
{
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP | CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		!m_wndToolBar.LoadToolBar(IDR_MAINFRAME))
	{
		TRACE0("未能创建工具栏\n");
		return -1;      // 未能创建
	}

	if (!m_wndStatusBar.Create(this))
	{
		TRACE0("未能创建状态栏\n");
		return -1;      // 未能创建
	}
	m_wndStatusBar.SetIndicators(indicators, sizeof(indicators)/sizeof(UINT));
	m_wndStatusBar.SetPaneInfo(1,ID_INDICATOR_INFO,SBPS_STRETCH,40);
	// TODO: 如果不需要可停靠工具栏，则删除这三行
	//m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	//EnableDocking(CBRS_ALIGN_ANY);
	//DockControlBar(&m_wndToolBar);


	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	cs.style = WS_OVERLAPPED | WS_CAPTION /*| FWS_ADDTOTITLE*/
		 | WS_THICKFRAME | WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_MAXIMIZE | WS_SYSMENU;

	return TRUE;
}

// CMainFrame 诊断

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}
#endif //_DEBUG


// CMainFrame 消息处理程序


BOOL CMainFrame::OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext)
{
	// TODO: 在此添加专用代码和/或调用基类
	if(!m_wndSplitter.CreateStatic(this,1,2))
	{
		return FALSE;
	}
	 
	CRect rect;
	GetClientRect(&rect);
	CPanelView* pPanelView; 
	CScaleView* pScaleView; 
	CDataMonitorView* pDataMonitorView; 

	pFrame=(CMainFrame*)AfxGetMainWnd();  

	if(!m_wndSplitter.CreateView(0,0,RUNTIME_CLASS(CPanelView),CSize(200,rect.Height()),pContext))
	{
		return FALSE;
	}
	pPanelView = (CPanelView*)m_wndSplitter.GetPane(0,0); 
	if(!m_wndSplitterSub.CreateStatic(&m_wndSplitter,2,1,WS_CHILD|WS_VISIBLE,m_wndSplitter.IdFromRowCol(0,1)))
	{
		return FALSE;
	}

	if(!m_wndSplitterSub.CreateView(0,0,RUNTIME_CLASS(CDataMonitorView), CSize(rect.Width()-200,::GetSystemMetrics(SM_CYSCREEN)-200),pContext))
	{
		return FALSE;
	}
	pDataMonitorView = (CDataMonitorView*)m_wndSplitterSub.GetPane(0,0); 
	if(!m_wndSplitterSub.CreateView(1,0,RUNTIME_CLASS(CScaleView), CSize(rect.Width()-200,200),pContext))
	{
		return FALSE;
	}
	pScaleView = (CScaleView*)m_wndSplitterSub.GetPane(1,0); 
	return TRUE;
	//return CFrameWnd::OnCreateClient(lpcs, pContext);
}


void CMainFrame::OnSize(UINT nType, int cx, int cy)
{
	CFrameWnd::OnSize(nType, cx, cy);
	
	// TODO: 在此处添加消息处理程序代码
	CRect rect;
	GetClientRect(&rect);
	if(m_wndSplitter && m_wndSplitterSub)  // m_bSplitterCreated set in OnCreateClient
    {
		m_wndStatusBar.SetPaneInfo(1,ID_INDICATOR_INFO,SBPS_NORMAL,200);
		if(rect.Width()>=200 && rect.Height()>=200)
		{
			m_wndSplitter.SetColumnInfo(0, 200, 10);
			m_wndSplitter.SetColumnInfo(1, rect.Width()-200, 10);
			m_wndSplitter.RecalcLayout();

			m_wndSplitterSub.SetRowInfo(0, rect.Height()-200, 10);
			m_wndSplitterSub.SetRowInfo(1, 200, 10);
			m_wndSplitterSub.RecalcLayout();
		}
    }
}

void CMainFrame::OnGetMinMaxInfo(MINMAXINFO* lpMMI)
{
	// TODO: Add your message handler code here and/or call default
	lpMMI-> ptMinTrackSize.x = 800 ; //宽 
	lpMMI-> ptMinTrackSize.y = 600 ; //高 
	CFrameWnd::OnGetMinMaxInfo(lpMMI);
}

void CMainFrame::OnToolbarConnect()
{
	// TODO: 在此添加命令处理程序代码
	bConnect = true;
	theApp.commLayer.SetConnectType(TYPE_NONE);
	theApp.commLayer.fatherHwnd = (AfxGetMainWnd()->GetSafeHwnd());//获取HWND，赋值给通信层进行消息传递
	bRunning = true;
	fp = NULL;
	StartThread();//启动线程
	startWork();
}


void CMainFrame::OnToolbarDisconnect()
{
	// TODO: 在此添加命令处理程序代码
	bConnect = false;
	stopWork();
}

void CMainFrame::OnUpdateToolbarConnect(CCmdUI *pCmdUI)
{
	// TODO: 在此添加命令更新用户界面处理程序代码
	pCmdUI->SetCheck(bConnect);
}


void CMainFrame::OnUpdateToolbarDisconnect(CCmdUI *pCmdUI)
{
	// TODO: 在此添加命令更新用户界面处理程序代码
	pCmdUI->SetCheck(!bConnect);
}
bool CMainFrame::StartThread()
{
    if (!(m_Thread = AfxBeginThread(MainThread, this)))
    {
    	m_Thread=NULL;
        return false;
    }
	TRACE(_T("Thread started\n"));
    return true;
}

LRESULT CMainFrame::OnCommReceive(WPARAM wParam, LPARAM lParam)
{
    TRACE(_T("Communication Receive!\n"));
	TRACE0("CMainFrame RX = ");
	TRACE(_T(" %02X\n"),wParam);
    for(WORD cont = 0; cont < wParam ; cont++)
    {
        TRACE(_T(" %02X"),theApp.commLayer.m_ReceiveBuff[cont]);
    }
    TRACE0("\n");
	CString str;
	totalReceiveByte += wParam;
	str.Format(_T("已接收:%d字节"),totalReceiveByte);
	m_wndStatusBar.SetPaneText(m_wndStatusBar.CommandToIndex(ID_INDICATOR_INFO),str);
	writeDataFile(&theApp.commLayer.m_ReceiveBuff[0],wParam);
    return 0;
}

UINT CMainFrame::MainThread(LPVOID pParam)
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

void CMainFrame::initCmdList()
{
	SEND_COMMAND_LIST* pCurrent=NULL;
	while(senCommandList.IsEmpty()==false)
    {
        pCurrent=senCommandList.RemoveHead();
        delete pCurrent;
        pCurrent=NULL;
    }
}

void CMainFrame::startWork()
{
	//open and write file
	//执行导出操作
	CString sGetFileName;
	CString strTime;
	//获取系统时间
	CTime tm;
	tm=CTime::GetCurrentTime();
	strTime=tm.Format(_T("(%Y-%m-%d-%H-%M-%S)"));
	CString fileName = theApp.DataPath + _T("export ")+strTime;
	CFileDialog dlg (FALSE, _T("dmor"), fileName, OFN_HIDEREADONLY | OFN_EXPLORER | OFN_OVERWRITEPROMPT, NULL);
	if (dlg.DoModal () == IDOK)
	{
		sGetFileName = dlg.GetPathName ();
		openDataFile(sGetFileName);
	}
	totalReceiveByte = 0;
	sendConnectCmd();
}

void CMainFrame::stopWork()
{
	//close file
	closeDataFile(sGetFileName);
}

void CMainFrame::sendConnectCmd()
{
	SEND_COMMAND_LIST* pCurrent= new SEND_COMMAND_LIST;
	pCurrent->cmd = _T("connect");
	pCurrent->buf = theApp.sendCmd;
	pCurrent->size = 0;
	senCommandList.AddTail(pCurrent);
	pCurrent = NULL;
}

void CMainFrame::openDataFile(CString strFile)  
{   
	if(fp != NULL)
	{
		fclose(fp);  
	}
	fp = _wfopen(strFile, _T("w")); 
}  

void CMainFrame::writeDataFile(BYTE* tmp, WPARAM wParam)  
{     
    if(fp != NULL)
	{ 
		fseek(fp,0L,2);
		//for(unsigned int i=0; i < wParam; ++i)
		{
			unsigned int nSize = fwrite( &tmp[0], wParam, 1, fp);  
		}
    }   
}  
void CMainFrame::closeDataFile(CString strFile)  
{    
	if(fp != NULL)
	{
		fclose(fp);   
	}
}  


void CMainFrame::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if (MessageBox(theApp.GetResString(IDS_EXIT_APP_QUESTION),_T("提示"),MB_ICONQUESTION|MB_OKCANCEL)==IDOK)
	{
		theApp.commLayer.m_SerialPort.ClosePort();//关闭串口
		bRunning = false;
		Sleep(500);
		initCmdList();
		CFrameWnd::OnClose();
	}
}


void CMainFrame::OnMenuWork()
{
	// TODO: 在此添加命令处理程序代码
	parameterFlag=0;
	myOnMenuWork(parameterFlag);
}

void CMainFrame::myOnMenuWork(int myparameterFlag){	
	CJobDlg m_jDlg;
	m_jDlg.m_Path=theApp.JobPath;
	m_jDlg.receiveFlag = myparameterFlag;
	m_jDlg.m_Title=_T("力擎作业管理");
	m_jDlg.m_TreeTitle=_T("作业列表");
	m_jDlg.DoModal();
}


void CMainFrame::OnMenuInstrument()
{
	// TODO: 在此添加命令处理程序代码
	CJobDlg m_jDlg;
	m_jDlg.m_Path=theApp.ToolPath;
	m_jDlg.m_Title=_T("力擎仪器管理");
	m_jDlg.m_TreeTitle=_T("仪器列表");
	m_jDlg.DoModal();
}






void CMainFrame::OnUpdateIndicatorInfo(CCmdUI *pCmdUI)
{
	// TODO: 在此添加命令更新用户界面处理程序代码
}


void CMainFrame::OnMenuNewjob()
{
	// TODO: 在此添加命令处理程序代码
	parameterFlag = 1;
	myOnMenuWork(parameterFlag);
}


void CMainFrame::OnMenuJobload()
{
	// TODO: 在此添加命令处理程序代码
	parameterFlag = 0;
	myOnMenuWork(parameterFlag);
}


void CMainFrame::OnMenuMeasureup()
{
	// TODO: 在此添加命令处理程序代码
}


void CMainFrame::OnMenuMeasuredown()
{
	// TODO: 在此添加命令处理程序代码
}


void CMainFrame::OnMenuDrawmodel()
{
	// TODO: 在此添加命令处理程序代码

	SHELLEXECUTEINFO  ShExecInfo = {0};
	CString strFile = theApp.ModuleFilePath + _T("\\PlotEditor.exe");  
	ShExecInfo.cbSize = sizeof(SHELLEXECUTEINFO);
	ShExecInfo.fMask    = NULL;
	ShExecInfo.hwnd      = NULL;
	ShExecInfo.lpVerb    = NULL;
	ShExecInfo.lpFile      = strFile;      // 执行的程序名
	ShExecInfo.lpParameters = NULL;
	ShExecInfo.lpDirectory    = NULL;
	ShExecInfo.nShow          = SW_NORMAL;                // 默认方式显示这个程序
	ShExecInfo.hInstApp = NULL;
	ShellExecuteEx(&ShExecInfo);
	/*
	if(!ShellExecuteEx(&ShExecInfo))
	{
		DWORD dErr = GetLastError();
		if(dErr == ERROR_FILE_NOT_FOUND)
		{
			MessageBox(_T("指定文件不存在"),_T("提示"),MB_ICONQUESTION|MB_OK);
		}
		else if(dErr == ERROR_NO_ASSOCIATION)
		{
			MessageBox(_T("没有与制定文件名扩展对应的应用程序"),_T("提示"),MB_ICONQUESTION|MB_OK);
		}
		else if(dErr == ERROR_ACCESS_DENIED)
		{
			MessageBox(_T("访问指定文件被拒绝"),_T("提示"),MB_ICONQUESTION|MB_OK);
		}
		else if(dErr == ERROR_DLL_NOT_FOUND)
		{
			MessageBox(_T("无法找到运行应用程序所必须的库文件"),_T("提示"),MB_ICONQUESTION|MB_OK);
		}
		else if(dErr == ERROR_NOT_ENOUGH_MEMORY)
		{
			MessageBox(_T("没有足够的内存来执行操作"),_T("提示"),MB_ICONQUESTION|MB_OK);
		}
	}
	*/
}


void CMainFrame::OnMenuDemo()
{
	// TODO: 在此添加命令处理程序代码
}


void CMainFrame::OnMenuDebugcmd()
{
	// TODO: 在此添加命令处理程序代码
}


void CMainFrame::OnEditCopy()
{
	// TODO: 在此添加命令处理程序代码
}


void CMainFrame::OnEditCut()
{
	// TODO: 在此添加命令处理程序代码
}


void CMainFrame::OnEditPaste()
{
	// TODO: 在此添加命令处理程序代码
}


void CMainFrame::OnMenuTargetdeepth()
{
	// TODO: 在此添加命令处理程序代码
	CTargetDepth targetDlg;
	targetDlg.DoModal();
}
