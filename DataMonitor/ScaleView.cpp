// ScaleView.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "DataMonitor.h"
#include "ScaleView.h"


// CScaleView

IMPLEMENT_DYNCREATE(CScaleView, CView)

CScaleView::CScaleView()
{

}

CScaleView::~CScaleView()
{
}

BEGIN_MESSAGE_MAP(CScaleView, CView)
END_MESSAGE_MAP()


// CScaleView ��ͼ

void CScaleView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: �ڴ���ӻ��ƴ���
}


// CScaleView ���

#ifdef _DEBUG
void CScaleView::AssertValid() const
{
	CView::AssertValid();
}

#ifndef _WIN32_WCE
void CScaleView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif
#endif //_DEBUG


// CScaleView ��Ϣ�������
