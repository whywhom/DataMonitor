// JobEditDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "DataMonitor.h"
#include "JobEditDlg.h"
#include "afxdialogex.h"


// CJobEditDlg 对话框

IMPLEMENT_DYNAMIC(CJobEditDlg, CDialog)

CJobEditDlg::CJobEditDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CJobEditDlg::IDD, pParent)
	, m_jobName(_T(""))
	, m_Open(false)
{
	
}

CJobEditDlg::~CJobEditDlg()
{
}

void CJobEditDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_JOBNAME, m_jobName);
	DDX_Control(pDX, IDC_JOBEDIT_TREE, m_jeTree);
}


BEGIN_MESSAGE_MAP(CJobEditDlg, CDialog)
	ON_NOTIFY(TVN_SELCHANGED, IDC_JOBEDIT_TREE, &CJobEditDlg::OnSelchangedJobeditTree)
	ON_BN_CLICKED(IDOK, &CJobEditDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CJobEditDlg 消息处理程序

BOOL CJobEditDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	CDaoDatabase m_DataBase;
	if(m_Open==TRUE){//如果是打开作业操作，则用数据库内容初始化窗口
		try{
			m_DataBase.Open(m_jobName);
			//初始化窗口代码
			//关闭数据库
			if (m_DataBase.IsOpen())
				m_DataBase.Close();
		}
		catch(CDaoException *e){
			MessageBox(e->m_pErrorInfo->m_strDescription);
			e->Delete();
			//return FALSE;
		}
	}

	//树形控件初始化
	m_jeTree.ModifyStyle(0,TVS_HASBUTTONS|TVS_LINESATROOT|TVS_HASLINES);
	HTREEITEM root0=m_jeTree.InsertItem(_T("缆芯提示"),0,1,TVI_ROOT,TVI_LAST);
	m_jeTree.SetItemData(root0,(DWORD)0);
	HTREEITEM root1=m_jeTree.InsertItem(_T("仪器信息"),0,1,TVI_ROOT,TVI_LAST);
	m_jeTree.SetItemData(root1,(DWORD)1);
	HTREEITEM root2=m_jeTree.InsertItem(_T("曲线信息"),0,1,TVI_ROOT,TVI_LAST);
	m_jeTree.SetItemData(root2,(DWORD)2);
	HTREEITEM root3=m_jeTree.InsertItem(_T("原始信号"),0,1,TVI_ROOT,TVI_LAST);
	m_jeTree.SetItemData(root3,(DWORD)3);
	HTREEITEM root4=m_jeTree.InsertItem(_T("控制信息"),0,1,TVI_ROOT,TVI_LAST);
	m_jeTree.SetItemData(root4,(DWORD)4);
	HTREEITEM root5=m_jeTree.InsertItem(_T("加电参数"),0,1,TVI_ROOT,TVI_LAST);
	m_jeTree.SetItemData(root5,(DWORD)5);

	CRect rect;  
	GetDlgItem(IDC_STATIC_RECT)-> GetWindowRect(&rect);  
	ScreenToClient(&rect); 
	//默认显示缆芯提示面版
	m_Cable.Create(IDD_JOBEDIT_CABLE,this);  
	m_Cable.MoveWindow(rect);  
	m_Cable.ShowWindow(SW_SHOW); 

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


void CJobEditDlg::OnSelchangedJobeditTree(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	HTREEITEM hSel=m_jeTree.GetSelectedItem();
	 DWORD ids;
	 ids = m_jeTree.GetItemData(hSel);
	 CRect rect;  
	 GetDlgItem(IDC_STATIC_RECT)-> GetWindowRect(&rect);  
	 ScreenToClient(&rect);  
	 if(m_Cable.GetSafeHwnd()!=NULL)
			m_Cable.DestroyWindow();//如果缆芯提示面版已经打开过，则先清理掉，避免重复打开
	 if(m_Tool.GetSafeHwnd()!=NULL)
			m_Tool.DestroyWindow();//如果信息仪器面版已经打开过，则先清理掉，避免重复打开
	 switch(ids){
	 case 0:		    
			m_Cable.Create(IDD_JOBEDIT_CABLE, this);  
			m_Cable.MoveWindow(rect);  
			m_Cable.ShowWindow( SW_SHOW ); 
		 break;
	 case 1:		  
			m_Tool.Create(IDD_JOBEDIT_TOOL, this);  
			m_Tool.MoveWindow(rect);  
			m_Tool.ShowWindow( SW_SHOW ); 
		break;
	 }
	*pResult = 0;
}


void CJobEditDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	 CDaoDatabase m_DataBase;
	 if(m_Open==TRUE){//如果作业已经存在则修改
			try{
			m_DataBase.Open(m_jobName);
				//修改数据库记录
				//关闭数据库
				if (m_DataBase.IsOpen())
					m_DataBase.Close();
            }
            catch(CDaoException *e){
			MessageBox(e->m_pErrorInfo->m_strDescription);
			e->Delete();
            }
	   }else{//否则创建新作业
		   try{
			    m_DataBase.Create(m_jobName);
			   //新建数据库记录
			    CDaoTableDef td(&m_DataBase);        //构造表对象
			   //创建Cable表
				td.Create(_T("CABLE"));
				td.CreateField(_T("LABEL"),dbText,50,dbVariableField);
				td.CreateField(_T("LONGNAME"),dbText,50,dbVariableField);
				td.CreateField(_T("WIRE"),dbText,50,dbVariableField);
				td.CreateField(_T("PANEL"),dbText,50,dbVariableField);
				td.CreateField(_T("REMARK"),dbText,50,dbVariableField);
			    td.Append();
				CDaoRecordset rs(&m_DataBase);
				rs.Open(&td,1,0);

				rs.AddNew();
				rs.SetFieldValue(0,_T("1#"));
				rs.SetFieldValue(2,COleVariant(m_Cable.m_Wire1));	
				rs.SetFieldValue(3,COleVariant(m_Cable.m_Panel1));
				rs.Update();

				rs.AddNew();
				rs.SetFieldValue(0,_T("2#"));
				rs.SetFieldValue(2,COleVariant(m_Cable.m_Wire2));	
				rs.SetFieldValue(3,COleVariant(m_Cable.m_Panel2));
				rs.Update();

				rs.AddNew();
				rs.SetFieldValue(0,_T("3#"));
				rs.SetFieldValue(2,COleVariant(m_Cable.m_Wire3));	
				rs.SetFieldValue(3,COleVariant(m_Cable.m_Panel3));
				rs.Update();

				rs.AddNew();
				rs.SetFieldValue(0,_T("4#"));
				rs.SetFieldValue(2,COleVariant(m_Cable.m_Wire4));	
				rs.SetFieldValue(3,COleVariant(m_Cable.m_Panel4));
				rs.Update();

				rs.AddNew();
				rs.SetFieldValue(0,_T("5#"));
				rs.SetFieldValue(2,COleVariant(m_Cable.m_Wire5));	
				rs.SetFieldValue(3,COleVariant(m_Cable.m_Panel5));
				rs.Update();

				rs.AddNew();
				rs.SetFieldValue(0,_T("6#"));
				rs.SetFieldValue(2,COleVariant(m_Cable.m_Wire6));	
				rs.SetFieldValue(3,COleVariant(m_Cable.m_Panel6));
				rs.Update();

				rs.AddNew();
				rs.SetFieldValue(0,_T("7#"));
				rs.SetFieldValue(2,COleVariant(m_Cable.m_Wire7));	
				rs.SetFieldValue(3,COleVariant(m_Cable.m_Panel7));
				rs.Update();

				rs.AddNew();
				rs.SetFieldValue(0,_T("CT#"));
				rs.SetFieldValue(2,COleVariant(m_Cable.m_Wire8));	
				rs.SetFieldValue(3,COleVariant(m_Cable.m_Panel8));
				rs.Update();

				rs.AddNew();
				rs.SetFieldValue(0,_T("Ground#"));
				rs.SetFieldValue(2,COleVariant(m_Cable.m_Wire9));	
				rs.SetFieldValue(3,COleVariant(m_Cable.m_Panel9));
				rs.Update();

				rs.AddNew();
				rs.SetFieldValue(0,_T("SWITCHCODE#"));
				rs.SetFieldValue(2,COleVariant(m_Cable.m_Wire10));	
				rs.SetFieldValue(3,COleVariant(m_Cable.m_Panel10));
				rs.Update();

				rs.Close();
                td.Close();
				//关闭数据库
				if (m_DataBase.IsOpen())
					m_DataBase.Close();
            }
            catch(CDaoException *e){
			MessageBox(e->m_pErrorInfo->m_strDescription);
			e->Delete();
            }
	 }

	CDialog::OnOK();
}
