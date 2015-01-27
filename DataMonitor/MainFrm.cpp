
// MainFrm.cpp : CMainFrame ���ʵ��
//

#include "stdafx.h"
#include "DataMonitor.h"
#include<iostream>
#include "MainFrm.h"
#include "DataMonitorView.h"
#include "PanelView.h"
#include "ScaleView.h"
#include <cctype>
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
	ON_MESSAGE( WM_USER_RECEIVEDATA, OnCommReceive)//���ն˿���Ϣ
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
	ON_COMMAND(ID_FILE_OPEN, &CMainFrame::OnFileOpen)
	ON_UPDATE_COMMAND_UI(ID_MENU_MEASUREDOWN, &CMainFrame::OnUpdateMenuMeasuredown)
	ON_UPDATE_COMMAND_UI(ID_MENU_MEASUREUP, &CMainFrame::OnUpdateMenuMeasureup)
	ON_UPDATE_COMMAND_UI(ID_TOOLBAR_MEASUREDOWN, &CMainFrame::OnUpdateToolbarMeasuredown)
	ON_COMMAND(ID_TOOLBAR_MEASUREDOWN, &CMainFrame::OnToolbarMeasuredown)
	ON_COMMAND(ID_TOOLBAR_MEASUREUP, &CMainFrame::OnToolbarMeasureup)
	ON_UPDATE_COMMAND_UI(ID_TOOLBAR_MEASUREUP, &CMainFrame::OnUpdateToolbarMeasureup)
	ON_COMMAND(ID_TOOLBAR_STOP, &CMainFrame::OnToolbarStop)
	ON_UPDATE_COMMAND_UI(ID_TOOLBAR_STOP, &CMainFrame::OnUpdateToolbarStop)
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // ״̬��ָʾ��
	ID_INDICATOR_INFO,
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};

// CMainFrame ����/����

CMainFrame::CMainFrame()
{
	// TODO: �ڴ���ӳ�Ա��ʼ������
	bConnect = false;
	parameterFlag = 0;

	CPetroData* pCurrent = NULL;
	while(theApp.petroList.IsEmpty()==false)
	{
		pCurrent=theApp.petroList.RemoveHead();
		delete pCurrent;
		pCurrent=NULL;
	}
	pData = NULL;
	pPanelView = NULL; 
	pScaleView = NULL; 
	pDataMonitorView = NULL; 
	fileLimit = 1024*1024*4;
	theApp.processType == NO_PROCESSING;
}

CMainFrame::~CMainFrame()
{
	if(pData != NULL)
	{
		delete[] pData;
	}
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP | CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		!m_wndToolBar.LoadToolBar(IDR_MAINFRAME))
	{
		TRACE0("δ�ܴ���������\n");
		return -1;      // δ�ܴ���
	}

	if (!m_wndStatusBar.Create(this))
	{
		TRACE0("δ�ܴ���״̬��\n");
		return -1;      // δ�ܴ���
	}
	m_wndStatusBar.SetIndicators(indicators, sizeof(indicators)/sizeof(UINT));
	m_wndStatusBar.SetPaneInfo(1,ID_INDICATOR_INFO,SBPS_STRETCH,40);
	// TODO: �������Ҫ��ͣ������������ɾ��������
	//m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	//EnableDocking(CBRS_ALIGN_ANY);
	//DockControlBar(&m_wndToolBar);


	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: �ڴ˴�ͨ���޸�
	//  CREATESTRUCT cs ���޸Ĵ��������ʽ

	cs.style = WS_OVERLAPPED | WS_CAPTION /*| FWS_ADDTOTITLE*/
		 | WS_THICKFRAME | WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_MAXIMIZE | WS_SYSMENU;

	return TRUE;
}

// CMainFrame ���

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


// CMainFrame ��Ϣ�������


BOOL CMainFrame::OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext)
{
	// TODO: �ڴ����ר�ô����/����û���
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

CPanelView* CMainFrame::GetPanelView()
{
    return (CPanelView*) m_wndSplitter.GetPane(0,0);
}

CDataMonitorView* CMainFrame::GetDataMonitorView()
{
    return (CDataMonitorView*) m_wndSplitterSub.GetPane(0,0);
}

CScaleView* CMainFrame::GetScaleView()
{
    return (CScaleView*) m_wndSplitterSub.GetPane(1,0);
}

void CMainFrame::OnSize(UINT nType, int cx, int cy)
{
	CFrameWnd::OnSize(nType, cx, cy);
	
	// TODO: �ڴ˴������Ϣ����������
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
	lpMMI-> ptMinTrackSize.x = 800 ; //�� 
	lpMMI-> ptMinTrackSize.y = 600 ; //�� 
	CFrameWnd::OnGetMinMaxInfo(lpMMI);
}

void CMainFrame::OnToolbarConnect()
{
	// TODO: �ڴ���������������
	bConnect = true;
	theApp.commLayer.SetConnectType(TYPE_NONE);
	theApp.commLayer.fatherHwnd = (AfxGetMainWnd()->GetSafeHwnd());//��ȡHWND����ֵ��ͨ�Ų������Ϣ����
	bRunning = true;
	
	fp = NULL;
	StartThread();//�����߳�
	theApp.processType = REALTIME_PROCESSING;
	pDataMonitorView = GetDataMonitorView();
	pPanelView = GetPanelView();
	pScaleView = GetScaleView();
	if(pDataMonitorView)
	{
		pDataMonitorView->KillDrawTimer();
		pDataMonitorView->StartTimer();
	}
	startWork();
}


void CMainFrame::OnToolbarDisconnect()
{
	// TODO: �ڴ���������������
	bConnect = false;
	stopWork();
	pDataMonitorView = GetDataMonitorView();
	if(pDataMonitorView)
	{
		pDataMonitorView->StopTimer();
	}
}

void CMainFrame::OnUpdateToolbarConnect(CCmdUI *pCmdUI)
{
	// TODO: �ڴ������������û����洦��������
	pCmdUI->SetCheck(bConnect);
}


void CMainFrame::OnUpdateToolbarDisconnect(CCmdUI *pCmdUI)
{
	// TODO: �ڴ������������û����洦��������
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
#ifdef _DEBUG
    for(WORD cont = 0; cont < wParam ; cont++)
    {
        TRACE(_T(" %02X"),theApp.commLayer.m_ReceiveBuff[cont]);
    }
	TRACE0("\n");
#endif
	CString str;
	totalReceiveByte += wParam;
	if(totalReceiveByte>fileLimit)
	{
		fileNum++;
		closeDataFile(sGetFileName);
		sGetFilePreName = sGetFileName;
		int pos = sGetFilePreName.ReverseFind(_T('.'));
		int length = sGetFilePreName.GetLength();
		CString strPre = sGetFilePreName.Left(pos);
		CString strSub = sGetFilePreName.Right(length - pos);
		CString strAdd;
		strAdd.Format(_T("_%d"),fileNum);
		sGetFileName = strPre + strAdd + strSub;
		openDataFile(sGetFileName);
	}
	if(totalReceiveByte < 1024)
	{
		str.Format(_T("�ѽ�������: %d B"),totalReceiveByte);
	}
	else if(totalReceiveByte < 1024*1024)
	{
		str.Format(_T("�ѽ�������:%2f KB"),(float)totalReceiveByte/1024);
	}
	else
	{
		str.Format(_T("�ѽ�������:%2f MB"),(float)totalReceiveByte/(1024*1024));
	}
	m_wndStatusBar.SetPaneText(m_wndStatusBar.CommandToIndex(ID_INDICATOR_INFO),str);

	writeDataFile(&theApp.commLayer.m_ReceiveBuff[0],wParam);
	ParseData(&theApp.commLayer.m_ReceiveBuff[0],wParam);
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
	//ִ�е�������
	//CString sGetFileName;
	CString strTime;
	//��ȡϵͳʱ��
	CTime tm;
	tm=CTime::GetCurrentTime();
	strTime=tm.Format(_T("(%Y-%m-%d-%H-%M-%S)"));
	fileName = theApp.DataPath + _T("export ")+strTime;
	CFileDialog dlg (FALSE, _T("dmor"), fileName, OFN_HIDEREADONLY | OFN_EXPLORER | OFN_OVERWRITEPROMPT, NULL);
	if (dlg.DoModal () == IDOK)
	{
		sGetFileName = dlg.GetPathName ();
		openDataFile(sGetFileName);
	}
	totalReceiveByte = 0;
	fileNum = 0;
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

void CMainFrame::ParseData(BYTE* tmp, WPARAM wParam) 
{
	theApp.nStartCoordinate = 0;
	//theApp.bStart = true;
	std::string str,strTitle,strData;
	CPetroData* pPData = NULL;
	BYTE buf0 = 0;
	for(int i = 0; i< wParam; i++)
	{
		if(i == wParam-1)
		{
			if(pPData != NULL)
			{
				theApp.petroList.AddTail(pPData);
			}
		}
		if(tmp[i] == '$')//��ʼ��ʾ
		{
			str.clear();
			TRACE(_T("Find Char '$' \r\n"));
		}
		else if(tmp[i] == ',' || tmp[i] == '*')//���ݽ���
		{
			if(str == "DEPT" || str == "TEMP" || str == "RM" 
				|| str == "GM" || str == "MAGX" || str == "CCL" )
			{
				strTitle = str;
				if(str == "DEPT")
				{
					TRACE(_T("Find Char 'DEPT' \r\n"));
					if(pPData != NULL)
					{
						theApp.petroList.AddTail(pPData);
					}
					pPData = new CPetroData();
				}
				str.clear();
			}
			else
			{
				int result = CheckString(str);
				//if(result == 0)//����
				if(str != "LQRD" )
				{
					double num;
					num=atof(str.c_str());
					if(pPData != NULL)
					{
						if(strTitle == "DEPT")
						{
							TRACE(_T("strTitle == 'DEPT' \r\n"));
							pPData->dept.iData = num;
							pPData->dept.strData = str.c_str();
							pPData->dept.bAssign = true;
							pPData->dept.strTag = strTitle.c_str();
							pPData->dept.strUnit = _T("m");
						}
						else if(strTitle == "TEMP")
						{
							TRACE(_T("strTitle == 'TEMP' \r\n"));
							pPData->temp.iData = num;
							pPData->temp.strData = str.c_str();
							pPData->temp.bAssign = true;
							pPData->temp.strTag = strTitle.c_str();
							pPData->temp.strUnit = _T("��");
						}
						else if(strTitle == "RM")
						{
							TRACE(_T("strTitle == 'RM' \r\n"));
							pPData->rm.iData = num;
							pPData->rm.strData = str.c_str();
							pPData->rm.bAssign = true;
							pPData->rm.strTag = strTitle.c_str();
							pPData->rm.strUnit = _T("��");
						}
						else if(strTitle == "GM")
						{
							TRACE(_T("strTitle == 'GM' \r\n"));
							pPData->gr.iData = num;
							pPData->gr.strData = str.c_str();
							pPData->gr.bAssign = true;
							pPData->gr.strTag = strTitle.c_str();
							pPData->gr.strUnit = _T("��");
						}
						else if(strTitle == "MAGX")
						{
							TRACE(_T("strTitle == 'MAGX' \r\n"));
							if(pPData->mag[0].iData == 0)
							{
								pPData->mag[0].iData = num;
								pPData->mag[0].strData = str.c_str();
								pPData->mag[0].bAssign = true;
								pPData->mag[0].strTag = strTitle.c_str();
								pPData->mag[0].strUnit = _T("��");
							}
							else if(pPData->mag[1].iData == 0)
							{
								pPData->mag[1].iData = num;
								pPData->mag[1].strData = str.c_str();
								pPData->mag[1].bAssign = true;
								pPData->mag[1].strTag = strTitle.c_str();
								pPData->mag[1].strUnit = _T("��");
							}
							else if(pPData->mag[2].iData == 0)
							{
								pPData->mag[2].iData = num;
								pPData->mag[2].strData = str.c_str();
								pPData->mag[2].bAssign = true;
								pPData->mag[2].strTag = strTitle.c_str();
								pPData->mag[2].strUnit = _T("��");
							}
							else
							{
							}
						}
						else if(strTitle == "CCL")
						{
							TRACE(_T("strTitle == 'CCL' \r\n"));
							pPData->ccl.iData = num;
							pPData->ccl.strData = str.c_str();
							pPData->ccl.bAssign = true;
							pPData->ccl.strTag = strTitle.c_str();
							pPData->ccl.strUnit = _T("��");
						}
					}
				}
				str.clear();
				strTitle.empty();
			}
			if( tmp[i] == '*')
			{
				i+=2;
			}
		}
		else if(tmp[i] == 0x0D || tmp[i] == 0x0A)
		{
			continue;
		}
		else
		{
			str += tmp[i];
		}
	}
}

int CMainFrame::CheckString( std::string str )
{
	bool bIsDigit = true;
	bool bIsPoint = false;
	int nCount = str.length(); // ����ַ�����
	for ( int i = 0; i < nCount; i ++ )
	{
		if ( 0 == isdigit( str[i] ) ) // �������־��ñ�־λ
		{
			if(str[i] == '.')//��С����
			{
				bIsPoint = true;
			}
			else
			{
				bIsDigit = false;
				break;// �˳�
			}
		}
	}
	if(!bIsDigit)
	{
		return 2;//������
	}
	else if(bIsPoint)
	{
		return 1;//��С����
	}
	else
	{
		return 0;//����
	}
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
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	if (MessageBox(theApp.GetResString(IDS_EXIT_APP_QUESTION),_T("��ʾ"),MB_ICONQUESTION|MB_OKCANCEL)==IDOK)
	{
		theApp.commLayer.m_SerialPort.ClosePort();//�رմ���
		bRunning = false;
		Sleep(500);
		initCmdList();
		CFrameWnd::OnClose();
	}
}


void CMainFrame::OnMenuWork()
{
	// TODO: �ڴ���������������
	parameterFlag=0;
	myOnMenuWork(parameterFlag);
}

void CMainFrame::myOnMenuWork(int myparameterFlag){	
	CJobDlg m_jDlg;
	m_jDlg.m_Path=theApp.JobPath;
	m_jDlg.receiveFlag = myparameterFlag;
	m_jDlg.m_Title=_T("������ҵ����");
	m_jDlg.m_TreeTitle=_T("��ҵ�б�");
	m_jDlg.DoModal();
}


void CMainFrame::OnMenuInstrument()
{
	// TODO: �ڴ���������������
	CJobDlg m_jDlg;
	m_jDlg.m_Path=theApp.ToolPath;
	m_jDlg.m_Title=_T("������������");
	m_jDlg.m_TreeTitle=_T("�����б�");
	m_jDlg.DoModal();
}






void CMainFrame::OnUpdateIndicatorInfo(CCmdUI *pCmdUI)
{
	// TODO: �ڴ������������û����洦��������
}


void CMainFrame::OnMenuNewjob()
{
	// TODO: �ڴ���������������
	parameterFlag = 1;
	myOnMenuWork(parameterFlag);
}


void CMainFrame::OnMenuJobload()
{
	// TODO: �ڴ���������������
	parameterFlag = 0;
	myOnMenuWork(parameterFlag);
}


void CMainFrame::OnUpdateMenuMeasureup(CCmdUI *pCmdUI)
{
	// TODO: �ڴ������������û����洦��������
	pDataMonitorView = GetDataMonitorView();
	if(pDataMonitorView)
	{
		pCmdUI->SetCheck(!pDataMonitorView->GetDirectionDown()); 
	}
}

void CMainFrame::OnMenuMeasureup()
{
	// TODO: �ڴ���������������
	pDataMonitorView = GetDataMonitorView();
	if(pDataMonitorView)
	{
		pDataMonitorView->SetDirectionDown(false);
		CMenu* pSubMenu = NULL;
		CMenu* pMainMenu = AfxGetMainWnd()->GetMenu();
		{
			pSubMenu = pMainMenu->GetSubMenu(3);
			if (pSubMenu && pSubMenu->GetMenuItemID(0) == ID_MENU_MEASUREUP)
			{
				pSubMenu->CheckMenuItem(ID_MENU_MEASUREUP,pDataMonitorView->GetDirectionDown()?MF_UNCHECKED:MF_CHECKED); 
			}
			if (pSubMenu && pSubMenu->GetMenuItemID(1) == ID_MENU_MEASUREDOWN)
			{
				pSubMenu->CheckMenuItem(ID_MENU_MEASUREDOWN,pDataMonitorView->GetDirectionDown()?MF_CHECKED:MF_UNCHECKED); 
			}
		}
	}
}


void CMainFrame::OnUpdateMenuMeasuredown(CCmdUI *pCmdUI)
{
	// TODO: �ڴ������������û����洦��������
	pDataMonitorView = GetDataMonitorView();
	if(pDataMonitorView)
	{
		pCmdUI->SetCheck(pDataMonitorView->GetDirectionDown()); 
	}
}


void CMainFrame::OnMenuMeasuredown()
{
	// TODO: �ڴ���������������
	pDataMonitorView = GetDataMonitorView();
	if(pDataMonitorView)
	{
		pDataMonitorView->SetDirectionDown(true);
		CMenu* pSubMenu = NULL;
		CMenu* pMainMenu = AfxGetMainWnd()->GetMenu();
		{
			pSubMenu = pMainMenu->GetSubMenu(3);
			if (pSubMenu && pSubMenu->GetMenuItemID(0) == ID_MENU_MEASUREUP)
			{
				pSubMenu->CheckMenuItem(ID_MENU_MEASUREUP,pDataMonitorView->GetDirectionDown()?MF_UNCHECKED:MF_CHECKED); 
			}
			if (pSubMenu && pSubMenu->GetMenuItemID(1) == ID_MENU_MEASUREDOWN)
			{
				pSubMenu->CheckMenuItem(ID_MENU_MEASUREDOWN,pDataMonitorView->GetDirectionDown()?MF_CHECKED:MF_UNCHECKED); 
			}
		}
	}
}


void CMainFrame::OnMenuDrawmodel()
{
	// TODO: �ڴ���������������

	SHELLEXECUTEINFO  ShExecInfo = {0};
	CString strFile = theApp.ModuleFilePath + _T("\\PlotEditor.exe");  
	ShExecInfo.cbSize = sizeof(SHELLEXECUTEINFO);
	ShExecInfo.fMask    = NULL;
	ShExecInfo.hwnd      = NULL;
	ShExecInfo.lpVerb    = NULL;
	ShExecInfo.lpFile      = strFile;      // ִ�еĳ�����
	ShExecInfo.lpParameters = NULL;
	ShExecInfo.lpDirectory    = NULL;
	ShExecInfo.nShow          = SW_NORMAL;                // Ĭ�Ϸ�ʽ��ʾ�������
	ShExecInfo.hInstApp = NULL;
	ShellExecuteEx(&ShExecInfo);
	/*
	if(!ShellExecuteEx(&ShExecInfo))
	{
		DWORD dErr = GetLastError();
		if(dErr == ERROR_FILE_NOT_FOUND)
		{
			MessageBox(_T("ָ���ļ�������"),_T("��ʾ"),MB_ICONQUESTION|MB_OK);
		}
		else if(dErr == ERROR_NO_ASSOCIATION)
		{
			MessageBox(_T("û�����ƶ��ļ�����չ��Ӧ��Ӧ�ó���"),_T("��ʾ"),MB_ICONQUESTION|MB_OK);
		}
		else if(dErr == ERROR_ACCESS_DENIED)
		{
			MessageBox(_T("����ָ���ļ����ܾ�"),_T("��ʾ"),MB_ICONQUESTION|MB_OK);
		}
		else if(dErr == ERROR_DLL_NOT_FOUND)
		{
			MessageBox(_T("�޷��ҵ�����Ӧ�ó���������Ŀ��ļ�"),_T("��ʾ"),MB_ICONQUESTION|MB_OK);
		}
		else if(dErr == ERROR_NOT_ENOUGH_MEMORY)
		{
			MessageBox(_T("û���㹻���ڴ���ִ�в���"),_T("��ʾ"),MB_ICONQUESTION|MB_OK);
		}
	}
	*/
}


void CMainFrame::OnMenuDemo()
{
	// TODO: �ڴ���������������
}


void CMainFrame::OnMenuDebugcmd()
{
	// TODO: �ڴ���������������
}


void CMainFrame::OnEditCopy()
{
	// TODO: �ڴ���������������
}


void CMainFrame::OnEditCut()
{
	// TODO: �ڴ���������������
}


void CMainFrame::OnEditPaste()
{
	// TODO: �ڴ���������������
}


void CMainFrame::OnMenuTargetdeepth()
{
	// TODO: �ڴ���������������
	CTargetDepth targetDlg;
	targetDlg.DoModal();
}

#if 1
void CMainFrame::OnFileOpen()
{
	// TODO: �ڴ���������������
	bool bReturn=false;
	theApp.processType = FILEDATA_PROCESSING;
	TCHAR   *pFileBuffer = new TCHAR[MAX_PATH];//�������300���ļ�
	CFileDialog dlg (TRUE, NULL, NULL, OFN_HIDEREADONLY | OFN_EXPLORER , _T("dmor�ļ�(*.dmor)|*.dmor||"),this);
	if (dlg.DoModal () == IDOK)
	{
		CString csFileName = dlg.GetPathName();
		CFile fAddressImport;
		CFileException eFileException;
		if (!fAddressImport.Open (csFileName, CFile::modeRead | CFile::shareDenyWrite, &eFileException))
		{
			return;
		}
		DWORD dwFileLength = fAddressImport.GetLength ();
		pData = new BYTE[dwFileLength];
		memset(pData,0,dwFileLength);
		fAddressImport.SeekToBegin ();
		fAddressImport.Read (pData,dwFileLength);
		theApp.petroList.RemoveAll();
		ParseData(pData,dwFileLength);
		pDataMonitorView = GetDataMonitorView();
		pPanelView = GetPanelView();
		pScaleView = GetScaleView();
		if(pDataMonitorView)
		{
			pDataMonitorView->StopTimer();
			pDataMonitorView->StartTimer();
		}
	}
}

void CMainFrame::OnUpdateToolbarMeasuredown(CCmdUI *pCmdUI)
{
	// TODO: �ڴ������������û����洦��������
	pDataMonitorView = GetDataMonitorView();
	if(pDataMonitorView)
	{
		pCmdUI->SetCheck(pDataMonitorView->GetDirectionDown()); 
	}
}


void CMainFrame::OnToolbarMeasuredown()
{
	// TODO: �ڴ���������������
	pDataMonitorView = GetDataMonitorView();
	if(pDataMonitorView)
	{
		pDataMonitorView->SetDirectionDown(true);
		CMenu* pSubMenu = NULL;
		CMenu* pMainMenu = AfxGetMainWnd()->GetMenu();
		{
			pSubMenu = pMainMenu->GetSubMenu(3);
			if (pSubMenu && pSubMenu->GetMenuItemID(0) == ID_MENU_MEASUREUP)
			{
				pSubMenu->CheckMenuItem(ID_MENU_MEASUREUP,pDataMonitorView->GetDirectionDown()?MF_UNCHECKED:MF_CHECKED); 
			}
			if (pSubMenu && pSubMenu->GetMenuItemID(1) == ID_MENU_MEASUREDOWN)
			{
				pSubMenu->CheckMenuItem(ID_MENU_MEASUREDOWN,pDataMonitorView->GetDirectionDown()?MF_CHECKED:MF_UNCHECKED); 
			}
		}
	}
}


void CMainFrame::OnToolbarMeasureup()
{
	// TODO: �ڴ���������������
	pDataMonitorView = GetDataMonitorView();
	if(pDataMonitorView)
	{
		pDataMonitorView->SetDirectionDown(false);
		CMenu* pSubMenu = NULL;
		CMenu* pMainMenu = AfxGetMainWnd()->GetMenu();
		{
			pSubMenu = pMainMenu->GetSubMenu(3);
			if (pSubMenu && pSubMenu->GetMenuItemID(0) == ID_MENU_MEASUREUP)
			{
				pSubMenu->CheckMenuItem(ID_MENU_MEASUREUP,pDataMonitorView->GetDirectionDown()?MF_UNCHECKED:MF_CHECKED); 
			}
			if (pSubMenu && pSubMenu->GetMenuItemID(1) == ID_MENU_MEASUREDOWN)
			{
				pSubMenu->CheckMenuItem(ID_MENU_MEASUREDOWN,pDataMonitorView->GetDirectionDown()?MF_CHECKED:MF_UNCHECKED); 
			}
		}
	}
}


void CMainFrame::OnUpdateToolbarMeasureup(CCmdUI *pCmdUI)
{
	// TODO: �ڴ������������û����洦��������
	pDataMonitorView = GetDataMonitorView();
	if(pDataMonitorView)
	{
		pCmdUI->SetCheck(!pDataMonitorView->GetDirectionDown()); 
	}
}
#endif

void CMainFrame::OnToolbarStop()
{
	// TODO: �ڴ���������������
	pDataMonitorView = GetDataMonitorView();
	if(pDataMonitorView)
	{
		pDataMonitorView->StopTimer();
	}
}


void CMainFrame::OnUpdateToolbarStop(CCmdUI *pCmdUI)
{
	// TODO: �ڴ������������û����洦��������
	
}
