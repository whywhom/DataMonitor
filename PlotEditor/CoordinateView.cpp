// CoordinateView.cpp : 实现文件
//

#include "stdafx.h"
#include "PlotEditor.h"
#include "CoordinateView.h"


// CCoordinateView

IMPLEMENT_DYNCREATE(CCoordinateView, CScrollView)

CCoordinateView::CCoordinateView()
{

}

CCoordinateView::~CCoordinateView()
{
}

BEGIN_MESSAGE_MAP(CCoordinateView, CScrollView)
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
	CScrollView::AssertValid();
}

#ifndef _WIN32_WCE
void CCoordinateView::Dump(CDumpContext& dc) const
{
	CScrollView::Dump(dc);
}
#endif
#endif //_DEBUG


// CCoordinateView 消息处理程序


void CCoordinateView::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	CScrollView::OnLButtonDblClk(nFlags, point);
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
	CScrollView::OnSize(nType, cx, cy);

	// TODO: 在此处添加消息处理程序代码
}
void CCoordinateView::DrawCoordinateSystem(CDC* pDC)
{
	TRACE(_T("DrawCoordinateSystem"));
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

	 LOGBRUSH logBrush;

	int i = 0;
	int gap = 40;
	int iDrawType = PS_SOLID;
	int iLineWidth = 1;
	COLORREF colorR = RGB(127,127,127);
	int count = theApp.curveList.GetCount();
	if(count > 0)
	{
		CPen pen(iDrawType, 1, colorR); //画笔
		CPen* pOldPen = pDC->SelectObject(&pen);//画笔和画线区连接
		POSITION pos = theApp.curveList.GetHeadPosition();
		while(pos)
		{
			CRect rectTop,rectBottom;
			CCurveInfo* plist = theApp.curveList.GetNext(pos);
			//COLORREF colorR = plist->curveColor;
			//iDrawType = plist->lineType;
			//iLineWidth = plist->lineWidth;
			memset(&logBrush, 0, sizeof(logBrush));
			logBrush.lbStyle = BS_SOLID;
			logBrush.lbColor = plist->curveColor;
			DWORD dwF[] = {5, 10, 15, 20};
			//画线
			CPen pen;//(iDrawType, iLineWidth, colorR); //画笔
			//pen.CreatePen(PS_USERSTYLE|PS_GEOMETRIC|PS_ENDCAP_ROUND, plist->lineWidth, &logBrush,4,dwF);
			pen.CreatePen(plist->lineType|PS_GEOMETRIC|PS_ENDCAP_ROUND, plist->lineWidth, &logBrush);
			pDC->SelectObject(&pen);//画笔和画线区连接
			
			rectTop = rect;
			rectTop.top = gap*i+1;
			rectTop.bottom = gap*i+gap/2-1;

			rectBottom = rect;
			rectBottom.top = gap*i+gap/2+1;
			rectBottom.bottom = gap*i+gap-1;
			
			pDC->MoveTo(0, gap*i+gap/2);
			pDC->LineTo(rect.Width(), gap*i+gap/2);

			//画边框
			CPen pen2(PS_SOLID, 1, RGB(0,0,0)); //画笔
			pDC->SelectObject(&pen2);//画笔和画线区连接

			pDC->MoveTo(0, rectBottom.bottom);
			pDC->LineTo(rect.Width(), rectBottom.bottom);
			//画标注
			pDC->SelectObject(&font);
			pDC->SetTextColor(colorR);   //文本字体颜色为蓝色
            //pDC->TextOut(rect.Width()/2,30*i,plist->strCurveName);   //输出文本值
			pDC->DrawText(plist->strCurveName,rectTop,DT_CENTER);

			pDC->SetTextAlign(TA_LEFT);
			str.Format(_T("%d"),plist->leftLimit);
			//pDC->TextOut(1,30*i+16,str);   //输出文本值
			pDC->DrawText(str,rectBottom,DT_LEFT);

			pDC->SetTextAlign(TA_RIGHT);
			str.Format(_T("%d"),plist->rightLimit);
			//pDC->TextOut(rect.Width()-100,30*i+16,str);   //输出文本值
			pDC->DrawText(str,rectBottom,DT_RIGHT);
			i++;
		}
		rectTotal = rect;
		rectTotal.bottom = rectTotal.top + gap*i;
		SetScrollTotalSizes(rectTotal);
		//画笔刷新
		pDC->SelectObject(pOldPen);  //
	}
}


void CCoordinateView::OnInitialUpdate()
{
	CScrollView::OnInitialUpdate();

	// TODO: 在此添加专用代码和/或调用基类
	SIZE sizeTotal;
	sizeTotal.cx = rect.Width();
	sizeTotal.cy = rect.Height();

    SetScrollSizes(MM_TEXT, sizeTotal);
}

void CCoordinateView::SetScrollTotalSizes(CRect rect)
{
	SIZE sizeTotal;
	sizeTotal.cx = rect.Width();
	sizeTotal.cy = rect.Height();
	SetScrollSizes(MM_TEXT, sizeTotal);
}
