// CurveSheet.cpp : 实现文件
//

#include "stdafx.h"
#include "PlotEditor.h"
#include "CurveSheet.h"


// CCurveSheet

IMPLEMENT_DYNAMIC(CCurveSheet, CPropertySheet)

CCurveSheet::CCurveSheet(UINT nIDCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(nIDCaption, pParentWnd, iSelectPage)
{
	AddControlPages();
}

CCurveSheet::CCurveSheet(LPCTSTR pszCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(pszCaption, pParentWnd, iSelectPage)
{
	AddControlPages();
}

CCurveSheet::~CCurveSheet()
{
}


BEGIN_MESSAGE_MAP(CCurveSheet, CPropertySheet)
END_MESSAGE_MAP()


// CCurveSheet 消息处理程序


BOOL CCurveSheet::OnInitDialog()
{
	BOOL bResult = CPropertySheet::OnInitDialog();

	// TODO:  在此添加您的专用代码
	
	CWnd *pWnd = GetDlgItem (ID_APPLY_NOW);
	pWnd->ShowWindow (FALSE);
	pWnd = GetDlgItem (IDHELP);
	pWnd->ShowWindow (FALSE);
	int ids[] = { IDOK, IDCANCEL };
	CRect rectWnd;
	CRect rectBtn;
	GetWindowRect (rectWnd);
	GetDlgItem (IDOK)->GetWindowRect (rectBtn);
	int btnWidth = rectBtn.Width();
	int btnHeight = rectBtn.Height();
	int btnOffset = rectWnd.bottom - rectBtn.bottom;
	int btnLeft = rectWnd.right - rectWnd.left;
	rectWnd.bottom = rectBtn.top;
	rectWnd.right = rectWnd.right + btnWidth + btnOffset;
	MoveWindow(rectWnd);
	rectBtn.left = btnLeft;
	rectBtn.right = btnLeft + btnWidth;
	for (int i = 0; i < sizeof (ids) / sizeof (int); i++)
	{
		rectBtn.top = (i + 1) * btnOffset + btnHeight * i;
		rectBtn.bottom = rectBtn.top + btnHeight;
		GetDlgItem (ids [i])->MoveWindow (rectBtn);
	}
	return bResult;
}

void CCurveSheet::AddControlPages()
{
    m_psh.dwFlags &= (~PSH_HASHELP);
    AddPage(&m_PageCurve);
}
