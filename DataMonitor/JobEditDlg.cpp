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
	ON_BN_CLICKED(IDC_ADD, &CJobEditDlg::OnBnClickedAdd)
	ON_BN_CLICKED(IDC_DELETE, &CJobEditDlg::OnBnClickedDelete)
END_MESSAGE_MAP()


// CJobEditDlg 消息处理程序

BOOL CJobEditDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	if(m_Open==TRUE){//如果是打开作业操作，则用数据库内容初始化窗口
		CableInit();
	}else{
		JobInit();		
	}

	DisplayTree();
	CRect rect;  
	GetDlgItem(IDC_STATIC_RECT)-> GetWindowRect(&rect);  
	ScreenToClient(&rect); 
	//默认显示仪器信息面版
	m_Cable.Create(IDD_JOBEDIT_CABLE,this);  
	m_Cable.MoveWindow(rect);  
	m_Cable.ShowWindow(SW_SHOW); 

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CJobEditDlg::DisplayTree(){
	m_jeTree.DeleteAllItems();
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

	//一层子节点
	 CDaoDatabase m_DataBase;
	 try{
				m_DataBase.Open(m_jobName);			   	  			   
				CDaoRecordset rs(&m_DataBase);
				 COleVariant OleVariant ;
				rs.Open(dbOpenDynaset, L"SELECT * FROM TOOL" );		
				while(!rs.IsEOF()){
					rs.GetFieldValue (0,OleVariant);
					m_jeTree.InsertItem(OleVariant.bstrVal,0,1,root1,TVI_LAST);
					rs.MoveNext();
				}
				rs.Close();              
				//关闭数据库
				if (m_DataBase.IsOpen())
				m_DataBase.Close();
	 } catch(CDaoException *e){		
			MessageBox(e->m_pErrorInfo->m_strDescription);
			e->Delete();
     }
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
			GetDlgItem(IDC_ADD)->ShowWindow(FALSE);
			GetDlgItem(IDC_DELETE)->ShowWindow(FALSE);
			m_editTable=0;
		 break;
	 case 1:		  
			m_Tool.Create(IDD_JOBEDIT_TOOL, this);  
			m_Tool.MoveWindow(rect);  
			m_Tool.ShowWindow( SW_SHOW ); 	
			GetDlgItem(IDC_ADD)->ShowWindow(TRUE);
			GetDlgItem(IDC_DELETE)->ShowWindow(TRUE);
			m_editTable=1;
		break;
	 case 2:		  		
		    GetDlgItem(IDC_ADD)->ShowWindow(TRUE);
			GetDlgItem(IDC_DELETE)->ShowWindow(TRUE);
			m_editTable=2;		
		break;
	 case 3:
			GetDlgItem(IDC_ADD)->ShowWindow(TRUE);
			GetDlgItem(IDC_DELETE)->ShowWindow(TRUE);
			m_editTable=3;
		break;
	 case 4:		  			
			GetDlgItem(IDC_ADD)->ShowWindow(FALSE);
			GetDlgItem(IDC_DELETE)->ShowWindow(FALSE);
			m_editTable=4;
		break;
	 case 5:		  			
			GetDlgItem(IDC_ADD)->ShowWindow(FALSE);
			GetDlgItem(IDC_DELETE)->ShowWindow(FALSE);
			m_editTable=5;
		break;
	 }
	*pResult = 0;
}

void CJobEditDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码	
	 if(m_Open==TRUE){//如果作业已经存在则修改
		CableUpdate();
	   }else{//否则创建新作业
		CableAdd();         
	 }
	CDialog::OnOK();
}

