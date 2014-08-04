// CurvePage.cpp : 实现文件
//

#include "stdafx.h"
#include "PlotEditor.h"
#include "CurvePage.h"
#include "afxdialogex.h"
#include "MainFrm.h"

// CCurvePage 对话框

IMPLEMENT_DYNAMIC(CCurvePage, CPropertyPage)

CCurvePage::CCurvePage()
	: CPropertyPage(CCurvePage::IDD)
{

}

CCurvePage::~CCurvePage()
{
}

void CCurvePage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_BIAOSHI, combBiaoShi);
	DDX_Control(pDX, IDC_COMBO_SHUJU, combShuJu);
	DDX_Control(pDX, IDC_COMBO_GUIDAO, combGuiDao);
	DDX_Control(pDX, IDC_EDIT_ZUOBIANJIE, editZuoBianJie);
	DDX_Control(pDX, IDC_EDIT_YOUBIANJIE, editYouBianJie);
	DDX_Control(pDX, IDC_BUTTON_COLOR, btColor);
	DDX_Control(pDX, IDC_COMBO_KEDU, combKeDu);
	DDX_Control(pDX, IDC_COMBO_XIANXING, combXianXing);
	DDX_Control(pDX, IDC_COMBO_XIANKUAN, combXianKuan);
	DDX_Control(pDX, IDC_COMBO_HUIRAOMOSHI, combHuiRaoMoShi);
	DDX_Control(pDX, IDC_EDIT_HUIRAOCISHU, combHuiRaoCiShu);
	DDX_Control(pDX, IDC_SPIN, spinEdit);
}


BEGIN_MESSAGE_MAP(CCurvePage, CPropertyPage)
	ON_BN_CLICKED(IDOK, &CCurvePage::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CCurvePage::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_BUTTON_ADD, &CCurvePage::OnBnClickedButtonAdd)
	ON_BN_CLICKED(IDC_BUTTON_DEL, &CCurvePage::OnBnClickedButtonDel)
END_MESSAGE_MAP()


// CCurvePage 消息处理程序


BOOL CCurvePage::OnInitDialog()
{
	CPropertyPage::OnInitDialog();

	// TODO:  在此添加额外的初始化
	CWnd::SetWindowText(theApp.GetResString(IDS_CURVE_TAB1));
	if(theApp.curveList.IsEmpty())
	{
	
	}
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


void CCurvePage::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
}


void CCurvePage::OnBnClickedCancel()
{
	// TODO: 在此添加控件通知处理程序代码
}


void CCurvePage::OnBnClickedButtonAdd()
{
	// TODO: 在此添加控件通知处理程序代码
}


void CCurvePage::OnBnClickedButtonDel()
{
	// TODO: 在此添加控件通知处理程序代码
}
