// JobDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "DMonitor.h"
#include "JobDlg.h"
#include "afxdialogex.h"


// CJobDlg 对话框

IMPLEMENT_DYNAMIC(CJobDlg, CDialog)

CJobDlg::CJobDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CJobDlg::IDD, pParent)
{

}

CJobDlg::~CJobDlg()
{
}

void CJobDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BUTTON_NEWJOB, btnNewJob);
	DDX_Control(pDX, IDC_BUTTON_CLOSE, btnClose);
}


BEGIN_MESSAGE_MAP(CJobDlg, CDialog)
	ON_BN_CLICKED(IDOK, &CJobDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CJobDlg::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_BUTTON_NEWJOB, &CJobDlg::OnBnClickedButtonNewjob)
	ON_BN_CLICKED(IDC_BUTTON_CLOSE, &CJobDlg::OnBnClickedButtonClose)
	ON_WM_SIZE()
END_MESSAGE_MAP()


// CJobDlg 消息处理程序
BOOL CJobDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	SetWindowText(m_Title); 
	CRect rectMain;
	GetClientRect(rectMain);
	if(btnClose.m_hWnd != NULL)
	{
		btnClose.SetWindowPos(NULL,rectMain.right-105 ,rectMain.bottom -50 ,100,40 ,0);
	}
	if(btnNewJob.m_hWnd != NULL)
	{
		btnNewJob.SetWindowPos(NULL,rectMain.right-225 ,rectMain.bottom -50 ,100,40 ,0);
	}
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CJobDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	CDialog::OnOK();
}


void CJobDlg::OnBnClickedCancel()
{
	// TODO: 在此添加控件通知处理程序代码
	CDialog::OnCancel();
}


BOOL CJobDlg::PreTranslateMessage(MSG* pMsg)
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
			default:
				break;
			}
		}
		break;
	default:
		break;
	}
	return CDialog::PreTranslateMessage(pMsg);
}





void CJobDlg::OnBnClickedButtonNewjob()
{
	// TODO: 在此添加控件通知处理程序代码
}


void CJobDlg::OnBnClickedButtonClose()
{
	// TODO: 在此添加控件通知处理程序代码
}


void CJobDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	// TODO: 在此处添加消息处理程序代码
	CRect rectMain;
	GetClientRect(rectMain);
	if(btnClose.m_hWnd != NULL)
	{
		btnClose.SetWindowPos(NULL,rectMain.right-105 ,rectMain.bottom -50 ,100,40 ,0);
	}
	if(btnNewJob.m_hWnd != NULL)
	{
		btnNewJob.SetWindowPos(NULL,rectMain.right-225 ,rectMain.bottom -50 ,100,40 ,0);
	}
}