void CJobEditDlg::CableInit(){
	//根据数据库初始化窗口控件内容
	 CDaoDatabase m_DataBase;
	 COleVariant OleVariant ;
		try{
			m_DataBase.Open(m_jobName);				
				CDaoRecordset rs(&m_DataBase);
				rs.Open(dbOpenDynaset, L"SELECT * FROM CABLE" );
					rs.MoveFirst () ;				
					rs.GetFieldValue (2,OleVariant);
					m_Cable.m_Wire1=OleVariant.bstrVal;
					rs.GetFieldValue (3,OleVariant);
					m_Cable.m_Panel1=OleVariant.bstrVal;

					rs.MoveNext();
					rs.GetFieldValue (2,OleVariant);
					m_Cable.m_Wire2=OleVariant.bstrVal;
					rs.GetFieldValue (3,OleVariant);
					m_Cable.m_Panel2=OleVariant.bstrVal;

					rs.MoveNext();
					rs.GetFieldValue (2,OleVariant);
					m_Cable.m_Wire3=OleVariant.bstrVal;
					rs.GetFieldValue (3,OleVariant);
					m_Cable.m_Panel3=OleVariant.bstrVal;

					rs.MoveNext();
					rs.GetFieldValue (2,OleVariant);
					m_Cable.m_Wire4=OleVariant.bstrVal;
					rs.GetFieldValue (3,OleVariant);
					m_Cable.m_Panel4=OleVariant.bstrVal;

					rs.MoveNext();
					rs.GetFieldValue (2,OleVariant);
					m_Cable.m_Wire5=OleVariant.bstrVal;
					rs.GetFieldValue (3,OleVariant);
					m_Cable.m_Panel5=OleVariant.bstrVal;

					rs.MoveNext();
					rs.GetFieldValue (2,OleVariant);
					m_Cable.m_Wire6=OleVariant.bstrVal;
					rs.GetFieldValue (3,OleVariant);
					m_Cable.m_Panel6=OleVariant.bstrVal;

					rs.MoveNext();
					rs.GetFieldValue (2,OleVariant);
					m_Cable.m_Wire7=OleVariant.bstrVal;
					rs.GetFieldValue (3,OleVariant);
					m_Cable.m_Panel7=OleVariant.bstrVal;

					rs.MoveNext();
					rs.GetFieldValue (2,OleVariant);
					m_Cable.m_Wire8=OleVariant.bstrVal;
					rs.GetFieldValue (3,OleVariant);
					m_Cable.m_Panel8=OleVariant.bstrVal;

					rs.MoveNext();
					rs.GetFieldValue (2,OleVariant);
					m_Cable.m_Wire9=OleVariant.bstrVal;
					rs.GetFieldValue (3,OleVariant);
					m_Cable.m_Panel9=OleVariant.bstrVal;
					
					rs.MoveNext();
					rs.GetFieldValue (2,OleVariant);
					m_Cable.m_Wire10=OleVariant.bstrVal;
					rs.GetFieldValue (3,OleVariant);
					m_Cable.m_Panel10=OleVariant.bstrVal;
				rs.Close();
				//关闭数据库
				if (m_DataBase.IsOpen())
					m_DataBase.Close();
            }
            catch(CDaoException *e){
			MessageBox(e->m_pErrorInfo->m_strDescription);
			e->Delete();
            }
}

void CJobEditDlg::CableAdd()
{
	 CDaoDatabase m_DataBase;
	 try{
				m_DataBase.Open(m_jobName);
			   //新建数据库记录
			    CDaoTableDef td(&m_DataBase);        //构造表对象
			   
				CDaoRecordset rs(&m_DataBase);
				rs.Open(dbOpenDynaset, L"SELECT * FROM CABLE" );

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
	 } catch(CDaoException *e){
			MessageBox(e->m_pErrorInfo->m_strDescription);
			e->Delete();
            }
}

void CJobEditDlg::CableUpdate(){
	    CDaoDatabase m_DataBase;
		try{
			m_DataBase.Open(m_jobName);
				//修改数据库记录
			CDaoRecordset rs(&m_DataBase);				
				rs.Open(dbOpenDynaset, L"SELECT * FROM CABLE" );					
					rs.MoveFirst () ;
					rs.Edit();
					rs.SetFieldValue(2,COleVariant(m_Cable.m_Wire1));	
					rs.SetFieldValue(3,COleVariant(m_Cable.m_Panel1));
					rs.Update();

					rs.MoveNext();
					rs.Edit();
					rs.SetFieldValue(2,COleVariant(m_Cable.m_Wire2));	
					rs.SetFieldValue(3,COleVariant(m_Cable.m_Panel2));
					rs.Update();

					rs.MoveNext();
					rs.Edit();
					rs.SetFieldValue(2,COleVariant(m_Cable.m_Wire3));	
					rs.SetFieldValue(3,COleVariant(m_Cable.m_Panel3));
					rs.Update();

					rs.MoveNext();
					rs.Edit();
					rs.SetFieldValue(2,COleVariant(m_Cable.m_Wire4));	
					rs.SetFieldValue(3,COleVariant(m_Cable.m_Panel4));
					rs.Update();

					rs.MoveNext();
					rs.Edit();
					rs.SetFieldValue(2,COleVariant(m_Cable.m_Wire5));	
					rs.SetFieldValue(3,COleVariant(m_Cable.m_Panel5));
					rs.Update();

					rs.MoveNext();
					rs.Edit();
					rs.SetFieldValue(2,COleVariant(m_Cable.m_Wire6));	
					rs.SetFieldValue(3,COleVariant(m_Cable.m_Panel6));
					rs.Update();

					rs.MoveNext();
					rs.Edit();
					rs.SetFieldValue(2,COleVariant(m_Cable.m_Wire7));	
					rs.SetFieldValue(3,COleVariant(m_Cable.m_Panel7));
					rs.Update();

					rs.MoveNext();
					rs.Edit();
					rs.SetFieldValue(2,COleVariant(m_Cable.m_Wire8));	
					rs.SetFieldValue(3,COleVariant(m_Cable.m_Panel8));
					rs.Update();

					rs.MoveNext();
					rs.Edit();
					rs.SetFieldValue(2,COleVariant(m_Cable.m_Wire9));	
					rs.SetFieldValue(3,COleVariant(m_Cable.m_Panel9));
					rs.Update();
					
					rs.MoveNext();
					rs.Edit();
					rs.SetFieldValue(2,COleVariant(m_Cable.m_Wire10));	
					rs.SetFieldValue(3,COleVariant(m_Cable.m_Panel10));
					rs.Update();
				rs.Close();
				//关闭数据库
				if (m_DataBase.IsOpen())
					m_DataBase.Close();
            }
            catch(CDaoException *e){
			MessageBox(e->m_pErrorInfo->m_strDescription);
			e->Delete();
            }
}

