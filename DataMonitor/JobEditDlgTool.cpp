// JobEditDlgTool.cpp : 实现文件
//

#include "stdafx.h"
#include "DataMonitor.h"
#include "JobEditDlgTool.h"
#include "afxdialogex.h"


// CJobEditDlgTool 对话框

IMPLEMENT_DYNAMIC(CJobEditDlgTool, CDialog)

CJobEditDlgTool::CJobEditDlgTool(CWnd* pParent /*=NULL*/)
	: CDialog(CJobEditDlgTool::IDD, pParent)
{

}

CJobEditDlgTool::~CJobEditDlgTool()
{
}

void CJobEditDlgTool::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CJobEditDlgTool, CDialog)
END_MESSAGE_MAP()


// CJobEditDlgTool 消息处理程序
