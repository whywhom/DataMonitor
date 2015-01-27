// CurvePage.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "PlotEditor.h"
#include "CurvePage.h"
#include "afxdialogex.h"
#include "MainFrm.h"

// CCurvePage �Ի���

IMPLEMENT_DYNAMIC(CCurvePage, CPropertyPage)

CCurvePage::CCurvePage()
	: CPropertyPage(CCurvePage::IDD)
{
	curveSelectColor = RGB(0xFF,0x00,0x00);
	pCurList = NULL;
}

CCurvePage::~CCurvePage()
{
	pCurList = NULL;
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
	ON_CBN_SELCHANGE(IDC_COMBO_GUIDAO, &CCurvePage::OnCbnSelchangeComboGuidao)
	ON_CBN_SELCHANGE(IDC_COMBO_KEDU, &CCurvePage::OnCbnSelchangeComboKedu)
	ON_CBN_SELCHANGE(IDC_COMBO_XIANXING, &CCurvePage::OnCbnSelchangeComboXianxing)
	ON_CBN_SELCHANGE(IDC_COMBO_XIANKUAN, &CCurvePage::OnCbnSelchangeComboXiankuan)
	ON_CBN_SELCHANGE(IDC_COMBO_HUIRAOMOSHI, &CCurvePage::OnCbnSelchangeComboHuiraomoshi)
	ON_EN_CHANGE(IDC_EDIT_HUIRAOCISHU, &CCurvePage::OnEnChangeEditHuiraocishu)
	ON_EN_CHANGE(IDC_EDIT_YOUBIANJIE, &CCurvePage::OnEnChangeEditYoubianjie)
	ON_EN_CHANGE(IDC_EDIT_ZUOBIANJIE, &CCurvePage::OnEnChangeEditZuobianjie)
END_MESSAGE_MAP()


// CCurvePage ��Ϣ�������


BOOL CCurvePage::OnInitDialog()
{
	CPropertyPage::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	spinEdit.SetBuddy(GetDlgItem(IDC_EDIT_HUIRAOCISHU));  //���û�鴰��
	spinEdit.SetRange(0, 100);  //����min~max

	CWnd::SetWindowText(theApp.GetResString(IDS_CURVE_TAB1));

	((CComboBox*)GetDlgItem(IDC_COMBO_BIAOSHI))->ResetContent();
	((CComboBox*)GetDlgItem(IDC_COMBO_SHUJU))->ResetContent();

	((CComboBox*)GetDlgItem(IDC_COMBO_GUIDAO))->InsertString(0,_T("Track 1"));
	((CComboBox*)GetDlgItem(IDC_COMBO_GUIDAO))->InsertString(1,_T("Depth Track"));
	((CComboBox*)GetDlgItem(IDC_COMBO_GUIDAO))->InsertString(2,_T("Track 23"));

	((CComboBox*)GetDlgItem(IDC_COMBO_KEDU))->InsertString(0,_T("����"));
	((CComboBox*)GetDlgItem(IDC_COMBO_KEDU))->InsertString(1,_T("����"));

	((CComboBox*)GetDlgItem(IDC_COMBO_XIANXING))->InsertString(0,_T("ʵ��"));//PS_SOLID /* ��*/
	((CComboBox*)GetDlgItem(IDC_COMBO_XIANXING))->InsertString(1,_T("����"));//PS_DASH  /* -------  */
	((CComboBox*)GetDlgItem(IDC_COMBO_XIANXING))->InsertString(2,_T("����"));//PS_DOT  /* .......  */
	((CComboBox*)GetDlgItem(IDC_COMBO_XIANXING))->InsertString(3,_T("�㻮��"));//PS_DASHDOT  /* _._._._  */
	((CComboBox*)GetDlgItem(IDC_COMBO_XIANXING))->InsertString(4,_T("˫�㻮��"));//PS_DASHDOTDOT  /* _.._.._  */
	//((CComboBox*)GetDlgItem(IDC_COMBO_XIANXING))->InsertString(5,_T("����5"));
	
	((CComboBox*)GetDlgItem(IDC_COMBO_XIANKUAN))->InsertString(0,_T("0"));
	((CComboBox*)GetDlgItem(IDC_COMBO_XIANKUAN))->InsertString(1,_T("1"));
	((CComboBox*)GetDlgItem(IDC_COMBO_XIANKUAN))->InsertString(2,_T("2"));
	((CComboBox*)GetDlgItem(IDC_COMBO_XIANKUAN))->InsertString(3,_T("3"));
	((CComboBox*)GetDlgItem(IDC_COMBO_XIANKUAN))->InsertString(4,_T("4"));
	((CComboBox*)GetDlgItem(IDC_COMBO_XIANKUAN))->InsertString(5,_T("5"));

	((CComboBox*)GetDlgItem(IDC_COMBO_HUIRAOMOSHI))->InsertString(0,_T("�����"));
	((CComboBox*)GetDlgItem(IDC_COMBO_HUIRAOMOSHI))->InsertString(1,_T("�һ���"));
	((CComboBox*)GetDlgItem(IDC_COMBO_HUIRAOMOSHI))->InsertString(2,_T("���Ҷ�����"));
	((CComboBox*)GetDlgItem(IDC_COMBO_HUIRAOMOSHI))->InsertString(3,_T("������"));
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
		spinEdit.SetPos(1);  //������ʼλ��
	}
	else
	{
		//����theApp.curveList
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
	// �쳣: OCX ����ҳӦ���� FALSE
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
	pCurList = plist;
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
	

	((CComboBox*)GetDlgItem(IDC_COMBO_XIANKUAN))->SetCurSel(GetCurLineIndex(plist->lineWidth));
	((CComboBox*)GetDlgItem(IDC_COMBO_HUIRAOMOSHI))->SetCurSel(GetCurLineIndex(plist->wrapMode));

	str.Format(_T("%d"),plist->wrapCount);
	combHuiRaoCiShu.SetWindowText(str);
	spinEdit.SetPos(plist->wrapCount);  //������ʼλ��
	return true;
}

int CCurvePage::GetCurLineIndex(int lineWidth)
{
	if(lineWidth == 0)
	{
		return 0;
	} else if(lineWidth == 1)
	{
		return 1;
	} else if(lineWidth == 2)
	{
		return 2;
	} else if(lineWidth == 3)
	{
		return 3;
	} else if(lineWidth == 4)
	{
		return 4;
	} else if(lineWidth == 5)
	{
		return 5;
	}
}

void CCurvePage::DeleteCurveInfo( )
{
	int item = combBiaoShi.GetCurSel();
	if(item < 0)
	{
		return;
	}
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
		MessageBox(_T("��������������"),_T("��ʾ"),MB_ICONQUESTION|MB_OK);
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
				MessageBox(_T("�����Ѵ��ڣ����������룡"),_T("��ʾ"),MB_ICONQUESTION|MB_OK);
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

	//pCurveInfo->lineWidth = combXianKuan.GetCurSel();
	int index = combXianKuan.GetCurSel();
	combXianKuan.GetLBText(index, str);
	pCurveInfo->lineWidth = _wtoi(str);

	pCurveInfo->wrapMode = combHuiRaoMoShi.GetCurSel();

	combHuiRaoCiShu.GetWindowText(str);
	pCurveInfo->wrapCount = _wtoi(str);
	spinEdit.SetPos(pCurveInfo->wrapCount);
}
void CCurvePage::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
}