void CJobEditDlg::ToolAdd()
{
	 CDaoDatabase m_DataBase;
	 try{
				m_DataBase.Open(m_jobName);
			   //新建数据库记录			  			   
				CDaoRecordset rs(&m_DataBase);				
				rs.Open(dbOpenDynaset,_T("SELECT * FROM TOOL where LABEL='")+m_Tool.m_Label+_T("'"));
				if(rs.IsBOF()){
			    rs.AddNew();
				rs.SetFieldValue(0,COleVariant(m_Tool.m_Label));
				rs.SetFieldValue(1,COleVariant(m_Tool.m_SN));	
				rs.SetFieldValue(2,COleVariant(m_Tool.m_Speed));
				rs.SetFieldValue(3,COleVariant(m_Tool.m_Type));
				rs.SetFieldValue(4,COleVariant(m_Tool.m_Length));	
				rs.SetFieldValue(5,COleVariant(m_Tool.m_Weight));
				rs.SetFieldValue(6,COleVariant(m_Tool.m_OuteRdiam));
				rs.Update();
				}else{
				MessageBox(_T("仪器")+m_Tool.m_Label+_T("已经存在"),MB_OK);
				}
				rs.Close();			
				//关闭数据库
				if (m_DataBase.IsOpen())
				m_DataBase.Close();				

	 } catch(CDaoException *e){
			MessageBox(e->m_pErrorInfo->m_strDescription);
			e->Delete();
     }
}

void CJobEditDlg::OnBnClickedAdd()
{
	// TODO: 在此添加控件通知处理程序代码
	switch(m_editTable){
	case 1:
		//MessageBox(_T("仪器信息"),MB_OK);
		if(m_Tool.m_Label.Trim().IsEmpty()){
		MessageBox(_T("名称不能为空"),MB_OK);
		}else{
		ToolAdd();		
		DisplayTree();	
		}
		break;
	case 2:
		MessageBox(_T("添加曲线信息"),MB_OK);
		break;
	case 3:
		MessageBox(_T("添加原始信号"),MB_OK);
		break;
	case 4:
		MessageBox(_T("添加控制信息"),MB_OK);
		break;
	case 5:
		MessageBox(_T("添加家电参数"),MB_OK);
		break;
	}

}

