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
	DDX_Control(pDX, IDC_STATIC_COLOR, mStaticColor);
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

	((CComboBox*)GetDlgItem(IDC_COMBO_GUIDAO))->InsertString(0,_T("Track 1"));
	((CComboBox*)GetDlgItem(IDC_COMBO_GUIDAO))->InsertString(1,_T("Depth Track"));
	((CComboBox*)GetDlgItem(IDC_COMBO_GUIDAO))->InsertString(2,_T("Track 23"));

	((CComboBox*)GetDlgItem(IDC_COMBO_KEDU))->InsertString(0,_T("线性"));
	((CComboBox*)GetDlgItem(IDC_COMBO_KEDU))->InsertString(1,_T("对数"));

	((CComboBox*)GetDlgItem(IDC_COMBO_XIANXING))->InsertString(0,_T("实线"));
	((CComboBox*)GetDlgItem(IDC_COMBO_XIANXING))->InsertString(1,_T("虚线1"));
	((CComboBox*)GetDlgItem(IDC_COMBO_XIANXING))->InsertString(2,_T("虚线2"));
	((CComboBox*)GetDlgItem(IDC_COMBO_XIANXING))->InsertString(3,_T("虚线3"));
	((CComboBox*)GetDlgItem(IDC_COMBO_XIANXING))->InsertString(4,_T("虚线4"));
	((CComboBox*)GetDlgItem(IDC_COMBO_XIANXING))->InsertString(5,_T("虚线5"));
	
	((CComboBox*)GetDlgItem(IDC_COMBO_XIANKUAN))->InsertString(0,_T("0"));
	((CComboBox*)GetDlgItem(IDC_COMBO_XIANKUAN))->InsertString(1,_T("0.5"));
	((CComboBox*)GetDlgItem(IDC_COMBO_XIANKUAN))->InsertString(2,_T("1"));
	((CComboBox*)GetDlgItem(IDC_COMBO_XIANKUAN))->InsertString(3,_T("1.5"));
	((CComboBox*)GetDlgItem(IDC_COMBO_XIANKUAN))->InsertString(4,_T("2"));
	((CComboBox*)GetDlgItem(IDC_COMBO_XIANKUAN))->InsertString(5,_T("2.5"));

	((CComboBox*)GetDlgItem(IDC_COMBO_HUIRAOMOSHI))->InsertString(0,_T("左回绕"));
	((CComboBox*)GetDlgItem(IDC_COMBO_HUIRAOMOSHI))->InsertString(1,_T("右回绕"));
	((CComboBox*)GetDlgItem(IDC_COMBO_HUIRAOMOSHI))->InsertString(2,_T("左右都回绕"));
	((CComboBox*)GetDlgItem(IDC_COMBO_HUIRAOMOSHI))->InsertString(3,_T("不回绕"));
	((CComboBox*)GetDlgItem(IDC_COMBO_HUIRAOMOSHI))->InsertString(4,_T("No Wrap"));

	

	if(theApp.curveList.IsEmpty())
	{
		((CComboBox*)GetDlgItem(IDC_COMBO_GUIDAO))->SetCurSel(0);
		((CComboBox*)GetDlgItem(IDC_COMBO_KEDU))->SetCurSel(0);
		((CComboBox*)GetDlgItem(IDC_COMBO_XIANXING))->SetCurSel(0);
		((CComboBox*)GetDlgItem(IDC_COMBO_XIANKUAN))->SetCurSel(0);
		((CComboBox*)GetDlgItem(IDC_COMBO_HUIRAOMOSHI))->SetCurSel(0);
		editZuoBianJie.SetWindowText(_T("1"));
		editYouBianJie.SetWindowText(_T("100"));
		mStaticColor.SetStaiticColor(RGB(0xFF,0x00,0x00));
		combHuiRaoCiShu.SetWindowText(_T("1"));
	}
	else
	{
		//解析theApp.curveList
		int i = 0;
		POSITION pos3 = theApp.curveList.GetHeadPosition();
		while(pos3)
		{
			CCurveInfo* plist = theApp.curveList.GetNext(pos3);
			AddCurveInfo(plist,i);
			i++;
		}
		if(!SetCurveInfo(0))
		{
			SendMessage(WM_CLOSE);
		}
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CCurvePage::AddCurveInfo(CCurveInfo* pCurveInfo, int item)
{
	((CComboBox*)GetDlgItem(IDC_COMBO_BIAOSHI))->InsertString(item,pCurveInfo->strCurveName);
	((CComboBox*)GetDlgItem(IDC_COMBO_SHUJU))->InsertString(item,pCurveInfo->strCurveName);
}

bool CCurvePage::SetCurveInfo(int item)
{
	if(item >= theApp.curveList.GetCount() || item <0)
	{
		return false;
	}
	CCurveInfo* plist = theApp.curveList.GetAt(theApp.curveList.FindIndex(item));
	CString str;

	((CComboBox*)GetDlgItem(IDC_COMBO_BIAOSHI))->SetCurSel(plist->curveName);

	((CComboBox*)GetDlgItem(IDC_COMBO_SHUJU))->SetCurSel(plist->curveName);
	
	((CComboBox*)GetDlgItem(IDC_COMBO_GUIDAO))->SetCurSel(plist->curveTrack);

	str.Format(_T("%d"),plist->leftLimit);
	editZuoBianJie.SetWindowText(str);

	str.Format(_T("%d"),plist->rightLimit);
	editYouBianJie.SetWindowText(str);

	mStaticColor.SetStaiticColor(plist->curveColor);

	((CComboBox*)GetDlgItem(IDC_COMBO_KEDU))->SetCurSel(plist->graduation);

	((CComboBox*)GetDlgItem(IDC_COMBO_XIANXING))->SetCurSel(plist->lineType);
	
	str.Format(_T("%d"),plist->lineWidth);
	((CComboBox*)GetDlgItem(IDC_COMBO_XIANKUAN))->SetCurSel(plist->lineWidth);

	((CComboBox*)GetDlgItem(IDC_COMBO_HUIRAOMOSHI))->SetCurSel(plist->wrapMode);

	str.Format(_T("%d"),plist->wrapCount);
	combHuiRaoCiShu.SetWindowText(str);
	
	return true;
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
	CString strTemp;
	combShuJu.GetWindowText(strTemp);
	if(strTemp.IsEmpty())
	{
		MessageBox(_T("请输入数据内容"),_T("提示"),MB_ICONQUESTION|MB_OK);
		return;
	}
}


void CCurvePage::OnBnClickedButtonDel()
{
	// TODO: 在此添加控件通知处理程序代码
}
