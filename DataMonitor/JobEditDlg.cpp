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
	//关闭数据库
	if (m_DataBase.IsOpen())
	m_DataBase.Close();
}

void CJobEditDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_ZCW_JOBEDIT_JOBNAME, m_jobName);
	DDX_Control(pDX, IDC_ZCW_JOBEDIT_TREE, m_jeTree);
}

BEGIN_MESSAGE_MAP(CJobEditDlg, CDialog)
	ON_NOTIFY(TVN_SELCHANGED, IDC_ZCW_JOBEDIT_TREE, &CJobEditDlg::OnSelchangedJobeditTree)
	ON_BN_CLICKED(IDOK, &CJobEditDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_ZCW_JOBEDIT_ADD, &CJobEditDlg::OnBnClickedAdd)
	ON_BN_CLICKED(IDC_ZCW_JOBEDIT_DELETE, &CJobEditDlg::OnBnClickedDelete)
	ON_BN_CLICKED(IDC_ZCW_JOBEDIT_UPDATE, &CJobEditDlg::OnBnClickedUpdate)
END_MESSAGE_MAP()


// CJobEditDlg 消息处理程序

BOOL CJobEditDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	// TODO:  在此添加额外的初始化
	if(m_Open==TRUE){//如果是打开作业操作，则用数据库内容初始化窗口
		m_DataBase.Open(m_jobName);	
		ToolInit();
	}else{
		JobInit();		
	}

	DisplayTree();
	CRect rect;  
	GetDlgItem(IDC_ZCW_STATIC_RECT)-> GetWindowRect(&rect);  
	ScreenToClient(&rect); 
	//默认显示缆芯提示面版	
	m_Tool.Create(IDD_JOBEDIT_TOOL,this);  
	m_Tool.MoveWindow(rect);  
	m_Tool.ShowWindow(SW_SHOW); 

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CJobEditDlg::DisplayTree(){
	m_jeTree.DeleteAllItems();
	//树形控件初始化
	m_jeTree.ModifyStyle(0,TVS_HASBUTTONS|TVS_LINESATROOT|TVS_HASLINES);
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
	 try{						   	  			   
				CDaoRecordset rs(&m_DataBase);
				COleVariant OleVariant ;
				//填充Tool的子节点
				rs.Open(dbOpenDynaset, L"SELECT * FROM TOOL" );		
				while(!rs.IsEOF()){
					rs.GetFieldValue (0,OleVariant);
					m_jeTree.InsertItem(OleVariant.bstrVal,0,1,root1,TVI_LAST);
					rs.MoveNext();
				}
				rs.Close();     
				//填充Curve的子节点
				rs.Open(dbOpenDynaset, L"SELECT * FROM Curve" );		
				while(!rs.IsEOF()){
					rs.GetFieldValue (0,OleVariant);
					m_jeTree.InsertItem(OleVariant.bstrVal,0,1,root2,TVI_LAST);
					rs.MoveNext();
				}
				rs.Close();     

				//填充Origin的子节点
				rs.Open(dbOpenDynaset, L"SELECT * FROM Origin" );		
				while(!rs.IsEOF()){
					rs.GetFieldValue (0,OleVariant);
					m_jeTree.InsertItem(OleVariant.bstrVal,0,1,root3,TVI_LAST);
					rs.MoveNext();
				}
				rs.Close();  
				
	 } catch(CDaoException *e){		
			MessageBox(e->m_pErrorInfo->m_strDescription);
			e->Delete();
     }
}

