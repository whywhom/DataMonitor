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
	curveSelectColor = RGB(0xFF,0x00,0x00);
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
	ON_BN_CLICKED(IDC_BUTTON_COLOR, &CCurvePage::OnBnClickedButtonColor)
	ON_CBN_SELCHANGE(IDC_COMBO_BIAOSHI, &CCurvePage::OnCbnSelchangeComboBiaoshi)
	ON_CBN_SELCHANGE(IDC_COMBO_SHUJU, &CCurvePage::OnCbnSelchangeComboShuju)
END_MESSAGE_MAP()


// CCurvePage 消息处理程序


BOOL CCurvePage::OnInitDialog()
{
	CPropertyPage::OnInitDialog();

	// TODO:  在此添加额外的初始化
	spinEdit.SetBuddy(GetDlgItem(IDC_EDIT_HUIRAOCISHU));  //设置伙伴窗口
	spinEdit.SetRange(0, 100);  //设置min~max

	CWnd::SetWindowText(theApp.GetResString(IDS_CURVE_TAB1));

	((CComboBox*)GetDlgItem(IDC_COMBO_BIAOSHI))->ResetContent();
	((CComboBox*)GetDlgItem(IDC_COMBO_SHUJU))->ResetContent();

	((CComboBox*)GetDlgItem(IDC_COMBO_GUIDAO))->InsertString(0,_T("Track 1"));
	((CComboBox*)GetDlgItem(IDC_COMBO_GUIDAO))->InsertString(1,_T("Depth Track"));
	((CComboBox*)GetDlgItem(IDC_COMBO_GUIDAO))->InsertString(2,_T("Track 23"));

	((CComboBox*)GetDlgItem(IDC_COMBO_KEDU))->InsertString(0,_T("线性"));
	((CComboBox*)GetDlgItem(IDC_COMBO_KEDU))->InsertString(1,_T("对数"));

	((CComboBox*)GetDlgItem(IDC_COMBO_XIANXING))->InsertString(0,_T("实线"));//PS_SOLID
	((CComboBox*)GetDlgItem(IDC_COMBO_XIANXING))->InsertString(1,_T("虚线"));//PS_DASH
	((CComboBox*)GetDlgItem(IDC_COMBO_XIANXING))->InsertString(2,_T("点线"));//PS_DOT
	((CComboBox*)GetDlgItem(IDC_COMBO_XIANXING))->InsertString(3,_T("点划线"));//PS_DASHDOT
	((CComboBox*)GetDlgItem(IDC_COMBO_XIANXING))->InsertString(4,_T("双点划线"));//PS_DASHDOTDOT
	//((CComboBox*)GetDlgItem(IDC_COMBO_XIANXING))->InsertString(5,_T("虚线5"));
	
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
		mStaticColor.SetStaiticColor(curveSelectColor);
		combHuiRaoCiShu.SetWindowText(_T("1"));
		spinEdit.SetPos(1);  //设置起始位置
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

	((CComboBox*)GetDlgItem(IDC_COMBO_BIAOSHI))->SetCurSel(item);

	((CComboBox*)GetDlgItem(IDC_COMBO_SHUJU))->SetCurSel(item);
	
	((CComboBox*)GetDlgItem(IDC_COMBO_GUIDAO))->SetCurSel(plist->curveTrack);

	str.Format(_T("%d"),plist->leftLimit);
	editZuoBianJie.SetWindowText(str);

	str.Format(_T("%d"),plist->rightLimit);
	editYouBianJie.SetWindowText(str);

	mStaticColor.SetStaiticColor(plist->curveColor);
	curveSelectColor = plist->curveColor;

	((CComboBox*)GetDlgItem(IDC_COMBO_KEDU))->SetCurSel(plist->graduation);

	((CComboBox*)GetDlgItem(IDC_COMBO_XIANXING))->SetCurSel(plist->lineType);
	
	str.Format(_T("%d"),plist->lineWidth);
	((CComboBox*)GetDlgItem(IDC_COMBO_XIANKUAN))->SetCurSel(plist->lineWidth);

	((CComboBox*)GetDlgItem(IDC_COMBO_HUIRAOMOSHI))->SetCurSel(plist->wrapMode);

	str.Format(_T("%d"),plist->wrapCount);
	combHuiRaoCiShu.SetWindowText(str);
	spinEdit.SetPos(plist->wrapCount);  //设置起始位置
	return true;
}

