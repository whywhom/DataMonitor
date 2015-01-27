// JobDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "DataMonitor.h"
#include "JobDlg.h"
#include "afxdialogex.h"
#include "JobCopyDlg.h"

#include <fstream> 
#include <cassert> 
#include "json\include\json.h"
using namespace std;
// CJobDlg �Ի���

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
	DDX_Control(pDX, IDC_ZCW_JOB_TREE, m_treeCtrl);
	DDX_Control(pDX, IDC_ZCW_JOB_TREE, m_treeCtrl);
}


BEGIN_MESSAGE_MAP(CJobDlg, CDialog)
	ON_COMMAND(ID_JOB_NEW, &CJobDlg::OnJobNew)
	ON_COMMAND(ID_JOB_OPEN, &CJobDlg::OnJobOpen)
	ON_COMMAND(ID_JOB_DEL, &CJobDlg::OnJobDel)
	ON_COMMAND(ID_JOB_COPY, &CJobDlg::OnJobCopy)
	ON_WM_SHOWWINDOW()
	ON_COMMAND(ID_JOB_LOAD, &CJobDlg::OnJobLoad)
	ON_BN_CLICKED(IDC_BUTTON1, &CJobDlg::OnBnClickedButton1)
END_MESSAGE_MAP()


// CJobDlg ��Ϣ�������


void CJobDlg::OnJobNew()
{
	// TODO: �ڴ���������������
	CFileDialog dlg(FALSE,_T(""),_T(""),OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,_T("tools files(*.tools)|*.tools|tool files(*.mdb)|*.mdb|All files(*.*)|*.*|"));//���챣���ļ��Ի���
	dlg.m_ofn.lpstrInitialDir=m_Path;
    if(dlg.DoModal()==IDOK)//��ʾ�����ļ��Ի���
	{
		CJobEditDlg m_jeDlg;
		m_jeDlg.m_jobName=dlg.GetPathName();//��ȡ�ļ�·������
		m_jeDlg.m_Open=FALSE;
		if(m_jeDlg.DoModal()==IDOK){
		m_treeCtrl.ModifyStyle(0,TVS_HASBUTTONS   |   TVS_LINESATROOT   |   TVS_HASLINES);	
		m_treeCtrl.DisplayTree (m_Path,TRUE);
		}
	}
}


void CJobDlg::OnJobOpen()
{
	// TODO: �ڴ���������������
		CJobEditDlg m_jeDlg;
		HTREEITEM sel_htem=m_treeCtrl.GetSelectedItem(); 
		 CString strPath=m_treeCtrl.GetFullPath(sel_htem);//��ȡ�ļ�·������
		if(!m_treeCtrl.FindSubDir(strPath)){//���ܱ༭�ǿ���Ŀ¼
		m_jeDlg.m_jobName=strPath;//��ȡ�ļ�·������
		m_jeDlg.m_Open=TRUE;
		m_jeDlg.DoModal();
		}
}


BOOL CJobDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	SetWindowText(m_Title); 	
	GetDlgItem(IDC_ZCW_JOB_TREE_TITLE)->SetWindowText(m_TreeTitle);
	m_treeCtrl.ModifyStyle(0,TVS_HASBUTTONS   |   TVS_LINESATROOT   |   TVS_HASLINES);	
	m_treeCtrl.DisplayTree (m_Path,TRUE);

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}


void CJobDlg::OnJobDel()
{
	// TODO: �ڴ���������������
	HTREEITEM sel_htem=m_treeCtrl.GetSelectedItem(); 
    CString strPath=m_treeCtrl.GetFullPath(sel_htem);//��ȡ�ļ�·������
	if(!m_treeCtrl.FindSubDir(strPath)){//����ɾ���ǿ���Ŀ¼
		if(MessageBox(_T("��ȷ��Ҫɾ����ҵ")+strPath+_T("��?"),_T("����"), MB_OKCANCEL )==IDOK)
		{
		  DeleteFile(strPath);
		  m_treeCtrl.DeleteItem(sel_htem);//ɾ���ڵ�
		} 
	}
}


void CJobDlg::OnJobCopy()
{
	// TODO: �ڴ���������������
	CJobCopyDlg m_jcDlg;
	HTREEITEM sel_htem=m_treeCtrl.GetSelectedItem(); //��ǰ�ڵ�
	HTREEITEM par_htem=m_treeCtrl.GetParentItem(sel_htem); //���ڵ�
	CString strPath=m_treeCtrl.GetFullPath(sel_htem);	
	if(!m_treeCtrl.FindSubDir(strPath)){//���ܸ���Ŀ¼
    CString stroldName=m_treeCtrl.GetItemText(sel_htem);
	m_jcDlg.m_oldName=stroldName.Left(stroldName.Find(_T(".")));//��ȡ�ļ���
	if(m_jcDlg.DoModal()==IDOK){
		CString strNewPath=m_treeCtrl.GetFullPath(par_htem)+_T("\\")+m_jcDlg.m_newName+_T(".tools");//����ҵ·��
		if(GetFileAttributes(strNewPath)==-1L){
		CopyFile(strPath,strNewPath,TRUE);
		m_treeCtrl.AddItem(par_htem,strNewPath);//��ӽڵ�
		}else{
			MessageBox(strNewPath+_T("�Ѵ���"),MB_OK);
		}
	}
	}
}

