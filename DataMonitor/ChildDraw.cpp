// ChildDraw.cpp : 实现文件
//

#include "stdafx.h"
#include "DataMonitor.h"
#include "ChildDraw.h"
#include "afxdialogex.h"


// CChildDraw 对话框

IMPLEMENT_DYNAMIC(CChildDraw, CDialogEx)

CChildDraw::CChildDraw(CWnd* pParent /*=NULL*/)
	: CDialogEx(CChildDraw::IDD, pParent)
{
	pCanvasView = new CCanvasView;
	
}

CChildDraw::~CChildDraw()
{
}

void CChildDraw::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
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


BEGIN_MESSAGE_MAP(CChildDraw, CDialogEx)
	ON_WM_SHOWWINDOW()
	ON_WM_SIZE()
	ON_BN_CLICKED(IDOK, &CChildDraw::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CChildDraw::OnBnClickedCancel)
END_MESSAGE_MAP()


// CChildDraw 消息处理程序


void CChildDraw::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialogEx::OnShowWindow(bShow, nStatus);

	// TODO: 在此处添加消息处理程序代码
}


BOOL CChildDraw::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	value01.LoadString(IDS_DEEPTH);
	cs01.SetWindowText(value01);
	value02.LoadString(IDS_SPEED);
	cs02.SetWindowText(value02);
	value03.LoadString(IDS_TENSION);
	cs03.SetWindowText(value03);
	value04.LoadString(IDS_STATE);
	cs04.SetWindowText(value04);

	btSetup.SetWindowText(theApp.GetResString(IDS_SETUP));
	listView.InsertColumn( 0, theApp.GetResString(IDS_WHY_CURVE), LVCFMT_LEFT, 50 ); 
    listView.InsertColumn( 1, theApp.GetResString(IDS_WHY_VALUE), LVCFMT_LEFT, 50 ); 
	listView.InsertColumn( 2, theApp.GetResString(IDS_WHY_UNIT), LVCFMT_LEFT, 50 ); 
	listView.SetExtendedStyle( LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES | LVS_EX_HEADERDRAGDROP /*| LVS_EX_CHECKBOXES*/); 
	ASSERT(pCanvasView != NULL);
	if (!((CView*)pCanvasView)->Create(NULL, NULL, WS_CHILD,
		CRect(0, 0, 0, 0), this, NULL, NULL))
	{
		AfxMessageBox(_T("Create view failed"));
	}
	pCanvasView->ShowWindow(SW_SHOW);
	pCanvasView->OnInitialUpdate();
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


void CChildDraw::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	// TODO: 在此处添加消息处理程序代码
	CRect rect,rectLeft,rectList,rectRight;
	GetClientRect(&rect);
	rectLeft.left = rect.left + 3;
	rectLeft.right = rectLeft.left + 150;
	rectLeft.top = rect.top + 3;
	rectLeft.bottom = rectLeft.top + rect.Height()/3;

	rectList.left = rectLeft.left;
	rectList.right = rectLeft.right;
	rectList.top = rectLeft.bottom + 3;
	rectList.bottom = rect.bottom - 3;

	rectRight.left = rectLeft.right + 3;
	rectRight.right = rect.right - 3;
	rectRight.top = rect.top + 3;
	rectRight.bottom = rect.bottom - 3;

	int mWidth1 = 40;
	int mWidth2 = 97;
	int temp = (rectLeft.Height()-3)/5;
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

	if(pCanvasView->m_hWnd != NULL)
	{
		pCanvasView->SetWindowPos(NULL,rectRight.left,rectRight.top,rectRight.Width(),rectRight.Height(),0);
	}
}


void CChildDraw::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	//CDialogEx::OnOK();
}


void CChildDraw::OnBnClickedCancel()
{
	// TODO: 在此添加控件通知处理程序代码
	//CDialogEx::OnCancel();
}


BOOL CChildDraw::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类
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
			case VK_F8:
				return FALSE;
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