void CJobEditDlg::OnSelchangedJobeditTree(NMHDR *pNMHDR, LRESULT *pResult)
{	
	
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	 HTREEITEM hSel=m_jeTree.GetSelectedItem();//当前节点
	 HTREEITEM hPar=m_jeTree.GetParentItem(hSel);//父节点
	 DWORD idp;//父节点ID
	 if(hPar){
	 idp = m_jeTree.GetItemData(hPar);//二级节点
	 }else{
	 idp=m_jeTree.GetItemData(hSel);//一级节点
	 }
	 //CString par;	
     //par.Format(_T("%d"), idp);此处代码用于显示选中的父节点号
	 //MessageBox(par,MB_OK);
	 CRect rect;  
	 GetDlgItem(IDC_ZCW_STATIC_RECT)-> GetWindowRect(&rect);  
	 ScreenToClient(&rect);  	
	 if(m_Tool.GetSafeHwnd()!=NULL)
			m_Tool.DestroyWindow();//如果信息仪器面版已经打开过，则先清理掉，避免重复打开
	 if(m_Curve.GetSafeHwnd()!=NULL)
			m_Curve.DestroyWindow();//如果曲线信息面版已经打开过，则先清理掉，避免重复打开
	 if(m_Origin.GetSafeHwnd()!=NULL)
			m_Origin.DestroyWindow();//如果原始信号面版已经打开过，则先清理掉，避免重复打开	
	 switch(idp){
	 case 1:
		    if(hPar){			
			ToolInit(m_jeTree.GetItemText(hSel));			
			}else{
			ToolInit();			
			}			
			m_Tool.Create(IDD_JOBEDIT_TOOL, this);  
			m_Tool.MoveWindow(rect);  
			m_Tool.ShowWindow( SW_SHOW ); 				
			m_editTable=1;		
			GetDlgItem(IDC_ZCW_JOBEDIT_ADD)->ShowWindow(TRUE);
			GetDlgItem(IDC_ZCW_JOBEDIT_DELETE)->ShowWindow(TRUE);
			GetDlgItem(IDC_ZCW_JOBEDIT_UPDATE)->ShowWindow(TRUE);
		break;
	 case 2:
		    if(hPar){			
			CurveInit(m_jeTree.GetItemText(hSel));				
			}else{
			CurveInit();			
			}		
		    m_Curve.Create(IDD_JOBEDIT_CURVE, this);  
			m_Curve.MoveWindow(rect);  			
			m_Curve.ShowWindow( SW_SHOW ); 		   		
			m_editTable=2;	
			GetDlgItem(IDC_ZCW_JOBEDIT_ADD)->ShowWindow(TRUE);
			GetDlgItem(IDC_ZCW_JOBEDIT_DELETE)->ShowWindow(TRUE);
			GetDlgItem(IDC_ZCW_JOBEDIT_UPDATE)->ShowWindow(TRUE);
		break;
	 case 3:
		    if(hPar){			
			OriginInit(m_jeTree.GetItemText(hSel));				
			}else{
			OriginInit();				
			}		
		    m_Origin.Create(IDD_JOBEDIT_ORIGIN, this);
			m_Origin.MoveWindow(rect);  			
			m_Origin.ShowWindow( SW_SHOW ); 			
			m_editTable=3;		
			GetDlgItem(IDC_ZCW_JOBEDIT_ADD)->ShowWindow(TRUE);
			GetDlgItem(IDC_ZCW_JOBEDIT_DELETE)->ShowWindow(TRUE);
			GetDlgItem(IDC_ZCW_JOBEDIT_UPDATE)->ShowWindow(TRUE);
		break;
	 case 4:
			GetDlgItem(IDC_ZCW_JOBEDIT_ADD)->ShowWindow(FALSE);
			GetDlgItem(IDC_ZCW_JOBEDIT_DELETE)->ShowWindow(FALSE);
			GetDlgItem(IDC_ZCW_JOBEDIT_UPDATE)->ShowWindow(FALSE);
			m_editTable=4;
		break;
	 case 5:
			GetDlgItem(IDC_ZCW_JOBEDIT_ADD)->ShowWindow(FALSE);
			GetDlgItem(IDC_ZCW_JOBEDIT_DELETE)->ShowWindow(FALSE);
			GetDlgItem(IDC_ZCW_JOBEDIT_UPDATE)->ShowWindow(FALSE);
			m_editTable=5;
		break;
	 }
	*pResult = 0;
}

void CJobEditDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码	
	 if(m_Open==TRUE){//如果作业已经存在则修改		
	   }else{//否则创建新作业	        
	 }	
	CDialog::OnOK();
}