void CJobEditDlg::JobInit(){
	CDaoDatabase m_DataBase;	
	try{
			m_DataBase.Create(m_jobName);//如果新建作业则创建数据库
				CDaoTableDef td_C(&m_DataBase);        //构造表对象
				CDaoTableDef td_T(&m_DataBase);        //构造表对象
				//创建Cable表
				td_C.Create(_T("CABLE"));
				CDaoFieldInfo Cf1;
				Cf1.m_strName = _T("LABEL");
				Cf1.m_nType = dbText;
				Cf1.m_lSize = 50;
				Cf1.m_lAttributes = dbVariableField;//dbVariableField为可变长度字段，仅限文本， dbFixedField字段长度是固定的(数字字段的默认值）
				Cf1.m_bRequired = FALSE;
				Cf1.m_bAllowZeroLength = TRUE;				
				td_C.CreateField(Cf1);

				CDaoFieldInfo Cf2;
				Cf2.m_strName = _T("LONGNAME");
				Cf2.m_nType = dbText;
				Cf2.m_lSize = 50;
				Cf2.m_lAttributes = dbVariableField;//dbVariableField为可变长度字段，仅限文本， dbFixedField字段长度是固定的(数字字段的默认值）
				Cf2.m_bRequired = FALSE;
				Cf2.m_bAllowZeroLength = TRUE;				
				td_C.CreateField(Cf2);

				CDaoFieldInfo Cf3;
				Cf3.m_strName = _T("WIRE");
				Cf3.m_nType = dbText;
				Cf3.m_lSize = 50;
				Cf3.m_lAttributes = dbVariableField;//dbVariableField为可变长度字段，仅限文本， dbFixedField字段长度是固定的(数字字段的默认值）
				Cf3.m_bRequired = FALSE;
				Cf3.m_bAllowZeroLength = TRUE;				
				td_C.CreateField(Cf3);

				CDaoFieldInfo Cf4;
				Cf4.m_strName = _T("PANEL");
				Cf4.m_nType = dbText;
				Cf4.m_lSize = 50;
				Cf4.m_lAttributes = dbVariableField;//dbVariableField为可变长度字段，仅限文本， dbFixedField字段长度是固定的(数字字段的默认值）
				Cf4.m_bRequired = FALSE;
				Cf4.m_bAllowZeroLength = TRUE;				
				td_C.CreateField(Cf4);

				CDaoFieldInfo Cf5;
				Cf5.m_strName = _T("REMARK");
				Cf5.m_nType = dbText;
				Cf5.m_lSize = 50;
				Cf5.m_lAttributes = dbVariableField;//dbVariableField为可变长度字段，仅限文本， dbFixedField字段长度是固定的(数字字段的默认值）
				Cf5.m_bRequired = FALSE;
				Cf5.m_bAllowZeroLength = TRUE;			
				td_C.CreateField(Cf5);
			    td_C.Append();
				td_C.Close();

				//创建TOOL表
				td_T.Create(_T("TOOL"));
				CDaoFieldInfo Tf1;
				Tf1.m_strName = _T("LABEL");
				Tf1.m_nType = dbText;
				Tf1.m_lSize = 50;
				Tf1.m_lAttributes = dbVariableField;//dbVariableField为可变长度字段，仅限文本， dbFixedField字段长度是固定的(数字字段的默认值）
				Tf1.m_bRequired = FALSE;
				Tf1.m_bAllowZeroLength = TRUE;							
				td_T.CreateField(Tf1);
			
				CDaoFieldInfo Tf2;
				Tf2.m_strName = _T("TYPE");
				Tf2.m_nType = dbText;
				Tf2.m_lSize = 50;
				Tf2.m_lAttributes = dbVariableField;//dbVariableField为可变长度字段，仅限文本， dbFixedField字段长度是固定的(数字字段的默认值）
				Tf2.m_bRequired = FALSE;
				Tf2.m_bAllowZeroLength = TRUE;			
				td_T.CreateField(Tf2);

				CDaoFieldInfo Tf3;
				Tf3.m_strName = _T("SN");
				Tf3.m_nType = dbText;
				Tf3.m_lSize = 50;
				Tf3.m_lAttributes = dbVariableField;//dbVariableField为可变长度字段，仅限文本， dbFixedField字段长度是固定的(数字字段的默认值）
				Tf3.m_bRequired = FALSE;
				Tf3.m_bAllowZeroLength = TRUE;						
				td_T.CreateField(Tf3);				
				
				td_T.CreateField(_T("LENGTH"),dbSingle,0L);
				td_T.CreateField(_T("Weight"),dbSingle,0L);
			    td_T.CreateField(_T("OUTERDIAMATER"),dbSingle,0L);			

				CDaoFieldInfo Tf7;
				Tf7.m_strName = _T("SPEED");
				Tf7.m_nType = dbText;
				Tf7.m_lSize = 50;
				Tf7.m_lAttributes = dbVariableField;//dbVariableField为可变长度字段，仅限文本， dbFixedField字段长度是固定的(数字字段的默认值）
				Tf7.m_bRequired = FALSE;
				Tf7.m_bAllowZeroLength = TRUE;							
				td_T.CreateField(Tf7);
			    td_T.Append();
				td_T.Close();

		}catch(CDaoException *e){
			MessageBox(e->m_pErrorInfo->m_strDescription);
			e->Delete();
        }
}

void CJobEditDlg::OnBnClickedDelete()
{
	// TODO: 在此添加控件通知处理程序代码
	switch(m_editTable){
	case 1:
		//MessageBox(_T("仪器信息"),MB_OK);
		MessageBox(_T("删除仪器信息"),MB_OK);
		break;
	case 2:
		MessageBox(_T("删除曲线信息"),MB_OK);
		break;
	case 3:
		MessageBox(_T("删除原始信号"),MB_OK);
		break;
	case 4:
		MessageBox(_T("删除控制信息"),MB_OK);
		break;
	case 5:
		MessageBox(_T("删除家电参数"),MB_OK);
		break;
	}
}
