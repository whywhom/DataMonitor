
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
	ON_WM_SIZE()
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()

// CDataMonitorView 构造/析构

CDataMonitorView::CDataMonitorView()
{
	// TODO: 在此处添加构造代码
	InitArrayData();

	InitOldArrayData();

	unitPixel = 5;//20 pixel = 1 m
	unitRatio = 4;
	m_bDirectDown = true;

	base = 0;
	bias = 0;
	
	minDepth = 0;
	maxDepth = 0;
	minDepthLimit = 0;
	maxDepthLimit = 0;
	
	m_drawCount = 0;

	pPData = NULL;
	pOldPData = NULL;

	pos = NULL;//当前记录位置
	pCurrentData = NULL;//当前记录指针

	VERIFY(m_font.CreateFont(
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
	{
		return;
	}
	// TODO: 在此处为本机数据添加绘制代码
	GetClientRect(m_clientRect);
	UpdateData(FALSE);
	//建立与屏幕显示兼容的内存显示设备，这时还不能绘图，因为没有地方画
	MemDC.CreateCompatibleDC(NULL);  
	//建立一个与屏幕显示兼容的位图，至于位图的大小，可以用窗口的大小  
	MemBitmap.CreateCompatibleBitmap(pDC,m_clientRect.Width(),m_clientRect.Height());  
	CBitmap *pOldBit=MemDC.SelectObject(&MemBitmap);
	MemDC.FillSolidRect(m_clientRect,RGB(255,255,255));

	DrawData(&MemDC);

	//将内存中的图拷贝到屏幕上进行显示  
	pDC->BitBlt(m_clientRect.left,m_clientRect.top,m_clientRect.Width(),m_clientRect.Height(),&MemDC,0,0,SRCCOPY);  
	//绘图完成后的清理  
	MemBitmap.DeleteObject();  
	MemDC.DeleteDC();  
}

void CDataMonitorView::SetDirectionDown(bool bDown)
{
	m_bDirectDown = bDown;
}

void CDataMonitorView::DrawBasic(CDC * pDC)
{
	m_totalRect = m_clientRect;
	maxDepth = minDepth + m_clientRect.Height()/unitPixel;
	int depPixel = (int)((maxDepth - minDepthLimit)*10/10) * unitPixel;
	m_totalRect.bottom = m_totalRect.top + max(depPixel,m_totalRect.Height());
	
	SetScrollTotalSizes(m_totalRect);
}
void CDataMonitorView::DrawGrid(CDC * pDC)
{
	m_plot1Rect = m_clientRect;
	m_plot1Rect.right = m_plot1Rect.left + 400;

	m_plot2Rect = m_clientRect;
	m_plot2Rect.left = m_plot1Rect.right;
	m_plot2Rect.right = m_plot2Rect.left + 200;

	m_plot3Rect = m_clientRect;
	m_plot3Rect.left = m_plot2Rect.right;

	m_gridColor = RGB(127,127,127);

	CPen *old, pen(PS_SOLID, 1, m_gridColor), pen2(PS_DOT, 1,m_gridColor); //画笔;
	CPen stick(PS_SOLID,0,RGB(0,0,0));
	CPen mline(PS_SOLID,0,RGB(192,192,192));
	//绘制坐标系
	int iCount=0;

	for (int i=m_plot1Rect.left,iCount= 0;i<=m_plot1Rect.right;i+=unitPixel*unitRatio,iCount++)
	{
		if(iCount != 0 && iCount%5 == 0)
		{
			pDC->SelectObject(&pen);
			pDC->MoveTo(i,m_plot1Rect.top); //
			pDC->LineTo(i,m_plot1Rect.bottom);
			pDC->MoveTo(i+1,m_plot1Rect.top); //
			pDC->LineTo(i+1,m_plot1Rect.bottom);
		}
		else
		{
			pDC->SelectObject(&pen2);
			pDC->MoveTo(i,m_plot1Rect.top); // [0,0]
			pDC->LineTo(i,m_plot1Rect.bottom);
		}
	}
	for (int i=m_plot3Rect.left,iCount = 0;i<=m_plot3Rect.right;i+=unitPixel*unitRatio,iCount++)
	{
		
		if(iCount%5 == 0)
		{
			pDC->SelectObject(&pen);
			pDC->MoveTo(i,m_plot3Rect.top);
			pDC->LineTo(i,m_plot3Rect.bottom);
			pDC->MoveTo(i+1,m_plot3Rect.top);
			pDC->LineTo(i+1,m_plot3Rect.bottom);
		}
		else
		{
			pDC->SelectObject(&pen2);
			pDC->MoveTo(i,m_plot3Rect.top);
			pDC->LineTo(i,m_plot3Rect.bottom);
		}
	}
	for (int j=m_plot1Rect.top,iCount = 0;j<=m_plot3Rect.bottom;j+=unitPixel,iCount++)
	{
		
		if(iCount != 0 && (iCount)%5 == 0)
		{
#if 0
			pDC->SelectObject(&pen);
			pDC->MoveTo(m_plot1Rect.left,j); // [0,0]
			pDC->LineTo(m_plot1Rect.right,j);
			pDC->MoveTo(m_plot1Rect.left,j+1); // [0,0]
			pDC->LineTo(m_plot1Rect.right,j+1);
		}
		else
		{
#endif
			pDC->SelectObject(&pen2);
			pDC->MoveTo(m_plot1Rect.left,j); // [0,0]
			pDC->LineTo(m_plot1Rect.right,j);
		}
	}
	for (int j=m_plot3Rect.top,iCount = 0;j<=m_plot3Rect.bottom;j+=unitPixel,iCount++)
	{
		
		if(iCount != 0 && (iCount)%5 == 0)
		{
#if 0
			pDC->SelectObject(&pen);
			pDC->MoveTo(m_plot3Rect.left,j);
			pDC->LineTo(m_plot3Rect.right,j);
			pDC->MoveTo(m_plot3Rect.left,j+1);
			pDC->LineTo(m_plot3Rect.right,j+1);
		}
		else
		{
#endif
			pDC->SelectObject(&pen2);
			pDC->MoveTo(m_plot3Rect.left,j); // [0,0]
			pDC->LineTo(m_plot3Rect.right,j);
		}
	}
	//画笔刷新
	pDC->SelectObject(old);
}

void CDataMonitorView::DrawPlot(CDC* pDC)
{
	m_zoomFont.lfHeight			= -13;
	m_zoomFont.lfWidth			= 0;
	m_zoomFont.lfEscapement		= 0;
	m_zoomFont.lfOrientation		= 0;
	m_zoomFont.lfWeight			= 400;
	m_zoomFont.lfItalic			= FALSE;
	m_zoomFont.lfUnderline		= FALSE;
	m_zoomFont.lfStrikeOut		= FALSE;
	m_zoomFont.lfCharSet			= ANSI_CHARSET;
	m_zoomFont.lfOutPrecision	= OUT_DEFAULT_PRECIS;
	m_zoomFont.lfClipPrecision	= CLIP_DEFAULT_PRECIS;
	m_zoomFont.lfQuality			= PROOF_QUALITY;
	m_zoomFont.lfPitchAndFamily	= DEFAULT_PITCH;
	wcscpy(m_zoomFont.lfFaceName,_T("Ariel"));
	
	m_font.Detach();
	m_font.CreateFontIndirect(&m_zoomFont);
	CFont *oFont = pDC->SelectObject(&m_font);
	CRect textRect;
	CSize z=pDC->GetTextExtent(CString("A"));
	textRect = m_plot2Rect;
	textRect.left += 2;
	textRect.right -= 2;
	CString str;
	for (int i=m_plot2Rect.top,iCount = 0;i<=m_plot2Rect.bottom;i+=unitPixel*10,iCount++)
	{
		textRect.top = m_plot2Rect.top + i-z.cy/2;
		textRect.bottom = textRect.top + z.cy;
		double tmp = i/unitPixel+minDepth;
		str.Format(_T("%.1f"),tmp);
		pDC->DrawText(str,textRect,DT_LEFT|DT_VCENTER);

	}
	pDC->SelectObject(oFont);
}

void CDataMonitorView::DrawCurve(CDC* pDC , double upDepth, double DownDepth)
{
	int iDrawType = PS_SOLID;
	long pre_iy=0,cur_iy=0,pre_dy=0,cur_dy=0;
	long pre_ix=0,cur_ix=0,pre_dx=0,cur_dx=0;
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
	int mCounter = 0;
	if(m_bDirectDown)
	{
		pos = theApp.petroList.GetHeadPosition();
	}
	else
	{
		pos = theApp.petroList.GetTailPosition();
	}
	while(pos)
	{
		pPData = theApp.petroList.GetNext(pos);
		if(pPData->dept.bAssign == true)
		{
			if(pPData->dept.iData < minDepth)
			{
				continue;
			}
			else
			{
				if(mCounter < m_drawCount*m_iterator && pPData->dept.iData < maxDepth)
				{
					if(pPData->dept.bAssign)
					{
						//深度
						if(olddeptArray.bAssign)
						{
							
						}
						else
						{
						
						}
						//draw tepm
						if(pPData->temp.bAssign)
						{
							if(oldtempArray.bAssign)
							{
								pDC->SelectObject(&pen);
								long tempRange = tempLimitArray[1]-tempLimitArray[0];
								int i_dx = (int)((oldtempArray.dx - (int)oldtempArray.dx)*10);
								int d_dx = (int)(oldtempArray.dx);
								pre_ix = (int)((d_dx-tempLimitArray[0])*m_plot1Rect.Width()/tempRange
									+i_dx*m_plot1Rect.Width()/(10*tempRange));
								double mid = (pPData->temp.iData-tempLimitArray[0])*m_plot1Rect.Width()/tempRange;
								cur_ix = (int)mid;
								pre_iy = (oldtempArray.dy - minDepth)*unitPixel;
								cur_iy = (pPData->dept.iData - minDepth)*unitPixel;
								pDC->MoveTo(pre_ix,pre_iy);
								pDC->LineTo(cur_ix,cur_iy);
							}
							else
							{
								oldtempArray.bAssign = pPData->temp.bAssign;
								oldtempArray.dx = pPData->temp.iData;
								oldtempArray.dy = pPData->dept.iData;
							}
						}
					}
					
					if(pPData->dept.iData > maxDepth)
					{
						minDepth =(int) (minDepth+ unitPixel*5);
						m_iterator = 1;
						mCounter = 0;
						break;
					}
					if(mCounter < m_drawCount*m_iterator )
					{
						mCounter++;
					}
				}
				else
				{
					if(pPData->dept.iData > maxDepth)
					{
						minDepth = (int) (minDepth+ unitPixel*5);//minDepth = (int)pPData->dept.iData;
						m_iterator = 1;
						mCounter = 0;
						break;
					}
					if(mCounter >= m_drawCount*m_iterator )
					{
						m_iterator++;
						break;
					}
					
				}
			}
			
		}
		
	}
	if(pos == NULL)
	{
		KillTimer(TIMER_CMD_DRAW);//
	}
	else
	{
		SetTimer(TIMER_CMD_DRAW,TIME_REFRESH_FILE,NULL);
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
	DrawBasic(pDC);//获取内存映射绘制区域大小
	DrawGrid(pDC);
	DrawPlot(pDC);
	DrawCurve(pDC,minDepth,maxDepth);
}


#if 0
void CDataMonitorView::DrawDataArray(CDC* pDC)
{
	int dept =0,temp = 0,olddept =0,oldtemp = 0;
	int oldgr = 0,gr = 0;
	int old_iy=0,y1=0,dold_iy=0,dy1=0,dgry1=0;
	int old_dy=0,y2=0,cur_dy=0,dy2=0,dgry2=0;
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
		dept = pPData->dept.iData;
		dold_iy=pPData->dept.iData;
		cur_dy=pPData->dept.decimal;
		dold_iy-=base;

		if(pOldPData != NULL)
		{
			pDC->SelectObject(&pen);
			old_iy=pOldPData->dept.iData;
			old_dy=pOldPData->dept.decimal;
			old_iy-=base;

			y1=pOldPData->temp.iData;
			y2=pOldPData->temp.decimal;

			int iOldDept_y = (int)(old_dy*gap/10);
			int iOld_ix = (int)(pOldPData->temp.iData*gap1/(tempArray[1]-tempArray[0])+pOldPData->temp.decimal*gap1/(10*(tempArray[1]-tempArray[0])));

			int iCurDept_y = (int)(cur_dy*gap/10);
			int iCur_ix = (int)(pPData->temp.iData*gap1/(tempArray[1]-tempArray[0])+pPData->temp.decimal*gap1/(10*(tempArray[1]-tempArray[0])));

			pDC->MoveTo(iOld_ix,old_iy*gap+iOldDept_y); // [0,0]
			pDC->LineTo(iCur_ix,dold_iy*gap+iCurDept_y);
			//gr

		}
		Sleep(1000);
		pOldPData = pPData;
	}
	pDC->SelectObject(pOldPen);  //[可以不需要]
}


void CDataMonitorView::DrawDataArrayFile(CDC* pDC)
{
	unsigned long dept =0,temp = 0,olddept =0,oldtemp = 0;
	unsigned long oldgr = 0,gr = 0;
	unsigned long old_iy=0,y1=0,cur_iy=0,dy1=0,dgry1=0;
	unsigned long old_dy=0,y2=0,cur_dy=0,dy2=0,dgry2=0;
	unsigned long iOldDept_y=0,iOld_ix=0,iCurDept_y=0,iCur_ix=0;
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
	int gap = 20, gap1 = 300, gap2 = 400;
	counter = 0;
	long length = theApp.petroList.GetCount();
	POSITION nPos = theApp.petroList.GetHeadPosition();
	
	while(nPos)
	{
		pPData = theApp.petroList.GetNext(nPos);
		//if(pOldPData != NULL)
		{
			if(pPData->dept.bAssign)
			{
				if(olddeptArray.bAssign)
				{
					old_iy=olddeptArray.dx_i;
					old_dy=olddeptArray.dx_d;
				}
				
				cur_iy=pPData->dept.iData;
				cur_dy=pPData->dept.decimal;
				cur_iy-=base;

				iOldDept_y = (int)(old_dy*gap/10);
				iCurDept_y = (int)(cur_dy*gap/10);
			
#ifdef FEATURE_TEMP
				//temp
				if(pPData->temp.bAssign)
				{
					if(oldtempArray.bAssign)
					{
						old_iy=oldtempArray.dx_i;
						old_dy=oldtempArray.dx_d;

						pDC->SelectObject(&pen);
						iOld_ix = (int)((oldtempArray.dx_i-tempArray[0])*gap1/(tempArray[1]-tempArray[0])+oldtempArray.dx_d*gap1/(10*(tempArray[1]-tempArray[0])));
						iCur_ix = (int)((pPData->temp.iData-tempArray[0])*gap1/(tempArray[1]-tempArray[0])+pPData->temp.decimal*gap1/(10*(tempArray[1]-tempArray[0])));

						pDC->MoveTo(iOld_ix,oldtempArray.dy); // [0,0]
						pDC->LineTo(iCur_ix,cur_iy*gap+iCurDept_y);
						TRACE(_T("TEMP %d : from (%d , %d ) to (%d , %d) \r\n"),counter,iOld_ix,oldtempArray.dy,iCur_ix,cur_iy*gap+iCurDept_y);
					}
					
					oldtempArray.dx_i = pPData->temp.iData;
					oldtempArray.dx_d = pPData->temp.decimal;
					oldtempArray.dy = cur_iy*gap+iCurDept_y;
					oldtempArray.bAssign = true;
				}
#endif
#ifdef FEATURE_GR
			//gr
				if(pPData->gr.bAssign)
				{
					if(oldgmArray.bAssign)
					{
						old_iy=oldgmArray.dx_i;
						old_dy=oldgmArray.dx_d;

						pDC->SelectObject(&pen2);
						iOld_ix = (int)((oldgmArray.dx_i-gmArray[0])*gap1/(gmArray[1]-gmArray[0])+oldgmArray.dx_d*gap1/(10*(gmArray[1]-gmArray[0])));
						iCur_ix = (int)((pPData->gr.iData-gmArray[0])*gap1/(gmArray[1]-gmArray[0])+pPData->gr.decimal*gap1/(10*(gmArray[1]-gmArray[0])));

						pDC->MoveTo(iOld_ix,oldgmArray.dy); // [0,0]
						pDC->LineTo(iCur_ix,cur_iy*gap+iCurDept_y);
						TRACE(_T("GR %d : from (%d , %d ) to (%d , %d) \r\n"),counter,iOld_ix,oldgmArray.dy,iCur_ix,cur_iy*gap+iCurDept_y);
					}
					
					oldgmArray.dx_i = pPData->gr.iData;
					oldgmArray.dx_d = pPData->gr.decimal;
					oldgmArray.dy = cur_iy*gap+iCurDept_y;
					oldgmArray.bAssign = true;
				}
#endif
#ifdef FEATURE_RM
			//rm
				if(pPData->rm.bAssign)
				{
					if(oldrmArray.bAssign)
					{
						old_iy=oldrmArray.dx_i;
						old_dy=oldrmArray.dx_d;

						pDC->SelectObject(&pen3);
						iOld_ix = (int)((oldrmArray.dx_i-rmArray[0])*gap1/(rmArray[1]-rmArray[0])+oldrmArray.dx_d*gap1/(10*(rmArray[1]-rmArray[0])));
						iCur_ix = (int)((pPData->rm.iData-rmArray[0])*gap1/(rmArray[1]-rmArray[0])+pPData->rm.decimal*gap1/(10*(rmArray[1]-rmArray[0])));

						pDC->MoveTo(iOld_ix,oldrmArray.dy); // [0,0]
						pDC->LineTo(iCur_ix,cur_iy*gap+iCurDept_y);
						TRACE(_T("RM %d : from (%d , %d ) to (%d , %d) \r\n"),counter,iOld_ix,oldgmArray.dy,iCur_ix,cur_iy*gap+iCurDept_y);
					}
					
					oldrmArray.dx_i = pPData->rm.iData;
					oldrmArray.dx_d = pPData->rm.decimal;
					oldrmArray.dy = cur_iy*gap+iCurDept_y;
					oldrmArray.bAssign = true;
				}
#endif
#ifdef FEATURE_CCL
			//ccl
				if(pPData->ccl.bAssign)
				{
					if(oldcclArray.bAssign)
					{
						old_iy=oldcclArray.dx_i;
						old_dy=oldcclArray.dx_d;

						pDC->SelectObject(&pen4);
						iOld_ix = (int)((oldcclArray.dx_i-cclArray[0])*gap1/(cclArray[1]-cclArray[0])+oldcclArray.dx_d*gap1/(10*(cclArray[1]-cclArray[0])));
						iCur_ix = (int)((pPData->ccl.iData-cclArray[0])*gap1/(cclArray[1]-cclArray[0])+pPData->ccl.decimal*gap1/(10*(cclArray[1]-cclArray[0])));

						pDC->MoveTo(iOld_ix,oldcclArray.dy); // [0,0]
						pDC->LineTo(iCur_ix,cur_iy*gap+iCurDept_y);
						TRACE(_T("CCL %d : from (%d , %d ) to (%d , %d) \r\n"),counter,iOld_ix,oldcclArray.dy,iCur_ix,cur_iy*gap+iCurDept_y);
					}
					
					oldcclArray.dx_i = pPData->ccl.iData;
					oldcclArray.dx_d = pPData->ccl.decimal;
					oldcclArray.dy = cur_iy*gap+iCurDept_y;
					oldcclArray.bAssign = true;
				}
#endif
#ifdef FEATURE_MAG
				//mag
				if(pPData->mag[0].bAssign)
				{
					if(oldmagArray.bAssign)
					{
						old_iy=oldmagArray.dx_i;
						old_dy=oldmagArray.dx_d;

						pDC->SelectObject(&pen5);
						iOld_ix = (int)((oldmagArray.dx_i-magArray[0])*gap1/(magArray[1]-magArray[0])+oldmagArray.dx_d*gap1/(10*(magArray[1]-magArray[0])));
						iCur_ix = (int)((pPData->mag[0].iData-magArray[0])*gap1/(magArray[1]-magArray[0])+pPData->mag[0].decimal*gap1/(10*(magArray[1]-magArray[0])));

						pDC->MoveTo(iOld_ix,oldmagArray.dy); // [0,0]
						pDC->LineTo(iCur_ix,cur_iy*gap+iCurDept_y);
						TRACE(_T("CCL %d : from (%d , %d ) to (%d , %d) \r\n"),counter,iOld_ix,oldmagArray.dy,iCur_ix,cur_iy*gap+iCurDept_y);
					}
					
					oldmagArray.dx_i = pPData->mag[0].iData;
					oldmagArray.dx_d = pPData->mag[0].decimal;
					oldmagArray.dy = cur_iy*gap+iCurDept_y;
					oldmagArray.bAssign = true;
				}
#endif
			}
			else
			{
			
			}
		}
		//Sleep(1000);
		pOldPData = pPData;
		counter++;
		TRACE(_T("current %d / Total %d \r\n"),counter,length);
	}
	InitOldArrayData();
	pDC->SelectObject(pOldPen);  //[可以不需要]
	//theApp.processType = NO_PROCESSING;
}
#endif

unsigned long CDataMonitorView::GetMinData(DATA_PART tmp,unsigned long m)
{
	unsigned long t = 0;
	if(tmp.bAssign == false)
	{
		return m;
	}
	if(tmp.iData > 0)
	{
		if(m == 0)
		{
			t = tmp.iData/10*10;
		}
		else
		{
			t = min(m,tmp.iData/10*10);
		}
		return t;
	}
	else
	{
		return m;
	}
}


void CDataMonitorView::SetScrollTotalSizes(CRect rect)
{
	sizeTotal.cx = rect.Width();
	sizeTotal.cy = rect.Height();
	TRACE(_T("cx = %d ; cy = %d \r\n"),sizeTotal.cx,sizeTotal.cy);
	SetScrollSizes(MM_TEXT, sizeTotal);
}

void CDataMonitorView::OnInitialUpdate()
{
	CScrollView::OnInitialUpdate();

	// TODO: 在此添加专用代码和/或调用基类
	CRect Rect;
	GetClientRect(Rect);
	SetScrollTotalSizes(Rect);
#ifdef FEATURE_TEST_DRAW
	InitPlot(Rect);
#endif
	SetScrollTotalSizes(Rect);
	//SetTimer(TIMER_CMD_TEST,TIME_REFRESH_FILE,NULL);
}

#ifdef FEATURE_TEST_DRAW
void CDataMonitorView::InitPlot(CRect Rect)
{

	m_Plot.Create(WS_CHILD|WS_VISIBLE,Rect,this,12000);

	m_Plot.SetSerie(0, PS_SOLID, RGB(255,0,0), 0.0, 2000.0, "Pressure");
	m_Plot.SetSerie(1, PS_DOT, RGB(0,255,0), 0.0, 2000.0, "Pressure");
	m_Plot.SetSerie(2, PS_DASH, RGB(0,0,255), 0.0, 2000.0, "Pressure");
	m_Plot.SetSerie(3, PS_SOLID, RGB(255,255,0), 0.0, 2000.0, "Pressure");

	m_Plot.SetLegend(0, PS_SOLID, RGB(255,0,0), "Temperature");
	m_Plot.SetLegend(1, PS_DOT, RGB(0,255,0), "Pressure");
	m_Plot.SetLegend(2, PS_DASH, RGB(0,0,255), "Pressure");
	m_Plot.SetLegend(3, PS_SOLID, RGB(255,255,0), "Pressure");

	m_Plot.m_bAutoScrollX=TRUE;

}
#endif
void CDataMonitorView::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	SCROLLINFO scrollinfo;
	GetScrollInfo(SB_VERT,&scrollinfo,SIF_ALL);
	switch (nSBCode)
	{
	case SB_BOTTOM:
		ScrollWindow(0,(scrollinfo.nPos-scrollinfo.nMax)*10);
		scrollinfo.nPos = scrollinfo.nMax;
		SetScrollInfo(SB_VERT,&scrollinfo,SIF_ALL);
		break;
	case SB_TOP:
		ScrollWindow(0,(scrollinfo.nPos-scrollinfo.nMin)*10);
		scrollinfo.nPos = scrollinfo.nMin;
		SetScrollInfo(SB_VERT,&scrollinfo,SIF_ALL);
		break;
	case SB_LINEUP:
		scrollinfo.nPos -= 1;
		if (scrollinfo.nPos<scrollinfo.nMin)
		{
			scrollinfo.nPos = scrollinfo.nMin;
			break;
		}
		SetScrollInfo(SB_VERT,&scrollinfo,SIF_ALL);
		ScrollWindow(0,15);
		break;
	case SB_LINEDOWN:
		scrollinfo.nPos += 1;
		if (scrollinfo.nPos>scrollinfo.nMax)
		{
			scrollinfo.nPos = scrollinfo.nMax;
			break;
		}
		SetScrollInfo(SB_VERT,&scrollinfo,SIF_ALL);
		ScrollWindow(0,-15);
		break;
	case SB_PAGEUP:
		scrollinfo.nPos -= 5;
		if (scrollinfo.nPos<scrollinfo.nMin)
		{
			scrollinfo.nPos = scrollinfo.nMin;
			break;
		}
		SetScrollInfo(SB_VERT,&scrollinfo,SIF_ALL);
		ScrollWindow(0,10*5);
		break;
	case SB_PAGEDOWN:
		scrollinfo.nPos += 5;
		if (scrollinfo.nPos>scrollinfo.nMax)
		{
			scrollinfo.nPos = scrollinfo.nMax;
			break;
		}
		SetScrollInfo(SB_VERT,&scrollinfo,SIF_ALL);
		ScrollWindow(0,-10*5);
		break;
	case SB_ENDSCROLL:
		// MessageBox("SB_ENDSCROLL");
		break;
	case SB_THUMBPOSITION:
		// ScrollWindow(0,(scrollinfo.nPos-nPos)*10);
		// scrollinfo.nPos = nPos;
		// SetScrollInfo(SB_VERT,&scrollinfo,SIF_ALL);
		break;
	case SB_THUMBTRACK:
		ScrollWindow(0,(scrollinfo.nPos-nPos)*15);
		scrollinfo.nPos = nPos;
		SetScrollInfo(SB_VERT,&scrollinfo,SIF_ALL);
		break;
	}
#if 0
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
	//DrawData();
	CScrollView::OnVScroll(nSBCode, nPos, pScrollBar);
#endif
}
void CDataMonitorView::KillDrawTimer()
{
	KillTimer(TIMER_CMD_TEST);
}
void CDataMonitorView::StartTimer()
{
	base = 0;
	bias = 0;
	counter = 0;
	m_iterator = 1;
	if(theApp.petroList.IsEmpty())
	{
		return;//没有数据不进行绘制
	}
	if(m_bDirectDown)
	{
		pos = theApp.petroList.GetHeadPosition();
		pCurrentData = theApp.petroList.GetAt(pos);
	}
	else
	{
		pos = theApp.petroList.GetTailPosition();
		pCurrentData = theApp.petroList.GetAt(pos);
	}

	if(theApp.processType == REALTIME_PROCESSING)
	{
		m_drawCount = TIME_REFRESH_REALTIME/20;
		SetTimer(TIMER_CMD_DRAW,TIME_REFRESH_REALTIME,NULL);
	}
	else// if(theApp.processType == FILEDATA_PROCESSING)
	{
		minDepth = 0;
		maxDepth = 0;
		minDepthLimit = 0;
		maxDepthLimit = 0;
		m_drawCount = TIME_REFRESH_FILE/200;
		GetMaxMinData();//在绘图前进行一次计算的操作
		minDepth = (int)minDepthLimit;
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
#ifdef FEATURE_TEST_DRAW
			KillTimer(TIMER_CMD_TEST);
#endif
			if(theApp.processType == FILEDATA_PROCESSING)
			{
				//KillTimer(nIDEvent);//TIMER_CMD_DRAW
				
				Invalidate();
				return;
			}
			else if(theApp.processType == REALTIME_PROCESSING)
			{
				Invalidate();
				SetTimer(TIMER_CMD_DRAW,TIME_REFRESH_REALTIME,NULL);
			}
			else if(theApp.processType == NO_PROCESSING)
			{
				
			}
		}
		break;
#ifdef FEATURE_TEST_DRAW
	case TIMER_CMD_TEST:
		{
			if(theApp.processType == NO_PROCESSING)
			{
				static BOOL pros={FALSE};
				double y;
				if(!pros)
				{
					pros=TRUE;

					y =(double)(abs(rand())%2000);
					m_Plot.AddPoint(0,  CTime::GetCurrentTime(), y);

					y =(double)(abs(rand())%2000);
					m_Plot.AddPoint(1,  CTime::GetCurrentTime(), y);

					y =(double)(abs(rand())%2000);
					m_Plot.AddPoint(2,  CTime::GetCurrentTime(), y);

					//double y =(double)(abs(rand())%2000);
					y=500.0;
					m_Plot.AddPoint(3,  CTime::GetCurrentTime(), y);
					Invalidate();
					pros=FALSE;
				}
				
			}
			
		}
		break;
#endif
	default:
		break;
	}
	CScrollView::OnTimer(nIDEvent);
}
void CDataMonitorView::GetMaxMinData()
{
	if(!theApp.petroList.IsEmpty())
	{
		minDepthLimit = theApp.petroList.GetHead()->dept.iData;
		maxDepthLimit = minDepthLimit;
	}
	POSITION nPos = theApp.petroList.GetHeadPosition();
	while(nPos)
	{
		CPetroData* pData = theApp.petroList.GetNext(nPos);

		//查找当前最小深度值
		if(minDepthLimit > pData->dept.iData)
		{
			minDepthLimit = pData->dept.iData;
		}
		if(maxDepthLimit < pData->dept.iData)
		{
			maxDepthLimit = pData->dept.iData;
		}

		unsigned long m = 0;
		
		tempLimitArray[0] = GetMinData(pData->temp,tempLimitArray[0]);
		tempLimitArray[1] = max(tempLimitArray[1], pData->temp.iData/10*10+10);

		gmLimitArray[0] = GetMinData(pData->gr,gmLimitArray[0]);
		gmLimitArray[1] = max(gmLimitArray[1], pData->gr.iData/10*10+10);

		rmLimitArray[0] = GetMinData(pData->rm,rmLimitArray[0]);
		rmLimitArray[1] = max(rmLimitArray[1], pData->rm.iData/10*10+10);

		cclLimitArray[0] = GetMinData(pData->ccl,cclLimitArray[0]);
		cclLimitArray[1] = max(cclLimitArray[1], pData->ccl.iData/10*10+10);

		magLimitArray[0] = GetMinData(pData->mag[0],magLimitArray[0]);
		magLimitArray[1] = max(magLimitArray[1], pData->mag[0].iData/10*10+10);
	}
}

int CDataMonitorView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CScrollView::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  在此添加您专用的创建代码
	return 0;
}

void CDataMonitorView::InitArrayData()
{
	for(int i = 0;i<2;i++)
	{
		tempLimitArray[i] = 0;//0-min;1-max;
		gmLimitArray[i] = 0;
		rmLimitArray[i] = 0;
		cclLimitArray[i] = 0;
		magLimitArray[i] = 0;
	}
}
void CDataMonitorView::InitOldArrayData()
{
	memset((void*)&olddeptArray,0,sizeof(DATA_TEMP));
	memset((void*)&oldtempArray,0,sizeof(DATA_TEMP));
	memset((void*)&oldgmArray,0,sizeof(DATA_TEMP));
	memset((void*)&oldrmArray,0,sizeof(DATA_TEMP));
	memset((void*)&oldcclArray,0,sizeof(DATA_TEMP));
	memset((void*)&oldmagArray,0,sizeof(DATA_TEMP));
}

void CDataMonitorView::OnSize(UINT nType, int cx, int cy)
{
	CScrollView::OnSize(nType, cx, cy);

	// TODO: 在此处添加消息处理程序代码
	CRect Rect;
	GetClientRect(Rect);
#ifdef FEATURE_TEST_DRAW
	if(m_Plot.m_hWnd)
	{
		//m_Plot.MoveWindow(Rect);
	}
#endif
}

