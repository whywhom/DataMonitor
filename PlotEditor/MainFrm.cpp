
// MainFrm.cpp : CMainFrame 类的实现
//

#include "stdafx.h"
#include "PlotEditor.h"

#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	ON_WM_CREATE()
	ON_COMMAND(ID_FILE_NEWPLOT, &CMainFrame::OnFileNewplot)
	ON_COMMAND(ID_FILE_SAVEPLOT, &CMainFrame::OnFileSaveplot)
	ON_WM_CLOSE()
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // 状态行指示器
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};

// CMainFrame 构造/析构

CMainFrame::CMainFrame()
{
	// TODO: 在此添加成员初始化代码
	CCurveInfo* pCurrent = NULL;
	while(theApp.curveList.IsEmpty()==false)
	{
		pCurrent=theApp.curveList.RemoveHead();
		delete pCurrent;
		pCurrent=NULL;
	}
}

CMainFrame::~CMainFrame()
{
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	if (!m_wndStatusBar.Create(this))
	{
		TRACE0("未能创建状态栏\n");
		return -1;      // 未能创建
	}
	m_wndStatusBar.SetIndicators(indicators, sizeof(indicators)/sizeof(UINT));

	return 0;
}

BOOL CMainFrame::OnCreateClient(LPCREATESTRUCT /*lpcs*/,
	CCreateContext* pContext)
{
	if(!m_wndSplitter.CreateStatic(this,2,1))
	{
		return FALSE;
	}
	CRect rect;
	GetClientRect(&rect);
	

	pFrame=(CMainFrame*)AfxGetMainWnd;

	if(!m_wndSplitter.CreateView(0,0,RUNTIME_CLASS(CPlotEditorView),CSize(rect.Width(),rect.Height()-200),pContext))
	{
		return FALSE;
	}
	pPlotEditorView = (CPlotEditorView*)m_wndSplitter.GetPane(0,0); 
	if(!m_wndSplitter.CreateView(1,0,RUNTIME_CLASS(CCoordinateView),CSize(rect.Width(),200),pContext))
	{
		return FALSE;
	}
	pCoodinateView = (CCoordinateView*)m_wndSplitter.GetPane(1,0); 
	return TRUE;
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


void CMainFrame::OnFileNewplot()
{
	// TODO: 在此添加命令处理程序代码
	CCurveSheet PropSht(theApp.GetResString(IDS_CURVE_PROPERTY));
	if(PropSht.DoModal() == IDOK)
	{
	}
}


void CMainFrame::OnFileSaveplot()
{
	// TODO: 在此添加命令处理程序代码
}


void CMainFrame::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	CFrameWnd::OnClose();
}