void CJobEditDlg::ToolInit(CString Label){
	//初始化仪器信息窗口
	 COleVariant OleVariant ;
		try{					
				CDaoRecordset rs(&m_DataBase);
				rs.Open(dbOpenDynaset,_T("SELECT * FROM TOOL where LABEL='")+Label+_T("'"));
					rs.MoveFirst () ;				
					rs.GetFieldValue (0,OleVariant);
					m_Tool.m_Label=OleVariant.bstrVal;

					rs.GetFieldValue (1,OleVariant);
					m_Tool.m_Type=OleVariant.bstrVal;

					rs.GetFieldValue (2,OleVariant);
					m_Tool.m_SN=OleVariant.bstrVal;	

					rs.GetFieldValue (3,OleVariant);
					m_Tool.m_Length=OleVariant.fltVal;	

					rs.GetFieldValue (4,OleVariant);
					m_Tool.m_Weight=OleVariant.fltVal;

					rs.GetFieldValue (5,OleVariant);
					m_Tool.m_OuteRdiam=OleVariant.fltVal;	

					rs.GetFieldValue (6,OleVariant);
					m_Tool.m_Speed=OleVariant.bstrVal;							
				rs.Close();				
            }
            catch(CDaoException *e){
			MessageBox(e->m_pErrorInfo->m_strDescription);
			e->Delete();
            }
}

void CJobEditDlg::CurveInit(CString Label){
	//初始化曲线信息窗口	
	 COleVariant OleVariant ;
		try{				
				CDaoRecordset rs(&m_DataBase);
				rs.Open(dbOpenDynaset,_T("SELECT * FROM Curve where LABEL='")+Label+_T("'"));
					rs.MoveFirst () ;				
					rs.GetFieldValue (0,OleVariant);
					m_Curve.m_Label=OleVariant.bstrVal;

					rs.GetFieldValue (1,OleVariant);
					m_Curve.m_unit=OleVariant.iVal;						

					rs.GetFieldValue (2,OleVariant);
					m_Curve.m_filter=OleVariant.iVal;		

				rs.Close();				
            }
            catch(CDaoException *e){
			MessageBox(e->m_pErrorInfo->m_strDescription);
			e->Delete();
            }
}

void CJobEditDlg::OriginInit(CString Label){
	//初始化原始信号窗口	
	 COleVariant OleVariant ;
		try{						
				CDaoRecordset rs(&m_DataBase);
				rs.Open(dbOpenDynaset,_T("SELECT * FROM Origin where LABEL='")+Label+_T("'"));
					rs.MoveFirst () ;				
					rs.GetFieldValue (0,OleVariant);
					m_Origin.m_Label=OleVariant.bstrVal;

					rs.GetFieldValue (1,OleVariant);
					m_Origin.m_unit=OleVariant.iVal;						

					rs.GetFieldValue (2,OleVariant);
					m_Origin.m_filter=OleVariant.iVal;		

				rs.Close();				
            }
            catch(CDaoException *e){
			MessageBox(e->m_pErrorInfo->m_strDescription);
			e->Delete();
            }
}

void CJobEditDlg::ToolDel(CString Label){
	//删除选中的仪器信息	
	 COleVariant OleVariant ;
		try{					
				CDaoRecordset rs(&m_DataBase);
				rs.Open(dbOpenDynaset,_T("SELECT * FROM TOOL where LABEL='")+Label+_T("'"));
				rs.Delete();					
				rs.Close();				
            }
            catch(CDaoException *e){
			MessageBox(e->m_pErrorInfo->m_strDescription);
			e->Delete();
            }
}

void CJobEditDlg::CurveDel(CString Label){
	//删除选中的曲线信息	
	 COleVariant OleVariant ;
		try{					
				CDaoRecordset rs(&m_DataBase);
				rs.Open(dbOpenDynaset,_T("SELECT * FROM Curve where LABEL='")+Label+_T("'"));
				rs.Delete();					
				rs.Close();				
            }
            catch(CDaoException *e){
			MessageBox(e->m_pErrorInfo->m_strDescription);
			e->Delete();
            }
}

void CJobEditDlg::OriginDel(CString Label){
	//删除选中的原始信号	
	 COleVariant OleVariant ;
		try{					
				CDaoRecordset rs(&m_DataBase);
				rs.Open(dbOpenDynaset,_T("SELECT * FROM Origin where LABEL='")+Label+_T("'"));
				rs.Delete();					
				rs.Close();				
            }
            catch(CDaoException *e){
			MessageBox(e->m_pErrorInfo->m_strDescription);
			e->Delete();
            }
}

void CJobEditDlg::ToolInit(){
	//清空tool窗口	
					m_Tool.m_Label=_T("");					
					m_Tool.m_SN=_T("001");					
					m_Tool.m_Speed=_T("1000");						
					m_Tool.m_Type=_T("001");						
					m_Tool.m_Length=0;						
					m_Tool.m_Weight=0;						
					m_Tool.m_OuteRdiam=0;			
}

void CJobEditDlg::CurveInit(){
	//清空tool窗口	
					m_Curve.m_Label=_T("");					
					m_Curve.m_unit=0;
					m_Curve.m_filter=0;
}

void CJobEditDlg::OriginInit(){
	//清空tool窗口	
					m_Origin.m_Label=_T("");					
					m_Origin.m_unit=0;
					m_Origin.m_filter=0;
}

void CJobEditDlg::ToolAdd()
{	
	 try{				
			   //新建数据库记录			  			   
				CDaoRecordset rs(&m_DataBase);				
				rs.Open(dbOpenDynaset,_T("SELECT * FROM TOOL where LABEL='")+m_Tool.m_Label+_T("'"));
				if(rs.IsBOF()){
			    rs.AddNew();
				rs.SetFieldValue(0,COleVariant(m_Tool.m_Label));
				rs.SetFieldValue(1,COleVariant(m_Tool.m_Type));	
				rs.SetFieldValue(2,COleVariant(m_Tool.m_SN));
				rs.SetFieldValue(3,COleVariant(m_Tool.m_Length));
				rs.SetFieldValue(4,COleVariant(m_Tool.m_Weight));	
				rs.SetFieldValue(5,COleVariant(m_Tool.m_OuteRdiam));
				rs.SetFieldValue(6,COleVariant(m_Tool.m_Speed));
				rs.Update();
				MessageBox(_T("添加成功,点左侧仪器信息继续添加"));
				}else{
				MessageBox(_T("仪器信息")+m_Tool.m_Label+_T("已经存在"),MB_OK);
				}
				rs.Close();		

	 } catch(CDaoException *e){
			MessageBox(e->m_pErrorInfo->m_strDescription);
			e->Delete();
     }
}

void CJobEditDlg::CurveAdd()
{	
	 try{				
			   //新建数据库记录			  			   
				CDaoRecordset rs(&m_DataBase);				
				rs.Open(dbOpenDynaset,_T("SELECT * FROM Curve where LABEL='")+m_Curve.m_Label+_T("'"));
				if(rs.IsBOF()){
			    rs.AddNew();
				rs.SetFieldValue(0,COleVariant(m_Curve.m_Label));
				rs.SetFieldValue(1,COleVariant(long(m_Curve.m_unitbox.GetCurSel())));	
				rs.SetFieldValue(2,COleVariant(long(m_Curve.m_filterbox.GetCurSel())));				
				rs.Update();
				MessageBox(_T("添加成功,点左侧曲线信息继续添加"));
				}else{
				MessageBox(_T("曲线信息")+m_Curve.m_Label+_T("已经存在"),MB_OK);
				}
				rs.Close();			
	 } catch(CDaoException *e){
			MessageBox(e->m_pErrorInfo->m_strDescription);
			e->Delete();
     }
}

void CJobEditDlg::OriginAdd()
{	
	 try{				
			   //新建数据库记录			  			   
				CDaoRecordset rs(&m_DataBase);				
				rs.Open(dbOpenDynaset,_T("SELECT * FROM Origin where LABEL='")+m_Origin.m_Label+_T("'"));
				if(rs.IsBOF()){
			    rs.AddNew();
				rs.SetFieldValue(0,COleVariant(m_Origin.m_Label));
				rs.SetFieldValue(1,COleVariant(long(m_Origin.m_unitbox.GetCurSel())));	
				rs.SetFieldValue(2,COleVariant(long(m_Origin.m_filterbox.GetCurSel())));				
				rs.Update();
				MessageBox(_T("添加成功,点左侧原始信号继续添加"));
				}else{
				MessageBox(_T("原始信号")+m_Origin.m_Label+_T("已经存在"),MB_OK);
				}
				rs.Close();	

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
		if(m_Tool.m_Label.Trim().IsEmpty()){
		MessageBox(_T("名称不能为空"),MB_OK);
		}else{
		ToolAdd();				
		}
		break;
	case 2:
		if(m_Curve.m_Label.Trim().IsEmpty()){
		MessageBox(_T("名称不能为空"),MB_OK);
		}else{
		CurveAdd();			
		}		
		break;
	case 3:
		if(m_Origin.m_Label.Trim().IsEmpty()){
		MessageBox(_T("名称不能为空"),MB_OK);
		}else{
		OriginAdd();			
		}		
		break;
	case 4:
		MessageBox(_T("添加控制信息"),MB_OK);
		break;
	case 5:
		MessageBox(_T("添加家电参数"),MB_OK);
		break;
	}
	DisplayTree();	
}

void CJobEditDlg::JobInit(){
	try{
			m_DataBase.Create(m_jobName);//如果新建作业则创建数据库
				CDaoTableDef td(&m_DataBase);        //构造表对象						
				
				//创建TOOL表
				td.Create(_T("TOOL"));
				td.CreateField(_T("LABEL"),dbText,50,0L);			
				td.CreateField(_T("TYPE"),dbText,50,0L);
				td.CreateField(_T("SN"),dbText,50,0L);					
				td.CreateField(_T("LENGTH"),dbSingle,0L);
				td.CreateField(_T("Weight"),dbSingle,0L);
			    td.CreateField(_T("OUTERDIAMATER"),dbSingle,0L);			
				td.CreateField(_T("SPEED"),dbText,50,0L);
			    td.Append();
				td.Close();

				//创建Curve表
				td.Create(_T("CURVE"));
				td.CreateField(_T("LABEL"),dbText,50,0L);
				td.CreateField(_T("UNIT"),dbInteger,0L);		
				td.CreateField(_T("FILTER"),dbInteger,0L);		
			    td.Append();
				td.Close();

				//创建Origin表
				td.Create(_T("ORIGIN"));
				td.CreateField(_T("LABEL"),dbText,50,0L);
				td.CreateField(_T("UNIT"),dbInteger,0L);		
				td.CreateField(_T("FILTER"),dbInteger,0L);		
			    td.Append();
				td.Close();				    

		}catch(CDaoException *e){
			MessageBox(e->m_pErrorInfo->m_strDescription);
			e->Delete();
        }
}

void CJobEditDlg::OnBnClickedDelete()
{
	// TODO: 在此添加控件通知处理程序代码
	HTREEITEM hSel=m_jeTree.GetSelectedItem();//当前节点
	HTREEITEM hPar=m_jeTree.GetParentItem(hSel);//父节点		
	switch(m_editTable){
	case 1:				
			if(hPar){//非一级节点
				if(MessageBox(_T("确定删除仪器信息")+m_jeTree.GetItemText(hSel)+_T("吗？"),_T("警告"),MB_OKCANCEL)==IDOK){
					ToolDel(m_jeTree.GetItemText(hSel));					
				}			
			}			
		break;
	case 2:
		   if(hPar){//非一级节点
				if(MessageBox(_T("确定删除曲线信息")+m_jeTree.GetItemText(hSel)+_T("吗？"),_T("警告"),MB_OKCANCEL)==IDOK){
					CurveDel(m_jeTree.GetItemText(hSel));					
				}			
			}	
		break;
	case 3:
		if(hPar){//非一级节点
				if(MessageBox(_T("确定删除原始信号")+m_jeTree.GetItemText(hSel)+_T("吗？"),_T("警告"),MB_OKCANCEL)==IDOK){
					OriginDel(m_jeTree.GetItemText(hSel));					
				}			
			}	
		break;
	case 4:
		MessageBox(_T("删除控制信息"),MB_OK);
		break;
	case 5:
		MessageBox(_T("删除家电参数"),MB_OK);
		break;
	}
	DisplayTree();	
}