/////////////////////////////////////////////////////////////////////////////
// CDataMonitorView message handlers
//这里运用了两点技术使得重绘的图形看起来不闪烁
//1.重载了OnEraseBkgnd函数,它是用来在每次重绘之前用空白位图擦除背景的函数,让它每次用自定义的
//位图来作为背景
//2.使用了双缓存的结构,即先把背景图形绘制在内存中,绘制好了一起显示出去
BOOL CDataMonitorView::OnEraseBkgnd(CDC* pDC)
{
#if 0
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CDC MemDC; //首先定义一个显示设备对象
	CBitmap MemBitmap;//定义一个位图对象
	//随后建立与屏幕显示兼容的内存显示设备
	MemDC.CreateCompatibleDC(NULL);
	//这时还不能绘图，因为没有地方画
	//下面建立一个与屏幕显示兼容的位图，至于位图的大小嘛，可以用窗口的大小
	MemBitmap.CreateCompatibleBitmap(pDC,m_plot.m_ctlRect.Width(),m_plot.m_ctlRect.Height());
	//将位图选入到内存显示设备中
	//只有选入了位图的内存显示设备才有地方绘图，画到指定的位图上
	CBitmap *pOldBit=MemDC.SelectObject(&MemBitmap);
	//先用背景色将位图清除干净，这里我用的是白色作为背景
	//你也可以用自己应该用的颜色
	m_plot.DrawBasic(&MemDC);
//	m_plot.DrawLegendShadow(&MemDC);
	m_plot.DrawXAxisGrid(&MemDC);
	m_plot.DrawYAxisGrid(&MemDC);
//	m_plot.DrawLegend(&MemDC);
	//将内存中的图拷贝到屏幕上进行显示
	pDC->BitBlt(0,0,m_plot.m_ctlRect.Width(),m_plot.m_ctlRect.Height(),&MemDC,0,0,SRCCOPY);
	//绘图完成后的清理
	MemBitmap.DeleteObject();
	MemDC.DeleteDC();
	return TRUE;
#else
	//return CScrollView::OnEraseBkgnd(pDC);
	return TRUE;   
#endif
}