void CCurvePage::OnBnClickedCancel()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
}


void CCurvePage::OnBnClickedButtonAdd()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	InseertCurveInfo();
}


void CCurvePage::OnBnClickedButtonDel()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	DeleteCurveInfo();
}


void CCurvePage::OnBnClickedButtonColor()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CColorDialog dlg;

	if (dlg.DoModal() == IDOK)
	{
		curveSelectColor = dlg.GetColor();
		mStaticColor.SetStaiticColor(curveSelectColor);
		if(pCurList != NULL)
		{
			pCurList->curveColor = curveSelectColor;
		}
	}
}


void CCurvePage::OnCbnSelchangeComboBiaoshi()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	int item = ((CComboBox*)GetDlgItem(IDC_COMBO_BIAOSHI))->GetCurSel();
	((CComboBox*)GetDlgItem(IDC_COMBO_SHUJU))->SetCurSel(item);
	SetCurveData(item);
}


void CCurvePage::OnCbnSelchangeComboShuju()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	int item = ((CComboBox*)GetDlgItem(IDC_COMBO_SHUJU))->GetCurSel();
	((CComboBox*)GetDlgItem(IDC_COMBO_BIAOSHI))->SetCurSel(item);
	SetCurveData(item);
}

void CCurvePage::SetCurveData(int index)
{
	CString str;
	CCurveInfo* plist = theApp.curveList.GetAt(theApp.curveList.FindIndex(index));
	pCurList = plist;
	combGuiDao.SetCurSel(plist->curveTrack);

	str.Format(_T("%d"),plist->leftLimit);
	editZuoBianJie.SetWindowText(str);
	str.Format(_T("%d"),plist->rightLimit);
	editYouBianJie.SetWindowText(str);

	curveSelectColor = plist->curveColor;
	mStaticColor.SetStaiticColor(curveSelectColor);

	combKeDu.SetCurSel(plist->graduation);
	combXianXing.SetCurSel(plist->lineType);
	combXianKuan.SetCurSel(GetCurLineIndex(plist->lineWidth));
	combHuiRaoMoShi.SetCurSel(plist->wrapMode);

	str.Format(_T("%d"),plist->wrapCount);
	combHuiRaoCiShu.SetWindowText(str);
	spinEdit.SetPos(plist->wrapCount);
}