void CJobEditDlg::ToolUpdate(CString Label)
{	
	 COleVariant OleVariant ;
		try{						
				CDaoRecordset rs(&m_DataBase);
				rs.Open(dbOpenDynaset,_T("SELECT * FROM TOOL where LABEL='")+Label+_T("'"));
				rs.Edit();
				if(!rs.IsBOF()){			   
				rs.SetFieldValue(0,COleVariant(m_Tool.m_Label));
				rs.SetFieldValue(1,COleVariant(m_Tool.m_Type));	
				rs.SetFieldValue(2,COleVariant(m_Tool.m_SN));
				rs.SetFieldValue(3,COleVariant(m_Tool.m_Length));
				rs.SetFieldValue(4,COleVariant(m_Tool.m_Weight));	
				rs.SetFieldValue(5,COleVariant(m_Tool.m_OuteRdiam));
				rs.SetFieldValue(6,COleVariant(m_Tool.m_Speed));
				rs.Update();
				MessageBox(_T("修改成功,点左侧仪器信息查看"));
				}else{
				MessageBox(_T("仪器信息")+m_Tool.m_Label+_T("不存在"),MB_OK);
				}				
				rs.Close();				
            }
            catch(CDaoException *e){
			MessageBox(e->m_pErrorInfo->m_strDescription);
			e->Delete();
            }
}

void CJobEditDlg::CurveUpdate(CString Label)
{	
	 COleVariant OleVariant ;
		try{						
				CDaoRecordset rs(&m_DataBase);
				rs.Open(dbOpenDynaset,_T("SELECT * FROM CURVE where LABEL='")+Label+_T("'"));
				rs.Edit();
				if(!rs.IsBOF()){			   
				rs.SetFieldValue(0,COleVariant(m_Curve.m_Label));
				rs.SetFieldValue(1,COleVariant(long(m_Curve.m_unitbox.GetCurSel())));	
				rs.SetFieldValue(2,COleVariant(long(m_Curve.m_filterbox.GetCurSel())));		
				rs.Update();
				MessageBox(_T("修改成功,点左侧曲线信息查看"));
				}else{
				MessageBox(_T("曲线信息")+m_Curve.m_Label+_T("不存在"),MB_OK);
				}				
				rs.Close();				
            }
            catch(CDaoException *e){
			MessageBox(e->m_pErrorInfo->m_strDescription);
			e->Delete();
            }
}

void CJobEditDlg::OriginUpdate(CString Label)
{	
	 COleVariant OleVariant ;
		try{					
				CDaoRecordset rs(&m_DataBase);
				rs.Open(dbOpenDynaset,_T("SELECT * FROM Origin where LABEL='")+Label+_T("'"));
				rs.Edit();
				if(!rs.IsBOF()){			   
				rs.SetFieldValue(0,COleVariant(m_Origin.m_Label));
				rs.SetFieldValue(1,COleVariant(long(m_Origin.m_unitbox.GetCurSel())));	
				rs.SetFieldValue(2,COleVariant(long(m_Origin.m_filterbox.GetCurSel())));		
				rs.Update();
				MessageBox(_T("修改成功,点左侧原始信号查看"));
				}else{
				MessageBox(_T("原始信号")+m_Origin.m_Label+_T("不存在"),MB_OK);
				}				
				rs.Close();				
            }
            catch(CDaoException *e){
			MessageBox(e->m_pErrorInfo->m_strDescription);
			e->Delete();
            }
}

void CJobEditDlg::OnBnClickedUpdate()
{
	// TODO: 在此添加控件通知处理程序代码
	// TODO: 在此添加控件通知处理程序代码
	HTREEITEM hSel=m_jeTree.GetSelectedItem();//当前节点
	HTREEITEM hPar=m_jeTree.GetParentItem(hSel);//父节点		
	switch(m_editTable){
	case 1:				
			if(hPar){//非一级节点				
				ToolUpdate(m_jeTree.GetItemText(hSel));						
			}			
		break;
	case 2:
		if(hPar){//非一级节点				
				CurveUpdate(m_jeTree.GetItemText(hSel));							
			}	
		break;
	case 3:
		if(hPar){//非一级节点				
				OriginUpdate(m_jeTree.GetItemText(hSel));							
			}	
		break;
	case 4:
		MessageBox(_T("删除控制信息"),MB_OK);
		break;
	case 5:
		MessageBox(_T("删除家电参数"),MB_OK);
		break;
	}
	DisplayTree();	
}
