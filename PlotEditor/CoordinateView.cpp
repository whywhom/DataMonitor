// CoordinateView.cpp : 实现文件
//

#include "stdafx.h"
#include "PlotEditor.h"
#include "CoordinateView.h"


// CCoordinateView

IMPLEMENT_DYNCREATE(CCoordinateView, CView)

CCoordinateView::CCoordinateView()
{

}

CCoordinateView::~CCoordinateView()
{
}

BEGIN_MESSAGE_MAP(CCoordinateView, CView)
	ON_WM_LBUTTONDBLCLK()
	ON_WM_PAINT()
	ON_WM_SIZE()
END_MESSAGE_MAP()


// CCoordinateView 绘图

void CCoordinateView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: 在此添加绘制代码
	DrawCoordinateSystem(pDC);
}


// CCoordinateView 诊断

#ifdef _DEBUG
void CCoordinateView::AssertValid() const
{
	CView::AssertValid();
}

#ifndef _WIN32_WCE
void CCoordinateView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif
#endif //_DEBUG


// CCoordinateView 消息处理程序


void CCoordinateView::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	CView::OnLButtonDblClk(nFlags, point);
}


void CCoordinateView::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: 在此处添加消息处理程序代码
	// 不为绘图消息调用 CView::OnPaint()
	OnPrepareDC(&dc);
	OnDraw(&dc);
}


void CCoordinateView::OnSize(UINT nType, int cx, int cy)
{
	CView::OnSize(nType, cx, cy);

	// TODO: 在此处添加消息处理程序代码
}
void CCoordinateView::DrawCoordinateSystem(CDC* pDC)
{
	TRACE(_T("DrawCoordinateSystem"));
	CRect rect;
	GetClientRect(rect);
	UpdateData(FALSE);
	CString str;
	CFont font;
	VERIFY(font.CreateFont(
		14,                        // nHeight
		0,                         // nWidth
		0,                         // nEscapement
		0,                         // nOrientation
		FW_NORMAL,                 // nWeight
		FALSE,                     // bItalic
		FALSE,                     // bUnderline
		0,                         // cStrikeOut
		ANSI_CHARSET,              // nCharSet
		OUT_DEFAULT_PRECIS,        // nOutPrecision
		CLIP_DEFAULT_PRECIS,       // nClipPrecision
		DEFAULT_QUALITY,           // nQuality
		DEFAULT_PITCH | FF_SWISS,  // nPitchAndFamily
		_T("Arial")));                 // lpszFacename

	int i = 0;
	int iDrawType = PS_SOLID;
	COLORREF colorR = RGB(127,127,127);
	int count = theApp.curveList.GetCount();
	if(count > 0)
	{
		CPen pen(iDrawType, 1, colorR); //画笔
		CPen* pOldPen = pDC->SelectObject(&pen);//画笔和画线区连接
		POSITION pos = theApp.curveList.GetHeadPosition();
		while(pos)
		{
			CCurveInfo* plist = theApp.curveList.GetNext(pos);
			COLORREF colorR = plist->curveColor;
			iDrawType = plist->lineType;
			CPen pen(iDrawType, 1, colorR); //画笔
			pDC->SelectObject(&pen);//画笔和画线区连接
			
			pDC->MoveTo(0, 30*i+15);
			pDC->LineTo(rect.Width(), 30*i+15);

			pDC->SelectObject(&font);
			pDC->SetTextColor(colorR);   //文本字体颜色为蓝色
            pDC->TextOut(rect.Width()/2,30*i,plist->strCurveName);   //输出文本值

			str.Format(_T("%d"),plist->leftLimit);
			pDC->TextOut(1,30*i+16,str);   //输出文本值
			str.Format(_T("%d"),plist->rightLimit);
			pDC->TextOut(rect.Width()-30,30*i+16,str);   //输出文本值
			i++;
		}
		//画笔刷新
		pDC->SelectObject(pOldPen);  //[可以不需要]
	}
}
