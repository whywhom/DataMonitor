// DrawFormView.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "DataMonitor.h"
#include "DrawFormView.h"


// CDrawFormView

IMPLEMENT_DYNCREATE(CDrawFormView, CFormView)

CDrawFormView::CDrawFormView()
	: CFormView(CDrawFormView::IDD)
{

}

CDrawFormView::~CDrawFormView()
{
}

void CDrawFormView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CDrawFormView, CFormView)
END_MESSAGE_MAP()


// CDrawFormView ���

#ifdef _DEBUG
void CDrawFormView::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CDrawFormView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CDrawFormView ��Ϣ�������
