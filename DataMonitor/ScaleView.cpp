// ScaleView.cpp : 实现文件
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


// CScaleView 绘图

void CScaleView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: 在此添加绘制代码
}


// CScaleView 诊断

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


// CScaleView 消息处理程序
