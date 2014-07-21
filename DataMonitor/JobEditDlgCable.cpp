// JobEditDlgCable.cpp : 实现文件
//

#include "stdafx.h"
#include "DataMonitor.h"
#include "JobEditDlgCable.h"
#include "afxdialogex.h"


// CJobEditDlgCable 对话框

IMPLEMENT_DYNAMIC(CJobEditDlgCable, CDialog)

CJobEditDlgCable::CJobEditDlgCable(CWnd* pParent /*=NULL*/)
	: CDialog(CJobEditDlgCable::IDD, pParent)
	, m_Wire1(_T(""))
	, m_Panel1(_T(""))
	, m_Wire2(_T(""))
	, m_Panel2(_T(""))
	, m_Wire3(_T(""))
	, m_Wire4(_T(""))
	, m_Wire5(_T(""))
	, m_Wire6(_T(""))
	, m_Wire7(_T(""))
	, m_Wire8(_T(""))
	, m_Wire9(_T(""))
	, m_Wire10(_T(""))
	, m_Panel3(_T(""))
	, m_Panel4(_T(""))
	, m_Panel5(_T(""))
	, m_Panel6(_T(""))
	, m_Panel7(_T(""))
	, m_Panel8(_T(""))
	, m_Panel9(_T(""))
	, m_Panel10(_T(""))
{
}

CJobEditDlgCable::~CJobEditDlgCable()
{
}

void CJobEditDlgCable::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_ZCW_CABLE_EDIT1, m_Wire1);
	DDV_MaxChars(pDX, m_Wire1, 50);
	DDX_Text(pDX, IDC_ZCW_CABLE_EDIT11, m_Panel1);
	DDV_MaxChars(pDX, m_Panel1, 50);
	DDX_Text(pDX, IDC_ZCW_CABLE_EDIT2, m_Wire2);
	DDV_MaxChars(pDX, m_Wire2, 50);
	DDX_Text(pDX, IDC_ZCW_CABLE_EDIT12, m_Panel2);
	DDV_MaxChars(pDX, m_Panel2, 50);
	DDX_Text(pDX, IDC_ZCW_CABLE_EDIT3, m_Wire3);
	DDV_MaxChars(pDX, m_Wire3, 50);
	DDX_Text(pDX, IDC_ZCW_CABLE_EDIT4, m_Wire4);
	DDV_MaxChars(pDX, m_Wire4, 50);
	DDX_Text(pDX, IDC_ZCW_CABLE_EDIT5, m_Wire5);
	DDV_MaxChars(pDX, m_Wire5, 50);
	DDX_Text(pDX, IDC_ZCW_CABLE_EDIT6, m_Wire6);
	DDV_MaxChars(pDX, m_Wire6, 50);
	DDX_Text(pDX, IDC_ZCW_CABLE_EDIT7, m_Wire7);
	DDV_MaxChars(pDX, m_Wire7, 50);
	DDX_Text(pDX, IDC_ZCW_CABLE_EDIT8, m_Wire8);
	DDV_MaxChars(pDX, m_Wire8, 50);
	DDX_Text(pDX, IDC_ZCW_CABLE_EDIT9, m_Wire9);
	DDV_MaxChars(pDX, m_Wire9, 50);
	DDX_Text(pDX, IDC_ZCW_CABLE_EDIT10, m_Wire10);
	DDV_MaxChars(pDX, m_Wire10, 50);
	DDX_Text(pDX, IDC_ZCW_CABLE_EDIT13, m_Panel3);
	DDV_MaxChars(pDX, m_Panel3, 50);
	DDX_Text(pDX, IDC_ZCW_CABLE_EDIT14, m_Panel4);
	DDV_MaxChars(pDX, m_Panel4, 50);
	DDX_Text(pDX, IDC_ZCW_CABLE_EDIT15, m_Panel5);
	DDV_MaxChars(pDX, m_Panel5, 50);
	DDX_Text(pDX, IDC_ZCW_CABLE_EDIT16, m_Panel6);
	DDV_MaxChars(pDX, m_Panel6, 50);
	DDX_Text(pDX, IDC_ZCW_CABLE_EDIT17, m_Panel7);
	DDV_MaxChars(pDX, m_Panel7, 50);
	DDX_Text(pDX, IDC_ZCW_CABLE_EDIT18, m_Panel8);
	DDV_MaxChars(pDX, m_Panel8, 50);
	DDX_Text(pDX, IDC_ZCW_CABLE_EDIT19, m_Panel9);
	DDV_MaxChars(pDX, m_Panel9, 50);
	DDX_Text(pDX, IDC_ZCW_CABLE_EDIT20, m_Panel10);
	DDV_MaxChars(pDX, m_Panel10, 50);
}


