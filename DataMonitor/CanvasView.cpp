// CanvasView.cpp : 实现文件
//

#include "stdafx.h"
#include "DataMonitor.h"
#include "CanvasView.h"


// CCanvasView

IMPLEMENT_DYNCREATE(CCanvasView, CView)

CCanvasView::CCanvasView()
{

}

CCanvasView::~CCanvasView()
{
}

BEGIN_MESSAGE_MAP(CCanvasView, CView)
END_MESSAGE_MAP()


// CCanvasView 绘图

void CCanvasView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: 在此添加绘制代码
}


// CCanvasView 诊断

#ifdef _DEBUG
void CCanvasView::AssertValid() const
{
	CView::AssertValid();
}

#ifndef _WIN32_WCE
void CCanvasView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif
#endif //_DEBUG


// CCanvasView 消息处理程序
