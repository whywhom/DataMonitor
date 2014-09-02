
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
	tempArray[1] = 0;

	gmArray[0] = 0;
	gmArray[1] = 0;

	rmArray[0] = 0;
	rmArray[1] = 0;

	cclArray[0] = 0;
	cclArray[1] = 0;

	magArray[0] = 0;
	magArray[1] = 0;

	oldtempArray[0] = 0;
	oldtempArray[1] = 0;

	oldgmArray[0] = 0;
	oldgmArray[1] = 0;

	oldrmArray[0] = 0;
	oldrmArray[1] = 0;

	oldcclArray[0] = 0;
	oldcclArray[1] = 0;

	oldmagArray[0] = 0;
	oldmagArray[1] = 0;

	base = 0;
	bias = 0;
	pPData = NULL;
	pOldPData = NULL;

	theApp.processType == NO_PROCESSING;
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
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: 在此处为本机数据添加绘制代码
	if(theApp.processType == REALTIME_PROCESSING)
	{
		DrawData(pDC);
	}
	else if(theApp.processType == FILEDATA_PROCESSING)
	{
		DrawDataFile(pDC);
	}
	else
	{
		//nothing to do
	}
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
	TRACE(_T("DrawCoordinateSystem\r\n"));
	GetClientRect(rect);
	UpdateData(FALSE);
	rectTotal = rect;
	DrawCoordinateSystem(pDC);
	DrawDataArray(pDC);
	
}

void CDataMonitorView::DrawDataFile(CDC* pDC)
{
	TRACE(_T("DrawCoordinateSystem\r\n"));
	GetClientRect(rect);
	UpdateData(FALSE);
	rectTotal = rect;
	DrawCoordinateSystemFile(pDC);
	DrawDataArrayFile(pDC);
	
}

void CDataMonitorView::DrawDataArray(CDC* pDC)
{
	int dept =0,temp = 0,olddept =0,oldtemp = 0;
	int oldgr = 0,gr = 0;
	int x1=0,y1=0,dx1=0,dy1=0,dgry1=0;
	int x2=0,y2=0,dx2=0,dy2=0,dgry2=0;
	int iDrawType = PS_SOLID;
	COLORREF color = RGB(255,0,0);
	COLORREF colorBlue = RGB(0,0,255);
	CPen pen(iDrawType, 1,color); //画笔
	CPen pen2(iDrawType, 1,colorBlue); //画笔
	CPen* pOldPen = pDC->SelectObject(&pen);//画笔和画线区连接
	int gap = 20, gap1 = 300, gap2 = 350;
	POSITION nPos = theApp.petroList.GetHeadPosition();
	while(nPos)
	{
		pPData = theApp.petroList.GetNext(nPos);
		dept = pPData->dept.integer;
		dx1=pPData->dept.integer;
		dx2=pPData->dept.decimal;
		dx1-=base;

		if(pOldPData != NULL)
		{
			pDC->SelectObject(&pen);
			x1=pOldPData->dept.integer;
			x2=pOldPData->dept.decimal;
			x1-=base;

			y1=pOldPData->temp.integer;
			y2=pOldPData->temp.decimal;

			int iTemp = (int)(x2*gap/10);
			int iTemp2 = (int)(pOldPData->temp.integer*gap1/(tempArray[1]-tempArray[0])+pOldPData->temp.decimal*gap1/(10*(tempArray[1]-tempArray[0])));

			int iTemp3 = (int)(dx2*gap/10);
			int iTemp4 = (int)(pPData->temp.integer*gap1/(tempArray[1]-tempArray[0])+pPData->temp.decimal*gap1/(10*(tempArray[1]-tempArray[0])));

			pDC->MoveTo(iTemp2,x1*gap+iTemp); // [0,0]
			pDC->LineTo(iTemp4,dx1*gap+iTemp3);
			//gr
#if 0
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
#endif
		}
		Sleep(1000);
		pOldPData = pPData;
	}

#if 0
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
#endif
	pDC->SelectObject(pOldPen);  //[可以不需要]
}

void CDataMonitorView::DrawDataArrayFile(CDC* pDC)
{
	int dept =0,temp = 0,olddept =0,oldtemp = 0;
	int oldgr = 0,gr = 0;
	long x1=0,y1=0,dx1=0,dy1=0,dgry1=0;
	long x2=0,y2=0,dx2=0,dy2=0,dgry2=0;
	long iTemp=0,iTemp2=0,iTemp3=0,iTemp4=0;
	int iDrawType = PS_SOLID;
	COLORREF colorRed = RGB(255,0,0);
	COLORREF colorBlue = RGB(0,0,255);
	COLORREF colorGreen = RGB(0,255,0);
	COLORREF colorBlack = RGB(0,0,0);
	COLORREF color1 = RGB(2,141,153);
	CPen pen(iDrawType, 1,colorRed); //画笔
	CPen pen2(iDrawType, 1,colorBlue); //画笔2
	CPen pen3(iDrawType, 1,colorGreen); //画笔3
	CPen pen4(iDrawType, 1,color1); //画笔4
	CPen pen5(iDrawType, 1,colorBlack); //画笔5
	CPen* pOldPen = pDC->SelectObject(&pen);//画笔和画线区连接
	int gap = 20, gap1 = 300, gap2 = 350;
	counter = 0;
	long length = theApp.petroList.GetCount();
	POSITION nPos = theApp.petroList.GetHeadPosition();
	while(nPos)
	{
		pPData = theApp.petroList.GetNext(nPos);
		dept = pPData->dept.integer;
		dx1=pPData->dept.integer;
		dx2=pPData->dept.decimal;
		dx1-=base;

		if(pOldPData != NULL)
		{
			//pDC->SelectObject(&pen);
			x1=pOldPData->dept.integer;
			x2=pOldPData->dept.decimal;
			x1-=base;
			//temp
			if(pOldPData->temp.integer >0 || pOldPData->temp.decimal >0)
			{
				oldtempArray[0] = pOldPData->temp.integer;
				oldtempArray[1] = pOldPData->temp.decimal;
			}
			if(pPData->temp.integer >0 || pPData->temp.decimal >0)
			{
				pDC->SelectObject(&pen);
				if(pOldPData->temp.integer >0 || pOldPData->temp.decimal >0)
				{
					oldtempArray[0] = pOldPData->temp.integer;
					oldtempArray[1] = pOldPData->temp.decimal;

					iTemp = (int)(x2*gap/10);
					iTemp2 = (int)((oldtempArray[0]-tempArray[0])*gap1/(tempArray[1]-tempArray[0])+oldtempArray[1]*gap1/(10*(tempArray[1]-tempArray[0])));

					iTemp3 = (int)(dx2*gap/10);
					iTemp4 = (int)((pPData->temp.integer-tempArray[0])*gap1/(tempArray[1]-tempArray[0])+pPData->temp.decimal*gap1/(10*(tempArray[1]-tempArray[0])));

					pDC->MoveTo(iTemp2,x1*gap+iTemp); // [0,0]
					pDC->LineTo(iTemp4,dx1*gap+iTemp3);
				}
			}
#if 1
			//gr
			if(pOldPData->gr.integer >0 || pOldPData->gr.decimal >0)
			{
				oldgmArray[0] = pOldPData->gr.integer;
				oldgmArray[1] = pOldPData->gr.decimal;
			}
			if(pPData->gr.integer >0 || pPData->gr.decimal >0)
			{
				pDC->SelectObject(&pen2);

				if(oldgmArray[0] >0 || oldgmArray[1] >0)
				{
					iTemp2 = (int)((oldgmArray[0]-gmArray[0])*gap1/(gmArray[1]-gmArray[0])+oldgmArray[1]*gap1/(10*(gmArray[1]-gmArray[0])));
					iTemp4 = (int)((pPData->gr.integer-gmArray[0])*gap1/(gmArray[1]-gmArray[0])+pPData->gr.decimal*gap1/(10*(gmArray[1]-gmArray[0])));
					pDC->MoveTo(iTemp2,x1*gap+iTemp); // [0,0]
					pDC->LineTo(iTemp4,dx1*gap+iTemp3);
				}
			}
			else
			{
				pDC->SelectObject(&pen2);

				if(oldgmArray[0] >0 || oldgmArray[1] >0)
				{
					iTemp2 = (int)((oldgmArray[0]-gmArray[0])*gap1/(gmArray[1]-gmArray[0])+oldgmArray[1]*gap1/(10*(gmArray[1]-gmArray[0])));
					pDC->MoveTo(iTemp2,x1*gap+iTemp); // [0,0]
					pDC->LineTo(iTemp2,dx1*gap+iTemp3);
				}
			}
#endif

#if 1
			//rm
			if(pOldPData->rm.integer >0 || pOldPData->rm.decimal >0)
			{
				oldrmArray[0] = pOldPData->rm.integer;
				oldrmArray[1] = pOldPData->rm.decimal;
			}
			if(pPData->rm.integer >0 || pPData->rm.decimal >0)
			{
				pDC->SelectObject(&pen3);

				if(oldrmArray[0] >0 || oldrmArray[1] >0)
				{
					iTemp2 = (int)((oldrmArray[0]-rmArray[0])*gap1/(rmArray[1]-rmArray[0])+oldrmArray[1]*gap1/(10*(rmArray[1]-rmArray[0])));
					iTemp4 = (int)((pPData->rm.integer-rmArray[0])*gap1/(rmArray[1]-rmArray[0])+pPData->rm.decimal*gap1/(10*(rmArray[1]-rmArray[0])));
					pDC->MoveTo(iTemp2,x1*gap+iTemp); // [0,0]
					pDC->LineTo(iTemp4,dx1*gap+iTemp3);
				}
			}
			else
			{
				pDC->SelectObject(&pen3);

				if(oldrmArray[0] >0 || oldrmArray[1] >0)
				{
					iTemp2 = (int)((oldrmArray[0]-rmArray[0])*gap1/(rmArray[1]-rmArray[0])+oldrmArray[1]*gap1/(10*(rmArray[1]-rmArray[0])));
					pDC->MoveTo(iTemp2,x1*gap+iTemp); // [0,0]
					pDC->LineTo(iTemp2,dx1*gap+iTemp3);
				}
			}
#endif
#if 1
			//ccl
			if(pOldPData->ccl.integer >0 || pOldPData->ccl.decimal >0)
			{
				oldcclArray[0] = pOldPData->ccl.integer;
				oldcclArray[1] = pOldPData->ccl.decimal;
			}
			if(pPData->ccl.integer >0 || pPData->ccl.decimal >0)
			{
				pDC->SelectObject(&pen4);

				if(oldcclArray[0] >0 || oldcclArray[1] >0)
				{
					iTemp2 = (int)((oldcclArray[0]-cclArray[0])*gap1/(cclArray[1]-cclArray[0])+oldcclArray[1]*gap1/(10*(cclArray[1]-cclArray[0])));
					iTemp4 = (int)((pPData->ccl.integer-cclArray[0])*gap1/(cclArray[1]-cclArray[0])+pPData->ccl.decimal*gap1/(10*(cclArray[1]-cclArray[0])));
					pDC->MoveTo(iTemp2,x1*gap+iTemp); // [0,0]
					pDC->LineTo(iTemp4,dx1*gap+iTemp3);
				}
			}
			else
			{
				pDC->SelectObject(&pen4);

				if(oldcclArray[0] >0 || oldcclArray[1] >0)
				{
					iTemp2 = (int)((oldcclArray[0]-cclArray[0])*gap1/(cclArray[1]-cclArray[0])+oldcclArray[1]*gap1/(10*(cclArray[1]-cclArray[0])));
					pDC->MoveTo(iTemp2,x1*gap+iTemp); // [0,0]
					pDC->LineTo(iTemp2,dx1*gap+iTemp3);
				}
			}
#endif
#if 1
			//mag
			if(pOldPData->mag[0].integer >0 || pOldPData->mag[0].decimal >0)
			{
				oldmagArray[0] = pOldPData->mag[0].integer;
				oldmagArray[1] = pOldPData->mag[0].decimal;
			}
			if(pPData->mag[0].integer >0 || pPData->mag[0].decimal >0)
			{
				pDC->SelectObject(&pen5);

				if(oldmagArray[0] >0 || oldmagArray[1] >0)
				{
					iTemp2 = (int)((oldmagArray[0]-magArray[0])*gap1/(magArray[1]-magArray[0])+oldmagArray[1]*gap1/(10*(magArray[1]-magArray[0])));
					iTemp4 = (int)((pPData->mag[0].integer-magArray[0])*gap1/(magArray[1]-magArray[0])+pPData->ccl.decimal*gap1/(10*(magArray[1]-magArray[0])));
					pDC->MoveTo(iTemp2,x1*gap+iTemp); // [0,0]
					pDC->LineTo(iTemp4,dx1*gap+iTemp3);
				}
			}
			else
			{
				pDC->SelectObject(&pen5);

				if(oldmagArray[0] >0 || oldmagArray[1] >0)
				{
					iTemp2 = (int)((oldmagArray[0]-magArray[0])*gap1/(magArray[1]-magArray[0])+oldmagArray[1]*gap1/(10*(magArray[1]-magArray[0])));
					pDC->MoveTo(iTemp2,x1*gap+iTemp); // [0,0]
					pDC->LineTo(iTemp2,dx1*gap+iTemp3);
				}
			}
#endif
		}
		//Sleep(1000);
		pOldPData = pPData;
		counter++;
		TRACE(_T("current %d / Total %d \r\n"),counter,length);
	}

#if 0
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
#endif
	pDC->SelectObject(pOldPen);  //[可以不需要]
	//theApp.processType = NO_PROCESSING;
}

void CDataMonitorView::DrawCoordinateSystem(CDC* pDC)
{
	TRACE(_T("DrawCoordinateSystem"));
	
	//获得线的颜色颜色
	COLORREF color = RGB(127,127,127);
	CRect textRect;
	int gap = 20, gap1 = 300, gap2 = 350;
	int coordinateLimit = 25;
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
	rectTotal.bottom = rectTotal.top + gap*coordinateLimit;
	UpdateData(FALSE);
	if(pPData)
	{
		if(pPData->dept.integer > bias)
		{
			base = pPData->dept.integer/10*10;//坐标起始取10的整数倍
		}
	}
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
		if(iCount != 0 && (iCount)%10 == 0)
		{
			pDC->MoveTo(0,j+1); // [0,0]
			pDC->LineTo(gap1,j+1);
		}
	}
	for (int j=0,iCount = 0;j<=rectTotal.Height();j+=gap,iCount++)
	{
		pDC->MoveTo(gap2,j); // [0,0]
		pDC->LineTo(rectTotal.Width(),j);
		if(iCount != 0 && (iCount)%10 == 0)
		{
			pDC->MoveTo(gap2,j+1); // [0,0]
			pDC->LineTo(rectTotal.Width(),j+1);
		}
	}
	bias = base + coordinateLimit;//深度到屏幕底部的坐标偏移

	for (int j=0,iCount = 0;j<=rectTotal.Height();j+=gap*10,iCount++)
	{
		textRect = rectTotal;
		textRect.left = gap1+2;
		textRect.right = gap2-2;
		textRect.top = rectTotal.top + j-10;
		textRect.bottom = textRect.top +gap;
		pDC->SelectObject(&font);
		pDC->SetTextAlign(TA_LEFT);
		str.Format(_T("%d"),(j/gap+base));
		//pDC->TextOut(1,30*i+16,str);   //输出文本值
		pDC->DrawText(str,textRect,DT_LEFT);

	}
	
	//画笔刷新
	pDC->SelectObject(pOldPen);  //[可以不需要]

	SetScrollTotalSizes(rectTotal);
}