void CCurvePage::OnCbnSelchangeComboGuidao()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if(pCurList != NULL)
	{
		pCurList->curveTrack = combGuiDao.GetCurSel();
	}
}


void CCurvePage::OnCbnSelchangeComboKedu()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if(pCurList != NULL)
	{
		pCurList->graduation = combKeDu.GetCurSel();
	}
}


void CCurvePage::OnCbnSelchangeComboXianxing()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if(pCurList != NULL)
	{
		pCurList->lineType = combXianXing.GetCurSel();
	}
}


void CCurvePage::OnCbnSelchangeComboXiankuan()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if(pCurList != NULL)
	{
		CString str;
		int index = combXianKuan.GetCurSel();
		combXianKuan.GetLBText(index, str);
		pCurList->lineWidth = _wtoi(str);
	}
}


void CCurvePage::OnCbnSelchangeComboHuiraomoshi()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if(pCurList != NULL)
	{
		pCurList->wrapMode = combHuiRaoMoShi.GetCurSel();
	}
}


void CCurvePage::OnEnChangeEditHuiraocishu()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
	// ���ʹ�֪ͨ��������д CPropertyPage::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	if(pCurList != NULL)
	{
		CString str;
		combHuiRaoCiShu.GetWindowText(str);
		pCurList->wrapCount = _wtoi(str);
	}
}


void CCurvePage::OnEnChangeEditYoubianjie()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
	// ���ʹ�֪ͨ��������д CPropertyPage::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	if(pCurList != NULL)
	{
		CString str;
		editYouBianJie.GetWindowText(str);
		pCurList->rightLimit = _wtoi(str);
	}
}


void CCurvePage::OnEnChangeEditZuobianjie()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
	// ���ʹ�֪ͨ��������д CPropertyPage::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	if(pCurList != NULL)
	{
		CString str;
		editZuoBianJie.GetWindowText(str);
		pCurList->leftLimit = _wtoi(str);
	}
}