void CCurvePage::DeleteCurveInfo( )
{
	int item = combBiaoShi.GetCurSel();
	theApp.curveList.RemoveAt(theApp.curveList.FindIndex(item));
	combBiaoShi.DeleteString(item);
	combShuJu.DeleteString(item);
	if(item > 0)
	{
		SetCurveInfo(item - 1);
	}
	else
	{
		SetCurveInfo(0);
	}
}
void CCurvePage::InseertCurveInfo( )
{
	int i = 0;
	int j = 0;
	POSITION pos,pos2;
	bool bLast = true;
	CString strTemp;
	combShuJu.GetWindowText(strTemp);
	if(strTemp.IsEmpty())
	{
		MessageBox(_T("请输入数据内容"),_T("提示"),MB_ICONQUESTION|MB_OK);
		return;
	}
	if(theApp.curveList.IsEmpty())
	{
		CCurveInfo* pCurveInfo = new CCurveInfo();
		GetCurveData(pCurveInfo, strTemp);
		theApp.curveList.AddTail(pCurveInfo);
	}
	else
	{
		i = 0;
		pos = theApp.curveList.GetHeadPosition();
		while(pos)
		{
			pos2 = pos;
			CCurveInfo* plist = theApp.curveList.GetNext(pos);
			if(plist->strCurveName.Compare(strTemp) == 0)
			{
				MessageBox(_T("数据已存在，请重新输入！"),_T("提示"),MB_ICONQUESTION|MB_OK);
				return;
			}
			else if(plist->strCurveName.Compare(strTemp) > 0)
			{
				bLast = false;
				break;
			}
			else
			{
				continue;
			}
			i++;
		}
		CCurveInfo* pCurveInfo = new CCurveInfo();

		GetCurveData(pCurveInfo, strTemp);
		
		if(bLast)
		{
			theApp.curveList.AddTail(pCurveInfo);
		}
		else
		{
			theApp.curveList.InsertBefore(pos2,pCurveInfo);
		}
	}
	i = 0;
	((CComboBox*)GetDlgItem(IDC_COMBO_BIAOSHI))->ResetContent();
	((CComboBox*)GetDlgItem(IDC_COMBO_SHUJU))->ResetContent();
	pos = theApp.curveList.GetHeadPosition();
	while(pos)
	{
		CCurveInfo* plist = theApp.curveList.GetNext(pos);
		AddCurveInfo(plist,i);
		if(plist->strCurveName == strTemp)
		{
			j = i;
		}
		i++;
	}
	SetCurveInfo(j);
}
void CCurvePage::GetCurveData(CCurveInfo* pCurveInfo, CString strTemp)
{
	CString str;
	pCurveInfo->strCurveName = strTemp;
	pCurveInfo->curveTrack = combGuiDao.GetCurSel();

	editZuoBianJie.GetWindowText(str);
	pCurveInfo->leftLimit = _wtoi(str);
	editYouBianJie.GetWindowText(str);
	pCurveInfo->rightLimit = _wtoi(str);

	pCurveInfo->curveColor = curveSelectColor;
	pCurveInfo->graduation = combKeDu.GetCurSel();
	pCurveInfo->lineType = combXianXing.GetCurSel();
	pCurveInfo->lineWidth = combXianKuan.GetCurSel();
	pCurveInfo->wrapMode = combHuiRaoMoShi.GetCurSel();

	combHuiRaoCiShu.GetWindowText(str);
	pCurveInfo->wrapCount = _wtoi(str);
	spinEdit.SetPos(pCurveInfo->wrapCount);
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
	InseertCurveInfo();
}


void CCurvePage::OnBnClickedButtonDel()
{
	// TODO: 在此添加控件通知处理程序代码
	DeleteCurveInfo();
}


void CCurvePage::OnBnClickedButtonColor()
{
	// TODO: 在此添加控件通知处理程序代码
	CColorDialog dlg;

	if (dlg.DoModal() == IDOK)
	{
		curveSelectColor = dlg.GetColor();
		mStaticColor.SetStaiticColor(curveSelectColor);
	}
}


void CCurvePage::OnCbnSelchangeComboBiaoshi()
{
	// TODO: 在此添加控件通知处理程序代码
	int item = ((CComboBox*)GetDlgItem(IDC_COMBO_BIAOSHI))->GetCurSel();
	((CComboBox*)GetDlgItem(IDC_COMBO_SHUJU))->SetCurSel(item);
	SetCurveData(item);
}


void CCurvePage::OnCbnSelchangeComboShuju()
{
	// TODO: 在此添加控件通知处理程序代码
	int item = ((CComboBox*)GetDlgItem(IDC_COMBO_SHUJU))->GetCurSel();
	((CComboBox*)GetDlgItem(IDC_COMBO_BIAOSHI))->SetCurSel(item);
	SetCurveData(item);
}

void CCurvePage::SetCurveData(int index)
{
	CString str;
	CCurveInfo* plist = theApp.curveList.GetAt(theApp.curveList.FindIndex(index));

	combGuiDao.SetCurSel(plist->curveTrack);

	str.Format(_T("%d"),plist->leftLimit);
	editZuoBianJie.SetWindowText(str);
	str.Format(_T("%d"),plist->rightLimit);
	editYouBianJie.SetWindowText(str);

	curveSelectColor = plist->curveColor;
	mStaticColor.SetStaiticColor(curveSelectColor);

	combKeDu.SetCurSel(plist->graduation);
	combXianXing.SetCurSel(plist->lineType);
	combXianKuan.SetCurSel(plist->lineWidth);
	combHuiRaoMoShi.SetCurSel(plist->wrapMode);

	str.Format(_T("%d"),plist->wrapCount);
	combHuiRaoCiShu.SetWindowText(str);
	spinEdit.SetPos(plist->wrapCount);
}