BEGIN_MESSAGE_MAP(CJobEditDlgCable, CDialog)
	ON_EN_CHANGE(IDC_ZCW_CABLE_EDIT1, &CJobEditDlgCable::OnEnChangeEdit1)
	ON_EN_CHANGE(IDC_ZCW_CABLE_EDIT2, &CJobEditDlgCable::OnEnChangeEdit2)
	ON_EN_CHANGE(IDC_ZCW_CABLE_EDIT3, &CJobEditDlgCable::OnEnChangeEdit3)
	ON_EN_CHANGE(IDC_ZCW_CABLE_EDIT4, &CJobEditDlgCable::OnEnChangeEdit4)
	ON_EN_CHANGE(IDC_ZCW_CABLE_EDIT5, &CJobEditDlgCable::OnEnChangeEdit5)
	ON_EN_CHANGE(IDC_ZCW_CABLE_EDIT6, &CJobEditDlgCable::OnEnChangeEdit6)
	ON_EN_CHANGE(IDC_ZCW_CABLE_EDIT7, &CJobEditDlgCable::OnEnChangeEdit7)
	ON_EN_CHANGE(IDC_ZCW_CABLE_EDIT8, &CJobEditDlgCable::OnEnChangeEdit8)
	ON_EN_CHANGE(IDC_ZCW_CABLE_EDIT9, &CJobEditDlgCable::OnEnChangeEdit9)
	ON_EN_CHANGE(IDC_ZCW_CABLE_EDIT10, &CJobEditDlgCable::OnEnChangeEdit10)
	ON_EN_CHANGE(IDC_ZCW_CABLE_EDIT11, &CJobEditDlgCable::OnEnChangeEdit11)
	ON_EN_CHANGE(IDC_ZCW_CABLE_EDIT12, &CJobEditDlgCable::OnEnChangeEdit12)
	ON_EN_CHANGE(IDC_ZCW_CABLE_EDIT13, &CJobEditDlgCable::OnEnChangeEdit13)
	ON_EN_CHANGE(IDC_ZCW_CABLE_EDIT14, &CJobEditDlgCable::OnEnChangeEdit14)
	ON_EN_CHANGE(IDC_ZCW_CABLE_EDIT15, &CJobEditDlgCable::OnEnChangeEdit15)
	ON_EN_CHANGE(IDC_ZCW_CABLE_EDIT16, &CJobEditDlgCable::OnEnChangeEdit16)
	ON_EN_CHANGE(IDC_ZCW_CABLE_EDIT17, &CJobEditDlgCable::OnEnChangeEdit17)
	ON_EN_CHANGE(IDC_ZCW_CABLE_EDIT18, &CJobEditDlgCable::OnEnChangeEdit18)
	ON_EN_CHANGE(IDC_ZCW_CABLE_EDIT19, &CJobEditDlgCable::OnEnChangeEdit19)
	ON_EN_CHANGE(IDC_ZCW_CABLE_EDIT20, &CJobEditDlgCable::OnEnChangeEdit20)
END_MESSAGE_MAP()


// CJobEditDlgCable 消息处理程序


void CJobEditDlgCable::OnEnChangeEdit1()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialog::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
	UpdateData();
}


void CJobEditDlgCable::OnEnChangeEdit2()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialog::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
	UpdateData();
}


void CJobEditDlgCable::OnEnChangeEdit3()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialog::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
	UpdateData();
}


void CJobEditDlgCable::OnEnChangeEdit4()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialog::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
	UpdateData();
}


void CJobEditDlgCable::OnEnChangeEdit5()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialog::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
	UpdateData();
}


void CJobEditDlgCable::OnEnChangeEdit6()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialog::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
	UpdateData();
}


void CJobEditDlgCable::OnEnChangeEdit7()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialog::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
	UpdateData();
}


void CJobEditDlgCable::OnEnChangeEdit8()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialog::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
	UpdateData();
}


void CJobEditDlgCable::OnEnChangeEdit9()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialog::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
	UpdateData();
}


void CJobEditDlgCable::OnEnChangeEdit10()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialog::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
	UpdateData();
}


void CJobEditDlgCable::OnEnChangeEdit11()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialog::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
	UpdateData();
}


void CJobEditDlgCable::OnEnChangeEdit12()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialog::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
	UpdateData();
}


void CJobEditDlgCable::OnEnChangeEdit13()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialog::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
	UpdateData();
}


void CJobEditDlgCable::OnEnChangeEdit14()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialog::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
	UpdateData();
}


void CJobEditDlgCable::OnEnChangeEdit15()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialog::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
	UpdateData();
}


void CJobEditDlgCable::OnEnChangeEdit16()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialog::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
	UpdateData();
}


void CJobEditDlgCable::OnEnChangeEdit17()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialog::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
	UpdateData();
}


void CJobEditDlgCable::OnEnChangeEdit18()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialog::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
	UpdateData();
}


void CJobEditDlgCable::OnEnChangeEdit19()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialog::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
	UpdateData();
}


void CJobEditDlgCable::OnEnChangeEdit20()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialog::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
	UpdateData();
}
