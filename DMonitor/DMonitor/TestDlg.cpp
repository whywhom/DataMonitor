// TestDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "DMonitor.h"
#include "TestDlg.h"
#include "afxdialogex.h"
#include <string>
#include <iostream> 
using namespace std; 

// CTestDlg 对话框

IMPLEMENT_DYNAMIC(CTestDlg, CDialog)

CTestDlg::CTestDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CTestDlg::IDD, pParent)
{
	curveSelectColor = RGB(0xFF,0x00,0x00);
	pCurList = NULL;
}

CTestDlg::~CTestDlg()
{
}

void CTestDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, mTestList);
	DDX_Control(pDX, IDC_COMBO_SIGNAL, mCbSignal);
	DDX_Control(pDX, IDC_COMBO_TITLE, nAbbr);
	DDX_Control(pDX, IDC_COMBO_UNIT, mTestUnit);
	DDX_Control(pDX, IDC_EDIT_MIN, nTestEditMin);
	DDX_Control(pDX, IDC_EDIT_MAX, nTestEditMax);
	DDX_Control(pDX, IDC_BUTTON_ADD, mTestAdd);
	DDX_Control(pDX, IDC_BUTTON_DEL, mTestDel);
	DDX_Control(pDX, IDC_STATIC_COLOR, mStaticColor);
}


BEGIN_MESSAGE_MAP(CTestDlg, CDialog)
	ON_BN_CLICKED(IDOK, &CTestDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CTestDlg::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_BUTTON1, &CTestDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON3, &CTestDlg::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON_COLOR, &CTestDlg::OnBnClickedButtonColor)
END_MESSAGE_MAP()


// CTestDlg 消息处理程序


BOOL CTestDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类
	switch (pMsg->message)
	{
	case WM_KEYDOWN:
		{
			switch (pMsg->wParam)
			{
			case VK_ESCAPE:
				return TRUE;
			case VK_UP://屏蔽上下方向键
			case VK_DOWN:	
			case VK_SPACE:
			case VK_RETURN://屏蔽回车
				return TRUE;
			default:
				break;
			}
		}
		break;
	default:
		break;
	}
	return CDialog::PreTranslateMessage(pMsg);
}


