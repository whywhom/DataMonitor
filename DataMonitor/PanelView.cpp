// PanelView.cpp : 实现文件
//

#include "stdafx.h"
#include "DataMonitor.h"
#include "PanelView.h"


// CPanelView

IMPLEMENT_DYNCREATE(CPanelView, CFormView)

CPanelView::CPanelView()
	: CFormView(CPanelView::IDD)
{

}

CPanelView::~CPanelView()
{
}

void CPanelView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_01, cs01);
	DDX_Text(pDX, IDC_STATIC_01, value01);
	DDX_Control(pDX, IDC_STATIC_02, cs02);
	DDX_Text(pDX, IDC_STATIC_02, value02);
	DDX_Control(pDX, IDC_STATIC_03, cs03);
	DDX_Text(pDX, IDC_STATIC_03, value03);
	DDX_Control(pDX, IDC_STATIC_04, cs04);
	DDX_Text(pDX, IDC_STATIC_04, value04);
	DDX_Control(pDX, IDC_EDIT_01, Edit01);
	DDX_Text(pDX, IDC_EDIT_01, strEdit01);
	DDX_Control(pDX, IDC_EDIT_02, Edit02);
	DDX_Text(pDX, IDC_EDIT_02, strEdit02);
	DDX_Control(pDX, IDC_EDIT_03, Edit03);
	DDX_Text(pDX, IDC_EDIT_03, strEdit03);
	DDX_Control(pDX, IDC_EDIT_04, Edit04);
	DDX_Text(pDX, IDC_EDIT_04, strEdit04);
	DDX_Control(pDX, IDC_BUTTON_SETUP, btSetup);
	DDX_Control(pDX, IDC_LIST_DETAIL, listView);
}

BEGIN_MESSAGE_MAP(CPanelView, CFormView)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_SHOWWINDOW()
END_MESSAGE_MAP()


// CPanelView 诊断

#ifdef _DEBUG
void CPanelView::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CPanelView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CPanelView 消息处理程序


int CPanelView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFormView::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  在此添加您专用的创建代码
	
	return 0;
}


void CPanelView::OnSize(UINT nType, int cx, int cy)
{
	CFormView::OnSize(nType, cx, cy);

	// TODO: 在此处添加消息处理程序代码
	CRect rect,rectLeft,rectList,rectRight;
	GetClientRect(&rect);
	rectLeft.left = rect.left + 10;
	rectLeft.right = rect.right - 10;
	rectLeft.top = rect.top + 10;
	rectLeft.bottom = rectLeft.top + rect.Height()/3;

	rectList.left = rectLeft.left;
	rectList.right = rectLeft.right;
	rectList.top = rectLeft.bottom + 3;
	rectList.bottom = rect.bottom - 10;

	rectRight.left = rectLeft.right + 3;
	rectRight.right = rect.right - 5;
	rectRight.top = rect.top + 10;
	rectRight.bottom = rect.bottom - 10;

	int mWidth1 = 40;
	int mWidth2 = rectLeft.Width()-10-mWidth1;
	int temp = (rectLeft.Height())/5;
	int mHight = temp*3/4;
	int gap = temp - mHight;
	if(cs01.m_hWnd != NULL)
	{
		cs01.SetWindowPos(NULL,rectLeft.left,rectLeft.top,mWidth1,mHight,0);
	}
	if(Edit01.m_hWnd != NULL)
	{
		Edit01.SetWindowPos(NULL,rectLeft.left+50,rectLeft.top,mWidth2,mHight,0);
	}

	if(cs02.m_hWnd != NULL)
	{
		cs02.SetWindowPos(NULL,rectLeft.left,rectLeft.top + (mHight + gap)*1,mWidth1,mHight,0);
	}
	if(Edit02.m_hWnd != NULL)
	{
		Edit02.SetWindowPos(NULL,rectLeft.left+50,rectLeft.top + (mHight + gap)*1,mWidth2,mHight,0);
	}

	if(cs03.m_hWnd != NULL)
	{
		cs03.SetWindowPos(NULL,rectLeft.left,rectLeft.top + (mHight + gap)*2,mWidth1,mHight,0);
	}
	if(Edit03.m_hWnd != NULL)
	{
		Edit03.SetWindowPos(NULL,rectLeft.left+50,rectLeft.top + (mHight + gap)*2,mWidth2,mHight,0);
	}

	if(cs04.m_hWnd != NULL)
	{
		cs04.SetWindowPos(NULL,rectLeft.left,rectLeft.top + (mHight + gap)*3,mWidth1,mHight,0);
	}
	if(Edit04.m_hWnd != NULL)
	{
		Edit04.SetWindowPos(NULL,rectLeft.left+50,rectLeft.top + (mHight + gap)*3,mWidth2,mHight,0);
	}
	if(btSetup.m_hWnd != NULL)
	{
		btSetup.SetWindowPos(NULL,rectLeft.left+50,rectLeft.top + (mHight + gap)*4,mWidth2,mHight,0);
	}
	
	if(listView.m_hWnd != NULL)
	{
		listView.SetWindowPos(NULL,rectList.left,rectList.top,rectList.Width(),rectList.Height(),0);
	}
}


void CPanelView::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CFormView::OnShowWindow(bShow, nStatus);

	// TODO: 在此处添加消息处理程序代码
	
}


void CPanelView::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();

	// TODO: 在此添加专用代码和/或调用基类
	value01.LoadString(IDS_DEEPTH);
	cs01.SetWindowText(value01);
	value02.LoadString(IDS_SPEED);
	cs02.SetWindowText(value02);
	value03.LoadString(IDS_TENSION);
	cs03.SetWindowText(value03);
	value04.LoadString(IDS_STATE);
	cs04.SetWindowText(value04);

	btSetup.SetWindowText(theApp.GetResString(IDS_SETUP));
	listView.InsertColumn( 0, theApp.GetResString(IDS_WHY_CURVE), LVCFMT_LEFT, 60 ); 
    listView.InsertColumn( 1, theApp.GetResString(IDS_WHY_VALUE), LVCFMT_LEFT, 60 ); 
	listView.InsertColumn( 2, theApp.GetResString(IDS_WHY_UNIT), LVCFMT_LEFT, 60 ); 
	listView.SetExtendedStyle( LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES | LVS_EX_HEADERDRAGDROP ); 
}
