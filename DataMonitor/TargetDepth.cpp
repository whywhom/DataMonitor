// TargetDepth.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "DataMonitor.h"
#include "TargetDepth.h"
#include "afxdialogex.h"


// CTargetDepth �Ի���

IMPLEMENT_DYNAMIC(CTargetDepth, CDialog)

CTargetDepth::CTargetDepth(CWnd* pParent /*=NULL*/)
	: CDialog(CTargetDepth::IDD, pParent)
{
	::GetPrivateProfileString(_T("TargetDepth"),_T("Depth1"),_T("100"),strEditOne.GetBuffer(MAX_PATH),MAX_PATH,theApp.strIniFilePath); //_T("100");
	::GetPrivateProfileString(_T("TargetDepth"),_T("Depth2"),_T("100"),strEditTwo.GetBuffer(MAX_PATH),MAX_PATH,theApp.strIniFilePath);//_T("100");
}

CTargetDepth::~CTargetDepth()
{
}

void CTargetDepth::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_DEPTHTIP, staticTip);
	DDX_Control(pDX, IDC_STATIC_DEPTHONE, staticOne);
	DDX_Control(pDX, IDC_STATIC_DEPTHTWO, staticTwo);
	DDX_Control(pDX, IDC_EDIT1, edit1);
	DDX_Control(pDX, IDC_EDIT2, edit2);
}


BEGIN_MESSAGE_MAP(CTargetDepth, CDialog)
	ON_BN_CLICKED(IDOK, &CTargetDepth::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CTargetDepth::OnBnClickedCancel)
	ON_WM_SHOWWINDOW()
END_MESSAGE_MAP()


// CTargetDepth ��Ϣ�������


void CTargetDepth::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if(!SaveParameter())
	{
		return;
	}
	::WritePrivateProfileString(_T("TargetDepth"),_T("Depth1"),strEditOne,theApp.strIniFilePath); 
	::WritePrivateProfileString(_T("TargetDepth"),_T("Depth2"),strEditTwo,theApp.strIniFilePath);
	CDialog::OnOK();
}


void CTargetDepth::OnBnClickedCancel()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CDialog::OnCancel();
}


BOOL CTargetDepth::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	SetWindowText(theApp.GetResString(IDS_SET_TARGETDEPTH));
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}




void CTargetDepth::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialog::OnShowWindow(bShow, nStatus);

	// TODO: �ڴ˴������Ϣ����������
	if(staticTip.m_hWnd != NULL)
	{
		staticTip.SetWindowText(theApp.GetResString(IDS_TARGETDEPTH_TIP));
	}
	if(staticOne.m_hWnd != NULL)
	{
		staticOne.SetWindowText(theApp.GetResString(IDS_TARGETDEPTH_ONE));
	}
	if(staticTwo.m_hWnd != NULL)
	{
		staticTwo.SetWindowText(theApp.GetResString(IDS_TARGETDEPTH_TWO));
	}
	if(edit1.m_hWnd != NULL)
	{
		edit1.SetWindowText(strEditOne);
	}
	if(edit2.m_hWnd != NULL)
	{
		edit2.SetWindowText(strEditTwo);
	}
}

bool CTargetDepth::SaveParameter()
{
	edit1.GetWindowText(strEditOne);
	edit2.GetWindowText(strEditTwo);
	int iEdit1 =  _ttoi(strEditOne);
	int iEdit2 =  _ttoi(strEditTwo);
	if(iEdit1 <100 || iEdit2 < 100)
	{
		MessageBox(theApp.GetResString(IDS_DEPTH_INVALIDPARA),_T("��ʾ"),MB_ICONERROR);
		return false;
	}
	return true;
}