void CJobDlg::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialog::OnShowWindow(bShow, nStatus);

	// TODO: �ڴ˴������Ϣ����������
	if(receiveFlag == 1)
	{
		OnJobNew();
	}
}


void CJobDlg::OnJobLoad()
{
	// TODO: �ڴ���������������
	HTREEITEM sel_htem=m_treeCtrl.GetSelectedItem(); 
    CString strPath=m_treeCtrl.GetFullPath(sel_htem);//��ȡ�ļ�·������
	if(!m_treeCtrl.FindSubDir(strPath)){//���ܼ���Ŀ¼
		if(MessageBox(_T("��ȷ��Ҫ������ҵ")+strPath+_T("��?"),_T("����"), MB_OKCANCEL )==IDOK)
		{
		  LoadFile(strPath);		 
		} 
	}
}

void CJobDlg::LoadFile(CString strPath){
		CDaoDatabase c_DataBase;
		c_DataBase.Open(strPath);
		CDaoRecordset rs(&c_DataBase);
		COleVariant OleVariant ;

		//����Json�ķ���
		Json::Value root;//��ʾһ��json��ʽ�Ķ���
		Json::Value arrayTool;
		Json::Value arrayCurve;
		Json::Value arrayOrigin;
        Json::Value arrayControl;
        Json::Value arrayPower;

		Json::Value itemTool;
		Json::Value itemCurve;
		Json::Value itemOrigin;
        Json::Value itemControl;
        Json::Value itemPower;
		Json::FastWriter writer; 

		//���Tool���ӽڵ�
		rs.Open(dbOpenDynaset, L"SELECT * FROM TOOL" );		
		while(!rs.IsEOF()){	
			theApp.m_JobTool=new CDataMonitorApp::JobTool; 	

			rs.GetFieldValue (0,OleVariant);
			theApp.m_JobTool->Label=OleVariant.bstrVal;
			_bstr_t bstr_t0(OleVariant.bstrVal);  
            std::string str0(bstr_t0);  
			itemTool["Label"]=str0;			

			rs.GetFieldValue (1,OleVariant);
			theApp.m_JobTool->Type=OleVariant.bstrVal;
			_bstr_t bstr_t1(OleVariant.bstrVal);  
            std::string str1(bstr_t1);  
			itemTool["Type"]=str1;			

			rs.GetFieldValue (2,OleVariant);
			theApp.m_JobTool->SN=OleVariant.bstrVal;
			_bstr_t bstr_t2(OleVariant.bstrVal);  
            std::string str2(bstr_t2);  
			itemTool["SN"]=str2;			

			rs.GetFieldValue (3,OleVariant);
			itemTool["Length"]=OleVariant.fltVal;
			theApp.m_JobTool->Length=OleVariant.fltVal;

			rs.GetFieldValue (4,OleVariant);
			itemTool["Weight"]=OleVariant.fltVal;
			theApp.m_JobTool->Weight=OleVariant.fltVal;

			rs.GetFieldValue (5,OleVariant);
			itemTool["OuteRdiam"]=OleVariant.fltVal;
			theApp.m_JobTool->OUTERDIAMATER=OleVariant.fltVal;

			rs.GetFieldValue (6,OleVariant);
			theApp.m_JobTool->Speed=OleVariant.bstrVal;
			_bstr_t bstr_t6(OleVariant.bstrVal);  
            std::string str6(bstr_t6);  
			itemTool["Speed"]=str6;			

			arrayTool.append(itemTool);//json�ڵ�		
			theApp.m_JobTools.AddTail(theApp.m_JobTool);
			
			rs.MoveNext();
		}
		rs.Close();     
		//���Curve���ӽڵ�
		rs.Open(dbOpenDynaset, L"SELECT * FROM Curve" );		
		while(!rs.IsEOF()){
			theApp.m_JobCurve=new CDataMonitorApp::JobCurve;
			rs.GetFieldValue (0,OleVariant);
			theApp.m_JobCurve->Label=OleVariant.bstrVal;
			_bstr_t bstr_c0(OleVariant.bstrVal);  
            std::string str_c0(bstr_c0); 
			itemCurve["Label"]=str_c0;			
			rs.GetFieldValue (1,OleVariant);
			theApp.m_JobCurve->UNIT=OleVariant.iVal;
			itemCurve["Unit"]=OleVariant.iVal;
			rs.GetFieldValue (2,OleVariant);
			theApp.m_JobCurve->FILTER=OleVariant.iVal;
			itemCurve["Filter"]=OleVariant.iVal;
			arrayCurve.append(itemCurve);
			theApp.m_JobCurves.AddTail(theApp.m_JobCurve);
			rs.MoveNext();
		}
		rs.Close();     

		//���Origin���ӽڵ�
		rs.Open(dbOpenDynaset, L"SELECT * FROM Origin" );		
		while(!rs.IsEOF()){
			theApp.m_JobOrigin=new CDataMonitorApp::JobOrigin;
			rs.GetFieldValue (0,OleVariant);
			theApp.m_JobOrigin->Label=OleVariant.bstrVal;
			_bstr_t bstr_o0(OleVariant.bstrVal);  
            std::string str_o0(bstr_o0);  
			itemOrigin["Label"]=str_o0;			
			rs.GetFieldValue (1,OleVariant);
			theApp.m_JobOrigin->UNIT=OleVariant.iVal;
			itemOrigin["Unit"]=OleVariant.iVal;
			rs.GetFieldValue (2,OleVariant);
			theApp.m_JobOrigin->FILTER=OleVariant.iVal;
			itemOrigin["Filter"]=OleVariant.iVal;
			arrayOrigin.append(itemOrigin);
			theApp.m_JobOrigins.AddTail(theApp.m_JobOrigin);
			rs.MoveNext();
		}
		rs.Close();  

		//���Control���ӽڵ�
		rs.Open(dbOpenDynaset, L"SELECT * FROM Control" );		
		while(!rs.IsEOF()){
			theApp.m_JobControl=new CDataMonitorApp::JobControl;
			rs.GetFieldValue (0,OleVariant);
			theApp.m_JobControl->Label=OleVariant.bstrVal;
			_bstr_t bstr_ct0(OleVariant.bstrVal);  
            std::string str_ct0(bstr_ct0);  
			itemControl["Label"]=str_ct0;	

			rs.GetFieldValue (1,OleVariant);
			theApp.m_JobControl->UNITS=OleVariant.bstrVal;
			_bstr_t bstr_ct1(OleVariant.bstrVal);  
            std::string str_ct1(bstr_ct1);  
			itemControl["Units"]=str_ct1;	

			rs.GetFieldValue (2,OleVariant);
			theApp.m_JobControl->DATAVALUE=OleVariant.bstrVal;
			_bstr_t bstr_ct2(OleVariant.bstrVal);  
            std::string str_ct2(bstr_ct2);  
			itemControl["DataValue"]=str_ct2;

			arrayControl.append(itemControl);
			theApp.m_JobControls.AddTail(theApp.m_JobControl);
			rs.MoveNext();
		}
		rs.Close();  

		//���Power���ӽڵ�
		rs.Open(dbOpenDynaset, L"SELECT * FROM Power" );		
		while(!rs.IsEOF()){
			theApp.m_JobPower=new CDataMonitorApp::JobPower;
			rs.GetFieldValue (0,OleVariant);
			theApp.m_JobPower->Label=OleVariant.bstrVal;
			_bstr_t bstr_p0(OleVariant.bstrVal);  
            std::string strp0(bstr_p0);  
			itemPower["Label"]=strp0;
			rs.GetFieldValue (1,OleVariant);
			theApp.m_JobPower->CABLE=OleVariant.iVal;
			itemPower["Cable"]=OleVariant.iVal;
			rs.GetFieldValue (2,OleVariant);
			theApp.m_JobPower->TYPE=OleVariant.iVal;
			itemPower["Type"]=OleVariant.iVal;
			rs.GetFieldValue (3,OleVariant);
			theApp.m_JobPower->VOLTAGE=OleVariant.iVal;
			itemPower["Voltage"]=OleVariant.iVal;
			rs.GetFieldValue (4,OleVariant);
			theApp.m_JobPower->ElectricCurrent=OleVariant.iVal;
			itemPower["ElectricCurrent"]=OleVariant.iVal;
			rs.GetFieldValue (5,OleVariant);
			theApp.m_JobPower->MODE=OleVariant.bstrVal;
			_bstr_t bstr_p6(OleVariant.bstrVal);  
            std::string strp6(bstr_p6);  
			itemPower["MODE"]=strp6;
			arrayPower.append(itemPower);
			theApp.m_JobPowers.AddTail(theApp.m_JobPower);
			rs.MoveNext();
		}
		rs.Close(); 

		root["job"]="CurrentJob";		
		root["arrayTool"]=arrayTool;	
		root["arrayCurve"]=arrayCurve;	
		root["arrayOrigin"]=arrayOrigin;	
		root["arrayControl"]=arrayControl;
		root["arrayPower"]=arrayPower;
		//д��CurrentJob.json�ļ�
		
		//��ǰ��ҵ·��,json��ʽ		
		std::string json_file = writer.write(root);   
		ofstream ofs; 
		ofs.open("currentjob.json"); 
		assert(ofs.is_open()); 
		ofs<<json_file; 
}

void CJobDlg::OnBnClickedButton1()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	//�˴�������JobTools��������������
	POSITION pos=theApp.m_JobTools.GetHeadPosition();	
	CString LabelDemo;//��ʾ��ҵ������Tool��Labelֵ��-������
	while(pos!=NULL){
		theApp.m_JobTool=theApp.m_JobTools.GetNext(pos); 	
		LabelDemo+=theApp.m_JobTool->Label+_T("-");
	}
	MessageBox(LabelDemo);
}
