// CoordinateView.cpp : ʵ���ļ�
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


// CCoordinateView ��ͼ

void CCoordinateView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: �ڴ���ӻ��ƴ���
	DrawCoordinateSystem(pDC);
}


// CCoordinateView ���

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


// CCoordinateView ��Ϣ�������


void CCoordinateView::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ

	CScrollView::OnLButtonDblClk(nFlags, point);
}


void CCoordinateView::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: �ڴ˴������Ϣ����������
	// ��Ϊ��ͼ��Ϣ���� CView::OnPaint()
	OnPrepareDC(&dc);
	OnDraw(&dc);
}


void CCoordinateView::OnSize(UINT nType, int cx, int cy)
{
	CScrollView::OnSize(nType, cx, cy);

	// TODO: �ڴ˴������Ϣ����������
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
		CPen pen(iDrawType, 1, colorR); //����
		CPen* pOldPen = pDC->SelectObject(&pen);//���ʺͻ���������
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
			//����
			CPen pen;//(iDrawType, iLineWidth, colorR); //����
			//pen.CreatePen(PS_USERSTYLE|PS_GEOMETRIC|PS_ENDCAP_ROUND, plist->lineWidth, &logBrush,4,dwF);
			pen.CreatePen(plist->lineType|PS_GEOMETRIC|PS_ENDCAP_SQUARE, plist->lineWidth, &logBrush);
			pDC->SelectObject(&pen);//���ʺͻ���������
			
			rectTop = rect;
			rectTop.top = gap*i+1;
			rectTop.bottom = gap*i+gap/2-1;

			rectBottom = rect;
			rectBottom.top = gap*i+gap/2+1;
			rectBottom.bottom = gap*i+gap-1;
			
			pDC->MoveTo(0, gap*i+gap/2);
			pDC->LineTo(rect.Width(), gap*i+gap/2);

			//���߿�
			CPen pen2(PS_SOLID, 1, RGB(0,0,0)); //����
			pDC->SelectObject(&pen2);//���ʺͻ���������

			pDC->MoveTo(0, rectBottom.bottom);
			pDC->LineTo(rect.Width(), rectBottom.bottom);

			pDC->MoveTo(rect.Width(), rectTop.top);
			pDC->LineTo(rect.Width(), rectBottom.bottom);
			//����ע
			pDC->SelectObject(&font);
			pDC->SetTextColor(colorR);   //�ı�������ɫΪ��ɫ
            //pDC->TextOut(rect.Width()/2,30*i,plist->strCurveName);   //����ı�ֵ
			pDC->DrawText(plist->strCurveName,rectTop,DT_CENTER);

			pDC->SetTextAlign(TA_LEFT);
			str.Format(_T("%d"),plist->leftLimit);
			//pDC->TextOut(1,30*i+16,str);   //����ı�ֵ
			pDC->DrawText(str,rectBottom,DT_LEFT);

			pDC->SetTextAlign(TA_RIGHT);
			str.Format(_T("%d"),plist->rightLimit);
			//pDC->TextOut(rect.Width()-100,30*i+16,str);   //����ı�ֵ
			pDC->DrawText(str,rectBottom,DT_RIGHT);
			i++;
		}
		rectTotal = rect;
		rectTotal.bottom = rectTotal.top + gap*i;
		SetScrollTotalSizes(rectTotal);
		//����ˢ��
		pDC->SelectObject(pOldPen);  //
	}
}


void CCoordinateView::OnInitialUpdate()
{
	CScrollView::OnInitialUpdate();

	// TODO: �ڴ����ר�ô����/����û���
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
