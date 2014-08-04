#pragma once



// CCurveSheet
#include "CurvePage.h"
class CCurveSheet : public CPropertySheet
{
	DECLARE_DYNAMIC(CCurveSheet)
public:
	CCurvePage m_PageCurve;
public:
	CCurveSheet(UINT nIDCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	CCurveSheet(LPCTSTR pszCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	virtual ~CCurveSheet();

	void AddControlPages();
protected:
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
};