BOOL CTestDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	CRect   temprect(0,0,320,200);
    CRect rect;
    CRect rc;
    CRect rc2;
    //CWnd::SetWindowPos(NULL,0,0,temprect.Width(),temprect.Height(),SWP_NOZORDER|SWP_NOMOVE);
    CenterWindow();
	InitCtrl();
	
	if(theApp.workInfoList.IsEmpty())
	{
		//初始化

		InitComboBox(0,_T("深度"),_T("DEPT"),_T("m"));
		InitComboBox(1,_T("井温"),_T("TEMP"),_T("C"));
		InitComboBox(2,_T("电阻率"),_T("RM"),_T("rm"));
		InitComboBox(3,_T("放射性"),_T("GR"),_T("GAPI"));
		InitComboBox(4,_T("磁三分量"),_T("MAG"),_T("mag"));
		InitComboBox(5,_T("磁定位器"),_T("CCL"),_T("CCL"));

		nTestEditMin.SetWindowText(_T(""));
		nTestEditMax.SetWindowText(_T(""));

		SetCurveInfo(0);
	}
	else
	{
		//解析theApp.workInfoList
		int i = 0;
		POSITION pos3 = theApp.workInfoList.GetHeadPosition();
		while(pos3)
		{
			CWorkInfo* plist = theApp.workInfoList.GetNext(pos3);
			AddWorkInfo(plist,i);
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

bool CTestDlg::SetCurveInfo(int item)
{
	if(item >= theApp.workInfoList.GetCount() || item <0)
	{
		return false;
	}
	CWorkInfo* plist = theApp.workInfoList.GetAt(theApp.workInfoList.FindIndex(item));
	pCurList = plist;

	CString str;

	((CComboBox*)GetDlgItem(IDC_COMBO_SIGNAL))->SetCurSel(item);

	((CComboBox*)GetDlgItem(IDC_COMBO_TITLE))->SetCurSel(item);
	
	((CComboBox*)GetDlgItem(IDC_COMBO_UNIT))->SetCurSel(item);

	str.Format(_T("%d"),plist->leftLimit);
	nTestEditMin.SetWindowText(str);

	str.Format(_T("%d"),plist->rightLimit);
	nTestEditMax.SetWindowText(str);

	mStaticColor.SetStaiticColor(plist->curveColor);
	curveSelectColor = plist->curveColor;
	return true;
}
void CTestDlg::InitComboBox(int id,CString signal,CString title, CString uint)
{
	((CComboBox*)GetDlgItem(IDC_COMBO_SIGNAL))->AddString(signal);
	((CComboBox*)GetDlgItem(IDC_COMBO_TITLE))->AddString(title);
	((CComboBox*)GetDlgItem(IDC_COMBO_UNIT))->AddString(uint);
	mTestList.InsertItem(id,signal);
	mTestList.SetItemText(id,1,title);
	mTestList.SetItemText(id,2,uint);
	CString strMin,strMax;
	nTestEditMin.GetWindowText(strMin);
	nTestEditMax.GetWindowText(strMax);
	if(!strMin.IsEmpty() || !strMax.IsEmpty())
	{
		mTestList.SetItemText(id,3,strMin+_T(" - ")+strMax);
	}
}

void CTestDlg::InitCtrl()
{
	CString m_unitary[]={_T("mV"),_T("GAPI"),_T("CPS"),_T("us/m"),_T("m"),_T("cm"),_T("mm"),_T("uS"),_T("mS"),_T("S"),_T("MΩ"),_T("kΩ"),_T("Ω"),_T("Ω.m"),_T("s"),_T("S/m"),_T("mS/m"),_T("m/h"),_T("Kg"),_T("Kn"),_T("N"),_T("℃"),_T("℃/m"),_T("kg/m^3"),_T("g/cm^3"),_T("km"),_T("km^3"),_T("m^3"),_T("dm^3"),_T("m/s^2"),_T("mm/s^2"),_T("rad"),_T("mrad"),_T("urad"),_T("°"),_T("km^2"),_T("ha"),_T("dm^2"),_T("cm^2"),_T("mm^2"),_T("kg/s"),_T("m^3/s"),_T("m^3/min"),_T("m^3/d"),_T("L/s"),_T("Std.m3/m3"),_T("t"),_T("Mg/m^3"),_T("pu"),_T("J"),_T("Mev"),_T("MW"),_T("KW"),_T("W"),_T("Pa"),_T("kPa"),_T("MPa"),_T("ATM"),_T("PSI"),_T("Bq"),_T("r/s"),_T("min"),_T("h"),_T("d"),_T("a"),_T("m/s"),_T("Pa.s"),_T("mPa.s"),_T("N.s/m^2"),_T("%"),_T("NAPI"),_T("inch"),_T("API"),_T("V"),_T("度"),_T("uS/m"),_T("ppm"),_T("I")};
	for(int i=0;i<78;i++)
	{
		((CComboBox*)GetDlgItem(IDC_COMBO_UNIT))->AddString(m_unitary[i]);
	}
	mStaticColor.SetStaiticColor(curveSelectColor);
	mTestList.InsertColumn( 0, _T("原始信号"), LVCFMT_LEFT, 60 ); 
    mTestList.InsertColumn( 1, _T("英文简写"), LVCFMT_LEFT, 60 ); 
	mTestList.InsertColumn( 2, _T("度量单位"), LVCFMT_LEFT, 60 ); 
	mTestList.InsertColumn( 2, _T("取值范围"), LVCFMT_LEFT, 60 ); 
	mTestList.SetExtendedStyle( LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES | LVS_EX_HEADERDRAGDROP ); 
}
void CTestDlg::AddWorkInfo(CWorkInfo* pWorkInfo, int item)
{
	//((CComboBox*)GetDlgItem(IDC_COMBO_BIAOSHI))->InsertString(item,pWorkInfo->strCurveName);
	//((CComboBox*)GetDlgItem(IDC_COMBO_SHUJU))->InsertString(item,pWorkInfo->strCurveName);
    
}

void CTestDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	CDialog::OnOK();
}


void CTestDlg::OnBnClickedCancel()
{
	// TODO: 在此添加控件通知处理程序代码
	CDialog::OnCancel();
}


void CTestDlg::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
}


void CTestDlg::OnBnClickedButton3()
{
	// TODO: 在此添加控件通知处理程序代码
}


void CTestDlg::OnBnClickedButtonColor()
{
	// TODO: 在此添加控件通知处理程序代码
}




