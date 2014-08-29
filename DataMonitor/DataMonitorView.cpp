
// DMonitorView.cpp : CDataMonitorView 类的实现
//

#include "stdafx.h"
// SHARED_HANDLERS 可以在实现预览、缩略图和搜索筛选器句柄的
// ATL 项目中进行定义，并允许与该项目共享文档代码。
#ifndef SHARED_HANDLERS
#include "DataMonitor.h"
#endif

#include "DataMonitorDoc.h"
#include "DataMonitorView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CDataMonitorView

IMPLEMENT_DYNCREATE(CDataMonitorView, CScrollView)

BEGIN_MESSAGE_MAP(CDataMonitorView, CScrollView)
	// 标准打印命令
	ON_COMMAND(ID_FILE_PRINT, &CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CScrollView::OnFilePrintPreview)
	ON_WM_PAINT()
	ON_WM_VSCROLL()
	ON_WM_TIMER()
	ON_WM_CREATE()
END_MESSAGE_MAP()

// CDataMonitorView 构造/析构

CDataMonitorView::CDataMonitorView()
{
	// TODO: 在此处添加构造代码
	tempArray[0] = 0;
	tempArray[1] = 20000;

	gmArray[0] = 0;
	gmArray[1] = 20000;

}

CDataMonitorView::~CDataMonitorView()
{
}

BOOL CDataMonitorView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式
	
	return CScrollView::PreCreateWindow(cs);
}

// CDataMonitorView 绘制

void CDataMonitorView::OnDraw(CDC* pDC)
{
	CDataMonitorDoc* pDoc = GetDocument();
	//ASSERT_VALID(pDoc);
	//if (!pDoc)
	//	return;

	// TODO: 在此处为本机数据添加绘制代码
	DrawData(pDC);
}


// CDataMonitorView 打印

BOOL CDataMonitorView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 默认准备
	return DoPreparePrinting(pInfo);
}

void CDataMonitorView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加额外的打印前进行的初始化过程
}

void CDataMonitorView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加打印后进行的清理过程
}


// CDataMonitorView 诊断

#ifdef _DEBUG
void CDataMonitorView::AssertValid() const
{
	CScrollView::AssertValid();
}

void CDataMonitorView::Dump(CDumpContext& dc) const
{
	CScrollView::Dump(dc);
}

CDataMonitorDoc* CDataMonitorView::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CDataMonitorDoc)));
	return (CDataMonitorDoc*)m_pDocument;
}
#endif //_DEBUG


// CDataMonitorView 消息处理程序


