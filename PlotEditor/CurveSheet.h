#pragma once



// CCurveSheet
#include "CurvePage.h"
#include "..\DataMonitor\tinyxml.h"

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
	int CreateCurveFile(CString strFile);
	void LinkElementeFuns(TiXmlElement * element,CCurveInfo* plist);
protected:
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnIdok();
	afx_msg void OnIdno();
	afx_msg void OnIdcancel();
};


