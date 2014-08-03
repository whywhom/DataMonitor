// JobDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "DataMonitor.h"
#include "JobDlg.h"
#include "afxdialogex.h"
#include "JobCopyDlg.h"

// CJobDlg 对话框

IMPLEMENT_DYNAMIC(CJobDlg, CDialog)

CJobDlg::CJobDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CJobDlg::IDD, pParent)
{
	receiveFlag = 0;
}

CJobDlg::~CJobDlg()
{
}

void CJobDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_ZCW_JOB_TREE, m_treeCtrl);
	DDX_Control(pDX, IDC_ZCW_JOB_TREE, m_treeCtrl);
}


BEGIN_MESSAGE_MAP(CJobDlg, CDialog)
	ON_COMMAND(ID_JOB_NEW, &CJobDlg::OnJobNew)
	ON_COMMAND(ID_JOB_OPEN, &CJobDlg::OnJobOpen)
	ON_COMMAND(ID_JOB_DEL, &CJobDlg::OnJobDel)
	ON_COMMAND(ID_JOB_COPY, &CJobDlg::OnJobCopy)
	ON_WM_SHOWWINDOW()
END_MESSAGE_MAP()


// CJobDlg 消息处理程序


void CJobDlg::OnJobNew()
{
	// TODO: 在此添加命令处理程序代码
	CFileDialog dlg(FALSE,_T(""),_T(""),OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,_T("tools files(*.tools)|*.tools|tool files(*.mdb)|*.mdb|All files(*.*)|*.*|"));//构造保存文件对话框
	dlg.m_ofn.lpstrInitialDir=m_Path;
    if(dlg.DoModal()==IDOK)//显示保存文件对话框
	{
		CJobEditDlg m_jeDlg;
		m_jeDlg.m_jobName=dlg.GetPathName();//获取文件路径名称
		m_jeDlg.m_Open=FALSE;
		if(m_jeDlg.DoModal()==IDOK){
		m_treeCtrl.ModifyStyle(0,TVS_HASBUTTONS   |   TVS_LINESATROOT   |   TVS_HASLINES);	
		m_treeCtrl.DisplayTree (m_Path,TRUE);
		}
	}
}


void CJobDlg::OnJobOpen()
{
	// TODO: 在此添加命令处理程序代码
		CJobEditDlg m_jeDlg;
		HTREEITEM sel_htem=m_treeCtrl.GetSelectedItem(); 
		 CString strPath=m_treeCtrl.GetFullPath(sel_htem);//获取文件路径名称
		if(!m_treeCtrl.FindSubDir(strPath)){//不能编辑非空子目录
		m_jeDlg.m_jobName=strPath;//获取文件路径名称
		m_jeDlg.m_Open=TRUE;
		m_jeDlg.DoModal();
		}
}


BOOL CJobDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	SetWindowText(m_Title); 	
	GetDlgItem(IDC_ZCW_JOB_TREE_TITLE)->SetWindowText(m_TreeTitle);
	m_treeCtrl.ModifyStyle(0,TVS_HASBUTTONS   |   TVS_LINESATROOT   |   TVS_HASLINES);	
	m_treeCtrl.DisplayTree (m_Path,TRUE);


	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


void CJobDlg::OnJobDel()
{
	// TODO: 在此添加命令处理程序代码
	HTREEITEM sel_htem=m_treeCtrl.GetSelectedItem(); 
    CString strPath=m_treeCtrl.GetFullPath(sel_htem);//获取文件路径名称
	if(!m_treeCtrl.FindSubDir(strPath)){//不能删除非空子目录
		if(MessageBox(_T("您确定要删除作业")+strPath+_T("吗?"),_T("警告"), MB_OKCANCEL )==IDOK)
		{
		  DeleteFile(strPath);
		  m_treeCtrl.DeleteItem(sel_htem);//删除节点
		} 
	}
}


void CJobDlg::OnJobCopy()
{
	// TODO: 在此添加命令处理程序代码
	CJobCopyDlg m_jcDlg;
	HTREEITEM sel_htem=m_treeCtrl.GetSelectedItem(); //当前节点
	HTREEITEM par_htem=m_treeCtrl.GetParentItem(sel_htem); //父节点
	CString strPath=m_treeCtrl.GetFullPath(sel_htem);	
	if(!m_treeCtrl.FindSubDir(strPath)){//不能复制目录
    CString stroldName=m_treeCtrl.GetItemText(sel_htem);
	m_jcDlg.m_oldName=stroldName.Left(stroldName.Find(_T(".")));//截取文件名
	if(m_jcDlg.DoModal()==IDOK){
		CString strNewPath=m_treeCtrl.GetFullPath(par_htem)+_T("\\")+m_jcDlg.m_newName+_T(".tools");//新作业路径
		if(GetFileAttributes(strNewPath)==-1L){
		CopyFile(strPath,strNewPath,TRUE);
		m_treeCtrl.AddItem(par_htem,strNewPath);//添加节点
		}else{
			MessageBox(strNewPath+_T("已存在"),MB_OK);
		}
	}
	}
}

void CJobDlg::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialog::OnShowWindow(bShow, nStatus);

	// TODO: 在此处添加消息处理程序代码
	if(receiveFlag == 1)
	{
		OnJobNew();
	}
}