unsigned long CDataMonitorView::GetMinData(DATA_PART tmp,unsigned long m)
{
	unsigned long t = 0;
	if(tmp.integer > 0)
	{
		if(m == 0)
		{
			t = tmp.integer/10*10;
		}
		else
		{
			t = min(m,tmp.integer/10*10);
		}
		return t;
	}
	else
	{
		return m;
	}
}
void CDataMonitorView::DrawCoordinateSystemFile(CDC* pDC)
{
	TRACE(_T("DrawCoordinateSystem"));
	
	//获得线的颜色颜色
	COLORREF color = RGB(127,127,127);
	CRect textRect;
	int gap = 20, gap1 = 300, gap2 = 350;
	int coordinateLimit = 25;
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
	
	CPetroData* pStartPData = theApp.petroList.GetHead();
	CPetroData* pEndPData = theApp.petroList.GetTail();

	POSITION nPos = theApp.petroList.GetHeadPosition();
	while(nPos)
	{
		unsigned long m = 0;
		CPetroData* pData = theApp.petroList.GetNext(nPos);
		
		tempArray[0] = GetMinData(pData->temp,tempArray[0]);
		tempArray[1] = max(tempArray[1], pData->temp.integer/10*10+10);

		gmArray[0] = GetMinData(pData->gr,gmArray[0]);
		gmArray[1] = max(gmArray[1], pData->gr.integer/10*10+10);

		rmArray[0] = GetMinData(pData->rm,rmArray[0]);
		rmArray[1] = max(rmArray[1], pData->rm.integer/10*10+10);

		cclArray[0] = GetMinData(pData->ccl,cclArray[0]);
		cclArray[1] = max(cclArray[1], pData->ccl.integer/10*10+10);

		magArray[0] = GetMinData(pData->mag[0],magArray[0]);
		magArray[1] = max(magArray[1], pData->mag[0].integer/10*10+10);
	}
	CRect rect;
	//以下两句讲获得控件在屏幕中的位置
	//GetDlgItem(IDC_STATIC_PIC)->GetWindowRect(&rect);//获取控件相对于屏幕的位置,这个控件必须隐藏
	//ScreenToClient(rect);//转化为对话框上的相对位置
	GetClientRect(rect);
	rectTotalFile = rect;
	rectTotalFile.right = rectTotalFile.left + 750;
	rectTotalFile.bottom = rectTotalFile.top + gap*(pEndPData->dept.integer/10*10+10 - pStartPData->dept.integer/10*10);
	base = pStartPData->dept.integer/10*10;
	UpdateData(FALSE);
	
	int iCount=0;

	for (int i=0,iCount = 0;i<=gap1;i+=gap,iCount++)
	{
		pDC->MoveTo(i,0); // [0,0]
		pDC->LineTo(i,rectTotalFile.Height());
		if(iCount != 0 && iCount%5 == 0)
		{
			pDC->MoveTo(i+1,0); // [0,0]
			pDC->LineTo(i+1,rectTotalFile.Height());
		}
	}
	for (int i=gap2,iCount = 0;i<=rectTotalFile.Width();i+=gap,iCount++)
	{
		pDC->MoveTo(i,0); // [0,0]
		pDC->LineTo(i,rectTotalFile.Height());
		if(iCount != 0 && iCount%5 == 0)
		{
			pDC->MoveTo(i+1,0); // [0,0]
			pDC->LineTo(i+1,rectTotalFile.Height());
		}
	}
	for (int j=0,iCount = 0;j<=rectTotalFile.Height();j+=gap,iCount++)
	{
		pDC->MoveTo(0,j); // [0,0]
		pDC->LineTo(gap1,j);
		if(iCount != 0 && (iCount)%10 == 0)
		{
			pDC->MoveTo(0,j+1); // [0,0]
			pDC->LineTo(gap1,j+1);
		}
	}
	for (int j=0,iCount = 0;j<=rectTotalFile.Height();j+=gap,iCount++)
	{
		pDC->MoveTo(gap2,j); // [0,0]
		pDC->LineTo(rectTotalFile.Width(),j);
		if(iCount != 0 && (iCount)%10 == 0)
		{
			pDC->MoveTo(gap2,j+1); // [0,0]
			pDC->LineTo(rectTotalFile.Width(),j+1);
		}
	}
	//bias = base + coordinateLimit;//深度到屏幕底部的坐标偏移

	for (int j=0,iCount = 0;j<=rectTotalFile.Height();j+=gap*10,iCount++)
	{
		textRect = rectTotalFile;
		textRect.left = gap1+2;
		textRect.right = gap2-2;
		textRect.top = rectTotalFile.top + j-10;
		textRect.bottom = textRect.top +gap;
		pDC->SelectObject(&font);
		pDC->SetTextAlign(TA_LEFT);
		str.Format(_T("%d"),(j/gap+base));
		//pDC->TextOut(1,30*i+16,str);   //输出文本值
		pDC->DrawText(str,textRect,DT_LEFT);

	}
	
	//画笔刷新
	pDC->SelectObject(pOldPen);  //[可以不需要]

	SetScrollTotalSizes(rectTotalFile);
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
void CDataMonitorView::StartTimer()
{
	base = 0;
	bias = 0;
	counter = 0;
	pos = theApp.petroList.GetHeadPosition();
	if(theApp.processType == REALTIME_PROCESSING)
	{
		SetTimer(TIMER_CMD_DRAW,TIME_REFRESH_REALTIME,NULL);
	}
	else// if(theApp.processType == FILEDATA_PROCESSING)
	{
		SetTimer(TIMER_CMD_DRAW,TIME_REFRESH_FILE,NULL);
	}
}

void CDataMonitorView::StopTimer()
{
	KillTimer(TIMER_CMD_DRAW);
}

void CDataMonitorView::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	//KillTimer(nIDEvent);
	switch(nIDEvent)
	{
	case TIMER_CMD_DRAW:
		{
			//TRACE(_T("\r\n OnTimer() for + %d + ms \r\n"),TIME_REFRESH);
			if(theApp.processType == FILEDATA_PROCESSING)
			{
				DrawData();
				KillTimer(nIDEvent);
				return;
			}
			if(theApp.processType == REALTIME_PROCESSING)
			{
				DrawData();
				SetTimer(TIMER_CMD_DRAW,TIME_REFRESH_REALTIME,NULL);
			}
		}
		break;
	default:
		break;
	}
	CScrollView::OnTimer(nIDEvent);
}

void CDataMonitorView::DrawData()
{
#if 0
	if(!theApp.petroList.IsEmpty())
	{
		pPData = theApp.petroList.RemoveHead();
	}
#endif
	Invalidate();
}
int CDataMonitorView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CScrollView::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  在此添加您专用的创建代码
	return 0;
}