void CDataMonitorView::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: 在此处添加消息处理程序代码
	// 不为绘图消息调用 CScrollView::OnPaint()
	OnPrepareDC(&dc);
	OnDraw(&dc);
}
void CDataMonitorView::DrawData(CDC* pDC)
{
	TRACE(_T("DrawCoordinateSystem"));
	GetClientRect(rect);
	UpdateData(FALSE);
	rectTotal = rect;
	DrawCoordinateSystem(pDC);
	DrawDataArray(pDC);
	
}
void CDataMonitorView::DrawDataArray(CDC* pDC)
{
	CPetroData* pPData = NULL ,*pOldPData = NULL;
	float dept =0,temp = 0,olddept =0,oldtemp = 0;
	float oldgr = 0,gr = 0;
	int x1=0,y1=0,dx1=0,dy1=0,dgry1=0;
	float x2=0,y2=0,dx2=0,dy2=0,dgry2=0;
	int iDrawType = PS_SOLID;
	COLORREF color = RGB(255,0,0);
	COLORREF colorBlue = RGB(0,0,255);
	CPen pen(iDrawType, 1,color); //画笔
	CPen pen2(iDrawType, 1,colorBlue); //画笔
	CPen* pOldPen = pDC->SelectObject(&pen);//画笔和画线区连接
	int gap = 20, gap1 = 300, gap2 = 350;
	POSITION pos = theApp.petroList.GetHeadPosition();
	while(pos)
	{
		pOldPData = pPData;
		pPData = theApp.petroList.GetNext(pos);
		dept = pPData->dept;
		dx1=(int)dept;    
		dx2=dept-(float)dx1;
		dx1-=theApp.nStartCoordinate;

		temp = pPData->temp;
		//dy1=(int)temp;    
		//dy2=temp-(float)dy1;

		gr = pPData->gr;
		if(pOldPData != NULL)
		{
			pDC->SelectObject(&pen);
			olddept = pOldPData->dept;
			x1=(int)olddept;    
			x2=olddept-(float)x1;
			x1-=theApp.nStartCoordinate;

			oldtemp = pOldPData->temp;
			y1=(int)oldtemp;    
			y2=oldtemp-(float)y1;

			int iTemp = (int)(x2*20/10);
			int iTemp2 = (int)(oldtemp*gap1/(tempArray[1]-tempArray[0]));

			int iTemp3 = (int)(dx2*20/10);
			int iTemp4 = (int)(temp*gap1/(tempArray[1]-tempArray[0]));

			pDC->MoveTo(x1+iTemp,iTemp2); // [0,0]
			pDC->LineTo(dx1+iTemp3,iTemp4);
			//gr
			pDC->SelectObject(&pen2);
			oldgr = pOldPData->gr;
			if(oldgr - 0 > 0.01 && gr - 0 > 0.01 )
			{
				y1=(int)oldgr;    
				y2=oldgr-(float)y1;

				iTemp = (int)(x2*20/10);
				iTemp2 = (int)(oldgr*gap1/(gmArray[1]-gmArray[0]));

				iTemp3 = (int)(dx2*20/10);
				iTemp4 = (int)(gr*gap1/(gmArray[1]-gmArray[0]));

				pDC->MoveTo(x1+iTemp,iTemp2); // [0,0]
				pDC->LineTo(dx1+iTemp3,iTemp4);
			}
		}
	}
	pDC->SelectObject(pOldPen);  //[可以不需要]
}
void CDataMonitorView::DrawCoordinateSystem(CDC* pDC)
{
	TRACE(_T("DrawCoordinateSystem"));
	//获得线的颜色颜色
	COLORREF color = RGB(127,127,127);
	CRect textRect;
	int gap = 20, gap1 = 300, gap2 = 350;
	CString str;
	CFont font;
	VERIFY(font.CreateFont(
		18,                        // nHeight
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
	//获得线的风格
	int iDrawType = PS_SOLID;
	//iDrawType = _wtoi(_T("1"));
	
    //设置线的画笔"风格"和"颜色"还有"粗细"
	CPen pen(iDrawType, 1,color); //画笔
	CPen* pOldPen = pDC->SelectObject(&pen);//画笔和画线区连接
	
	CRect rect;
	//以下两句讲获得控件在屏幕中的位置
	//GetDlgItem(IDC_STATIC_PIC)->GetWindowRect(&rect);//获取控件相对于屏幕的位置,这个控件必须隐藏
	//ScreenToClient(rect);//转化为对话框上的相对位置
	GetClientRect(rect);
	rectTotal = rect;
	rectTotal.right = rectTotal.left + 750;
	rectTotal.bottom = rectTotal.top + gap*25;
	UpdateData(FALSE);

	int iCount=0;

	for (int i=0,iCount = 0;i<=gap1;i+=gap,iCount++)
	{
		pDC->MoveTo(i,0); // [0,0]
		pDC->LineTo(i,rectTotal.Height());
		if(iCount != 0 && iCount%5 == 0)
		{
			pDC->MoveTo(i+1,0); // [0,0]
			pDC->LineTo(i+1,rectTotal.Height());
		}
	}
	for (int i=gap2,iCount = 0;i<=rectTotal.Width();i+=gap,iCount++)
	{
		pDC->MoveTo(i,0); // [0,0]
		pDC->LineTo(i,rectTotal.Height());
		if(iCount != 0 && iCount%5 == 0)
		{
			pDC->MoveTo(i+1,0); // [0,0]
			pDC->LineTo(i+1,rectTotal.Height());
		}
	}
	for (int j=0,iCount = 0;j<=rectTotal.Height();j+=gap,iCount++)
	{
		pDC->MoveTo(0,j); // [0,0]
		pDC->LineTo(gap1,j);
		if(iCount != 0 && iCount%10 == 0)
		{
			pDC->MoveTo(0,j+1); // [0,0]
			pDC->LineTo(gap1,j+1);
		}
	}
	for (int j=0,iCount = 0;j<=rectTotal.Height();j+=gap,iCount++)
	{
		pDC->MoveTo(gap2,j); // [0,0]
		pDC->LineTo(rectTotal.Width(),j);
		if(iCount != 0 && iCount%10 == 0)
		{
			pDC->MoveTo(gap2,j+1); // [0,0]
			pDC->LineTo(rectTotal.Width(),j+1);
		}
	}
	for (int j=0,iCount = 0;j<=rectTotal.Height();j+=gap*10,iCount++)
	{
		textRect = rectTotal;
		textRect.left = gap1+2;
		textRect.right = gap2-2;
		textRect.top = rectTotal.top + j-10;
		textRect.bottom = textRect.top +gap;
		pDC->SelectObject(&font);
		pDC->SetTextAlign(TA_LEFT);
		str.Format(_T("%d"),(j/gap+theApp.nStartCoordinate));
		//pDC->TextOut(1,30*i+16,str);   //输出文本值
		pDC->DrawText(str,textRect,DT_LEFT);

	}
	//画笔刷新
	pDC->SelectObject(pOldPen);  //[可以不需要]

	SetScrollTotalSizes(rectTotal);
}

void CDataMonitorView::SetScrollTotalSizes(CRect rect)
{
	SIZE sizeTotal;
	sizeTotal.cx = rect.Width();
	sizeTotal.cy = rect.Height();
	SetScrollSizes(MM_TEXT, sizeTotal);
}

void CDataMonitorView::OnInitialUpdate()
{
	CScrollView::OnInitialUpdate();

	// TODO: 在此添加专用代码和/或调用基类
	SIZE sizeTotal;
	sizeTotal.cx = rect.Width();
	sizeTotal.cy = rect.Height();

    SetScrollSizes(MM_TEXT, sizeTotal);
}


void CDataMonitorView::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	SCROLLINFO	scrollinfo;
	GetScrollInfo(SB_VERT,&scrollinfo,SIF_ALL);
	
#if 1
	pScrollBar = GetScrollBarCtrl(SB_VERT);
	int temp_01=GetScrollPos(SB_VERT);//取得当前值

	switch(nSBCode)
	{
	case SB_THUMBTRACK://拖动
		SetScrollPos(SB_VERT,nPos);
		break;
	case SB_ENDSCROLL://拖动完毕
		temp_01=GetScrollPos(SB_VERT);
		break;//取得当前值
	case SB_PAGEUP:
		//SetScrollPos(SB_VERT,temp_01-100);
		scrollinfo.nPos-=10;
		SetScrollInfo(SB_VERT,&scrollinfo);
		break;
	case SB_PAGEDOWN:
		//SetScrollPos(SB_VERT,temp_01+100);
		scrollinfo.nPos+=10;
		SetScrollInfo(SB_VERT,&scrollinfo);
		break;
	case SB_LINEUP:
		//SetScrollPos(SB_VERT,temp_01-1);
		scrollinfo.nPos-=10;
		SetScrollInfo(SB_VERT,&scrollinfo);
		break;
	case SB_LINEDOWN:
		//SetScrollPos(SB_VERT,temp_01+1);
		scrollinfo.nPos+=10;
		SetScrollInfo(SB_VERT,&scrollinfo);
		break;

	}
	
	CScrollView::OnVScroll(nSBCode, scrollinfo.nPos, pScrollBar);
#else
	if((nSBCode == SB_THUMBTRACK || nSBCode == SB_THUMBPOSITION) && ((nPos & 0xffff) > 0x7d00)){
		int yOrig, y;
		yOrig = GetScrollPos(SB_VERT);
		y = nPos;
		ScrollWindow(0, -(y - yOrig));
		SetScrollPos(SB_VERT, nPos & 0xFFFF);
		return;
	}

	CScrollView::OnVScroll(nSBCode, nPos, pScrollBar);
#endif
}


void CDataMonitorView::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	CScrollView::OnTimer(nIDEvent);
}


int CDataMonitorView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CScrollView::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  在此添加您专用的创建代码
	return 0;
}
