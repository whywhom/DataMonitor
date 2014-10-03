
// DMonitorView.cpp : CDataMonitorView 类的实现
//

#include "stdafx.h"
// SHARED_HANDLERS 可以在实现预览、缩略图和搜索筛选器句柄的
// ATL 项目中进行定义，并允许与该项目共享文档代码。
#ifndef SHARED_HANDLERS
#include "DataMonitor.h"
#endif
#include "MainFrm.h"
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
	ON_WM_CLOSE()
	ON_WM_DESTROY()
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
	maxPreDepth = 0;
	minDepthLimit = 0;
	maxDepthLimit = 0;
	
	m_drawCount = 0;
	bScroll = false;
	m_step = 5;//每次移动步长为5米
	pPData = NULL;
	pOldPData = NULL;

	pos = NULL;//当前记录位置

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

bool CDataMonitorView::GetDirectionDown( )
{
	return m_bDirectDown;
}

void CDataMonitorView::DrawBasic(CDC * pDC)
{
	m_totalRect = m_clientRect;
	maxPreDepth = maxDepth;
	int depPixel = 0;
	if(theApp.processType == REALTIME_PROCESSING)
	{
		CPetroData * pHeadData = theApp.petroList.GetHead();
		CPetroData * pTailData = theApp.petroList.GetTail();
		
		if(m_bDirectDown)
		{
			minDepthLimit = pHeadData->dept.iData;
			maxDepthLimit = pTailData->dept.iData;
			if(bScroll)
			{
				maxDepth = maxDepthLimit;
				minDepth = maxDepth - m_clientRect.Height()/unitPixel;
				depPixel = (int)((maxDepth - minDepthLimit)*10/10) * unitPixel;
			}
			else
			{
				minDepth = minDepthLimit;
				maxDepth = minDepth + m_clientRect.Height()/unitPixel;
				depPixel = (int)((maxDepth - minDepthLimit)*10/10) * unitPixel;
				if(maxDepthLimit >= maxDepth)
				{
					bScroll = true;
				}
			}
		}
		else
		{
			maxDepthLimit = pHeadData->dept.iData;
			minDepthLimit = pTailData->dept.iData;
			if(bScroll)
			{
				minDepth = minDepthLimit;
				maxDepth = minDepth + m_clientRect.Height()/unitPixel;
				depPixel = (int)((maxDepth - minDepthLimit)*10/10) * unitPixel;
			}
			else
			{
				maxDepth = maxDepthLimit;
				minDepth = maxDepth - m_clientRect.Height()/unitPixel;
				depPixel = (int)((maxDepth - minDepthLimit)*10/10) * unitPixel;
				if(minDepthLimit <= minDepth)
				{
					bScroll = true;
				}
			}
		}
	}
	else if(theApp.processType == FILEDATA_PROCESSING)
	{
		if(m_bDirectDown)
		{
			maxDepth = minDepth + m_clientRect.Height()/unitPixel;
			depPixel = (int)((maxDepth - minDepthLimit)*10/10) * unitPixel;
		}
		else
		{
			minDepth = maxDepth - m_clientRect.Height()/unitPixel;
			depPixel = (int)((maxDepthLimit - minDepth)*10/10) * unitPixel;
		}
	}
	else
	{
	
	}
	//int depPixel = (int)((maxDepth - minDepthLimit)*10/10) * unitPixel;
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
	m_plot3Rect.right = m_plot3Rect.left + 400;

	m_gridColor = RGB(127,127,127);

	CPen *old, pen(PS_SOLID, 1, m_gridColor), pen2(PS_DOT, 1,m_gridColor); //画笔;
	CPen stick(PS_SOLID,0,RGB(0,0,0));
	CPen mline(PS_SOLID,0,RGB(192,192,192));
	old = pDC->SelectObject(&pen);
	//绘制坐标系
	int iCount=0;

	for (int i=m_plot1Rect.left,iCount= 0;i<=m_plot1Rect.right;i+=unitPixel*unitRatio,iCount++)
	{
		if(iCount != 0 && iCount%5 == 0)
		{
			pDC->SelectObject(&pen);
			pDC->MoveTo(i,m_plot1Rect.top); //
			pDC->LineTo(i,m_plot1Rect.bottom);
			//pDC->MoveTo(i+1,m_plot1Rect.top); //
			//pDC->LineTo(i+1,m_plot1Rect.bottom);
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
			//pDC->MoveTo(i+1,m_plot3Rect.top);
			//pDC->LineTo(i+1,m_plot3Rect.bottom);
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
		
		if(iCount != 0 && (iCount)%10 == 0)
		{
			pDC->SelectObject(&pen);
			pDC->MoveTo(m_plot1Rect.left,j); 
			pDC->LineTo(m_plot1Rect.right,j);
			//pDC->MoveTo(m_plot1Rect.left,j+1); 
			//pDC->LineTo(m_plot1Rect.right,j+1);
		}
		else if(iCount != 0 && (iCount)%5 == 0)
		{
			pDC->SelectObject(&pen2);
			pDC->MoveTo(m_plot1Rect.left,j); 
			pDC->LineTo(m_plot1Rect.right,j);
		}
	}
	for (int j=m_plot3Rect.top,iCount = 0;j<=m_plot3Rect.bottom;j+=unitPixel,iCount++)
	{
		if(iCount != 0 && (iCount)%10 == 0)
		{
			pDC->SelectObject(&pen);
			pDC->MoveTo(m_plot3Rect.left,j);
			pDC->LineTo(m_plot3Rect.right,j);
			//pDC->MoveTo(m_plot3Rect.left,j+1);
			//pDC->LineTo(m_plot3Rect.right,j+1);
		}
		else if(iCount != 0 && (iCount)%5 == 0)
		{
			pDC->SelectObject(&pen2);
			pDC->MoveTo(m_plot3Rect.left,j); 
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
	if(theApp.processType == REALTIME_PROCESSING)
	{
		if(m_bDirectDown)
		{
			pos = theApp.petroList.GetTailPosition();
			while(pos)
			{
				pPData = theApp.petroList.GetPrev(pos);
				if(pPData->dept.bAssign == true)
				{
					if(bScroll)
					{
						if(pPData->dept.iData > minDepth)
						{
							DrawDeptData(pDC,pPData,&pen);//深度
							DrawTempData(pDC,pPData,&pen);//draw tepm
							DrawRmData(pDC,pPData,&pen2);//rm
							DrawGmData(pDC,pPData,&pen3);//gm
							DrawCclData(pDC,pPData,&pen4);//ccl
							DrawMagxData(pDC,pPData,&pen5);//magx
						}
						else
						{
							break;
						}
					}
					else
					{
						DrawDeptData(pDC,pPData,&pen);//深度
						DrawTempData(pDC,pPData,&pen);//draw tepm
						DrawRmData(pDC,pPData,&pen2);//rm
						DrawGmData(pDC,pPData,&pen3);//gm
						DrawCclData(pDC,pPData,&pen4);//ccl
						DrawMagxData(pDC,pPData,&pen5);//magx
					}
				}
		
			}
			pPData = theApp.petroList.GetTail();
			UpdatePanelListView(pPData);
		}
		else
		{
			pos = theApp.petroList.GetTailPosition();
			while(pos)
			{
				pPData = theApp.petroList.GetPrev(pos);
				if(pPData->dept.bAssign == true)
				{
					if(bScroll)
					{
						if(pPData->dept.iData < maxDepth)
						{
							DrawDeptData(pDC,pPData,&pen);//深度
							DrawTempData(pDC,pPData,&pen);//draw tepm
							DrawRmData(pDC,pPData,&pen2);//rm
							DrawGmData(pDC,pPData,&pen3);//gm
							DrawCclData(pDC,pPData,&pen4);//ccl
							DrawMagxData(pDC,pPData,&pen5);//magx
						}
						else
						{
							break;
						}
					}
					else
					{
						DrawDeptData(pDC,pPData,&pen);//深度
						DrawTempData(pDC,pPData,&pen);//draw tepm
						DrawRmData(pDC,pPData,&pen2);//rm
						DrawGmData(pDC,pPData,&pen3);//gm
						DrawCclData(pDC,pPData,&pen4);//ccl
						DrawMagxData(pDC,pPData,&pen5);//magx
					}
				}
		
			}
			UpdatePanelListView(pPData);
		}	
	}
	else if(theApp.processType == FILEDATA_PROCESSING)
	{
		if(m_bDirectDown)
		{
			pos = theApp.petroList.GetHeadPosition();
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
						if(pPData->dept.iData < maxDepth && (!bScroll))
						{
							if(mCounter < m_drawCount*m_iterator)
							{
								if(pPData->dept.bAssign)
								{
									DrawDeptData(pDC,pPData,&pen);//深度
									DrawTempData(pDC,pPData,&pen);//draw tepm
									DrawRmData(pDC,pPData,&pen2);//rm
									DrawGmData(pDC,pPData,&pen3);//gm
									DrawCclData(pDC,pPData,&pen4);//ccl
									DrawMagxData(pDC,pPData,&pen5);//magx
								}
								mCounter++;
							}
							else
							{
								m_iterator++;
								pDC->SelectObject(pOldPen);
								break;
							}
						}
						else if(pPData->dept.iData < maxDepth && bScroll)
						{
							if(pPData->dept.bAssign)
							{
								DrawDeptData(pDC,pPData,&pen);//深度
								DrawTempData(pDC,pPData,&pen);//draw tepm
								DrawRmData(pDC,pPData,&pen2);//rm
								DrawGmData(pDC,pPData,&pen3);//gm
								DrawCclData(pDC,pPData,&pen4);//ccl
								DrawMagxData(pDC,pPData,&pen5);//magx
							}
						}
						else
						{
							if(!bScroll)
							{
								bScroll = true;
							}
							minDepth =(int) (minDepth+ m_step);
							m_iterator = 1;
							mCounter = 0;
							break;
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
				UpdatePanelListView(pPData);
				SetTimer(TIMER_CMD_DRAW,TIME_REFRESH_FILE,NULL);
			}
		}
		else
		{
			pos = theApp.petroList.GetTailPosition();
			while(pos)
			{
				pPData = theApp.petroList.GetPrev(pos);
				if(pPData->dept.bAssign == true)
				{
					if(pPData->dept.iData > maxDepth)
					{
						continue;
					}
					else
					{
						if(pPData->dept.iData > minDepth && (!bScroll))
						{
							if(mCounter < m_drawCount*m_iterator)
							{
								if(pPData->dept.bAssign)
								{
									DrawDeptData(pDC,pPData,&pen);//深度
									DrawTempData(pDC,pPData,&pen);//draw tepm
									DrawRmData(pDC,pPData,&pen2);//rm
									DrawGmData(pDC,pPData,&pen3);//gm
									DrawCclData(pDC,pPData,&pen4);//ccl
									DrawMagxData(pDC,pPData,&pen5);//magx
								}
								mCounter++;
							}
							else
							{
								m_iterator++;
								pDC->SelectObject(pOldPen);
								break;
							}
						}
						else if(pPData->dept.iData > minDepth && bScroll)
						{
							if(pPData->dept.bAssign)
							{
								DrawDeptData(pDC,pPData,&pen);//深度
								DrawTempData(pDC,pPData,&pen);//draw tepm
								DrawRmData(pDC,pPData,&pen2);//rm
								DrawGmData(pDC,pPData,&pen3);//gm
								DrawCclData(pDC,pPData,&pen4);//ccl
								DrawMagxData(pDC,pPData,&pen5);//magx
							}
						}
						else
						{
							if(!bScroll)
							{
								bScroll = true;
							}
							maxDepth =(int) (maxDepth - m_step);
							m_iterator = 1;
							mCounter = 0;
							break;
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
				UpdatePanelListView(pPData);
				SetTimer(TIMER_CMD_DRAW,TIME_REFRESH_FILE,NULL);
			}
		}	
	}
	else
	{

	}
}

void CDataMonitorView::DrawDeptData(CDC* pDC ,CPetroData* pPData,CPen* pPpen)
{
	if(olddeptArray.bAssign)
	{
							
	}
	else
	{
						
	}
}
void CDataMonitorView::DrawTempData(CDC* pDC ,CPetroData* pPData,CPen* pPpen)
{
	long pre_iy=0,cur_iy=0;
	long pre_ix=0,cur_ix=0;
	if(pPData->temp.bAssign)
	{
		if(oldtempArray.bAssign)
		{
			pDC->SelectObject(pPpen);
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
		//else
		{
			oldtempArray.bAssign = pPData->temp.bAssign;
			oldtempArray.dx = pPData->temp.iData;
			oldtempArray.dy = pPData->dept.iData;
			oldtempArray.strDx = pPData->temp.strData;
		}
	}
}
void CDataMonitorView::DrawRmData(CDC* pDC ,CPetroData* pPData,CPen* pPpen)
{
	long pre_iy=0,cur_iy=0;
	long pre_ix=0,cur_ix=0;
	if(pPData->rm.bAssign)
	{
		if(oldtempArray.bAssign)
		{
			pDC->SelectObject(pPpen);
			long rmRange = rmLimitArray[1]-rmLimitArray[0];
			int i_dx = (int)((oldrmArray.dx - (int)oldrmArray.dx)*10);
			int d_dx = (int)(oldrmArray.dx);
			pre_ix = (int)((d_dx-rmLimitArray[0])*m_plot1Rect.Width()/rmRange
				+i_dx*m_plot1Rect.Width()/(10*rmRange));
			double mid = (pPData->rm.iData-rmLimitArray[0])*m_plot1Rect.Width()/rmRange;
			cur_ix = (int)mid;
			pre_iy = (oldrmArray.dy - minDepth)*unitPixel;
			cur_iy = (pPData->dept.iData - minDepth)*unitPixel;
			pDC->MoveTo(pre_ix,pre_iy);
			pDC->LineTo(cur_ix,cur_iy);
		}
		//else
		{
			oldrmArray.bAssign = pPData->rm.bAssign;
			oldrmArray.dx = pPData->rm.iData;
			oldrmArray.dy = pPData->dept.iData;
			oldrmArray.strDx = pPData->rm.strData;
		}
	}
}
//绘制磁三分量曲线
void CDataMonitorView::DrawMagxData(CDC* pDC ,CPetroData* pPData,CPen* pPpen)
{
	int iDrawType = PS_SOLID;
	COLORREF color1 = RGB(255,0,255);
	COLORREF color2 = RGB(0,255,255);

	CPen penmag0(iDrawType, 1,color1); //画笔
	CPen penmag1(iDrawType, 1,color1); //画笔2
	CPen penmag2(iDrawType, 1,color2); //画笔3
	
	long pre_iy=0,cur_iy=0;
	long pre_ix=0,cur_ix=0;
	//0
	if(pPData->mag[0].bAssign)
	{
		if(oldmagArray[0].bAssign)
		{
			pDC->SelectObject(pPpen);
			long magRange = magLimitArray[0][1]-magLimitArray[0][0];
			int i_dx = (int)((oldmagArray[0].dx - (int)oldmagArray[0].dx)*10);
			int d_dx = (int)(oldmagArray[0].dx);
			pre_ix = (int)((d_dx-magLimitArray[0][0])*m_plot1Rect.Width()/magRange
				+i_dx*m_plot1Rect.Width()/(10*magRange));
			double mid = (pPData->mag[0].iData-magLimitArray[0][0])*m_plot1Rect.Width()/magRange;
			cur_ix = (int)mid;
			pre_iy = (oldmagArray[0].dy - minDepth)*unitPixel;
			cur_iy = (pPData->dept.iData - minDepth)*unitPixel;
			pDC->MoveTo(pre_ix,pre_iy);
			pDC->LineTo(cur_ix,cur_iy);
		}
		//else
		{
			oldmagArray[0].bAssign = pPData->mag[0].bAssign;
			oldmagArray[0].dx = pPData->mag[0].iData;
			oldmagArray[0].dy = pPData->dept.iData;
			oldmagArray[0].strDx = pPData->mag[0].strData;
		}
	}
	//1
	if(pPData->mag[1].bAssign)
	{
		if(oldmagArray[1].bAssign)
		{
			pDC->SelectObject(&penmag1);
			long magRange = magLimitArray[1][1]-magLimitArray[1][0];
			int i_dx = (int)((oldmagArray[1].dx - (int)oldmagArray[1].dx)*10);
			int d_dx = (int)(oldmagArray[1].dx);
			pre_ix = (int)((d_dx-magLimitArray[1][0])*m_plot1Rect.Width()/magRange
				+i_dx*m_plot1Rect.Width()/(10*magRange));
			double mid = (pPData->mag[1].iData-magLimitArray[1][0])*m_plot1Rect.Width()/magRange;
			cur_ix = (int)mid;
			pre_iy = (oldmagArray[1].dy - minDepth)*unitPixel;
			cur_iy = (pPData->dept.iData - minDepth)*unitPixel;
			pDC->MoveTo(pre_ix,pre_iy);
			pDC->LineTo(cur_ix,cur_iy);
		}
		//else
		{
			oldmagArray[1].bAssign = pPData->mag[1].bAssign;
			oldmagArray[1].dx = pPData->mag[1].iData;
			oldmagArray[1].dy = pPData->dept.iData;
			oldmagArray[1].strDx = pPData->mag[1].strData;
		}
	}
	//2
	if(pPData->mag[2].bAssign)
	{
		if(oldmagArray[2].bAssign)
		{
			pDC->SelectObject(&penmag2);
			long magRange = magLimitArray[2][1]-magLimitArray[2][0];
			int i_dx = (int)((oldmagArray[2].dx - (int)oldmagArray[2].dx)*10);
			int d_dx = (int)(oldmagArray[2].dx);
			pre_ix = (int)((d_dx-magLimitArray[2][0])*m_plot1Rect.Width()/magRange
				+i_dx*m_plot1Rect.Width()/(10*magRange));
			double mid = (pPData->mag[2].iData-magLimitArray[2][0])*m_plot1Rect.Width()/magRange;
			cur_ix = (int)mid;
			pre_iy = (oldmagArray[2].dy - minDepth)*unitPixel;
			cur_iy = (pPData->dept.iData - minDepth)*unitPixel;
			pDC->MoveTo(pre_ix,pre_iy);
			pDC->LineTo(cur_ix,cur_iy);
		}
		//else
		{
			oldmagArray[2].bAssign = pPData->mag[2].bAssign;
			oldmagArray[2].dx = pPData->mag[2].iData;
			oldmagArray[2].dy = pPData->dept.iData;
			oldmagArray[2].strDx = pPData->mag[2].strData;
		}
	}
}
//绘制Gm曲线
void CDataMonitorView::DrawGmData(CDC* pDC ,CPetroData* pPData,CPen* pPpen)
{
	long pre_iy=0,cur_iy=0;
	long pre_ix=0,cur_ix=0;
	if(pPData->gr.bAssign)
	{
		if(oldgmArray.bAssign)
		{
			pDC->SelectObject(pPpen);
			long gmRange = gmLimitArray[1]-gmLimitArray[0];
			int i_dx = (int)((oldgmArray.dx - (int)oldgmArray.dx)*10);
			int d_dx = (int)(oldgmArray.dx);
			pre_ix = (int)((d_dx-gmLimitArray[0])*m_plot1Rect.Width()/gmRange
				+i_dx*m_plot1Rect.Width()/(10*gmRange));
			double mid = (pPData->gr.iData-gmLimitArray[0])*m_plot1Rect.Width()/gmRange;
			cur_ix = (int)mid;
			pre_iy = (oldgmArray.dy - minDepth)*unitPixel;
			cur_iy = (pPData->dept.iData - minDepth)*unitPixel;
			pDC->MoveTo(pre_ix,pre_iy);
			pDC->LineTo(cur_ix,cur_iy);
		}
		//else
		{
			oldgmArray.bAssign = pPData->gr.bAssign;
			oldgmArray.dx = pPData->gr.iData;
			oldgmArray.dy = pPData->dept.iData;
			oldgmArray.strDx = pPData->gr.strData;
		}
	}
}
//绘制Ccl曲线
void CDataMonitorView::DrawCclData(CDC* pDC ,CPetroData* pPData,CPen* pPpen)
{
	long pre_iy=0,cur_iy=0;
	long pre_ix=0,cur_ix=0;
	if(pPData->ccl.bAssign)
	{
		if(oldcclArray.bAssign)
		{
			pDC->SelectObject(pPpen);
			long cclRange = cclLimitArray[1]-cclLimitArray[0];
			int i_dx = (int)((oldcclArray.dx - (int)oldcclArray.dx)*10);
			int d_dx = (int)(oldcclArray.dx);
			pre_ix = (int)((d_dx-cclLimitArray[0])*m_plot1Rect.Width()/cclRange
				+i_dx*m_plot1Rect.Width()/(10*cclRange));
			double mid = (pPData->ccl.iData-cclLimitArray[0])*m_plot1Rect.Width()/cclRange;
			cur_ix = (int)mid;
			pre_iy = (oldcclArray.dy - minDepth)*unitPixel;
			cur_iy = (pPData->dept.iData - minDepth)*unitPixel;
			pDC->MoveTo(pre_ix,pre_iy);
			pDC->LineTo(cur_ix,cur_iy);
		}
		//else
		{
			oldcclArray.bAssign = pPData->ccl.bAssign;
			oldcclArray.dx = pPData->ccl.iData;
			oldcclArray.dy = pPData->dept.iData;
			oldcclArray.strDx = pPData->ccl.strData;
		}
	}
}


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
	if(theApp.processType == REALTIME_PROCESSING)
	{
		DrawBasic(pDC);//获取内存映射绘制区域大小
		DrawGrid(pDC);
		DrawPlot(pDC);
		DrawCurve(pDC,minDepth,maxDepth);
	}
	else
	{
		DrawBasic(pDC);//获取内存映射绘制区域大小
		DrawGrid(pDC);
		DrawPlot(pDC);
		DrawCurve(pDC,minDepth,maxDepth);
	}
}

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
	m_drawCount = 0;
	bScroll = false;
	m_step = 5;//每次移动步长为5米
	pPData = NULL;
	pOldPData = NULL;
	pos = NULL;//当前记录位置
	if(theApp.petroList.IsEmpty())
	{
		return;//没有数据不进行绘制
	}
	if(m_bDirectDown)
	{
		pos = theApp.petroList.GetHeadPosition();
	}
	else
	{
		pos = theApp.petroList.GetTailPosition();
	}

	if(theApp.processType == REALTIME_PROCESSING)
	{
		minDepth = 0;
		maxDepth = 0;
		minDepthLimit = 0;
		maxDepthLimit = 0;
		bScroll = false;
		AddPanelListView();
		SetTimer(TIMER_CMD_DRAW,TIME_REFRESH_REALTIME,NULL);
	}
	else// if(theApp.processType == FILEDATA_PROCESSING)
	{
		minDepth = 0;
		maxDepth = 0;
		minDepthLimit = 0;
		maxDepthLimit = 0;
		bScroll = false;
		m_drawCount = TIME_REFRESH_FILE/20;
		GetMaxMinData();//在绘图前进行一次计算的操作
		AddPanelListView();
		if(m_bDirectDown)
		{
			minDepth = (int)minDepthLimit;
		}
		else
		{
			maxDepth = (int)maxDepthLimit+1;
		}
		SetTimer(TIMER_CMD_DRAW,TIME_REFRESH_FILE,NULL);
	}
}
void CDataMonitorView::AddPanelListView( )
{
	CMainFrame*  pFrame=(CMainFrame*)AfxGetMainWnd();  
	if(pFrame)
	{
		pFrame->pPanelView->listView.SetRedraw(FALSE);
		pFrame->pPanelView->listView.DeleteAllItems();
		pFrame->pPanelView->listView.InsertItem(0,_T("DEPT"));
		pFrame->pPanelView->listView.InsertItem(1,_T("TEMP"));
		pFrame->pPanelView->listView.InsertItem(2,_T("RM"));
		pFrame->pPanelView->listView.InsertItem(3,_T("GM"));
		pFrame->pPanelView->listView.InsertItem(4,_T("MAGX"));
		pFrame->pPanelView->listView.InsertItem(5,_T("CCL"));
		pFrame->pPanelView->listView.SetRedraw(TRUE);
		pFrame->pPanelView->listView.Invalidate();
		pFrame->pPanelView->listView.UpdateWindow();
	}
}
void CDataMonitorView::UpdatePanelListView(CPetroData* pPData)
{
	CMainFrame*  pFrame=(CMainFrame*)AfxGetMainWnd();  
	if(pFrame)
	{
		pFrame->pPanelView->Edit01.SetWindowText(pPData->dept.strData);

		pFrame->pPanelView->listView.SetRedraw(FALSE);
		pFrame->pPanelView->listView.SetItemText(0,1,pPData->dept.strData);
		pFrame->pPanelView->listView.SetItemText(0,2,pPData->dept.strUnit);

		pFrame->pPanelView->listView.SetItemText(1,1,oldtempArray.strDx);
		pFrame->pPanelView->listView.SetItemText(1,2,pPData->temp.strUnit);

		pFrame->pPanelView->listView.SetItemText(2,1,oldrmArray.strDx);
		pFrame->pPanelView->listView.SetItemText(2,2,pPData->rm.strUnit);

		pFrame->pPanelView->listView.SetItemText(3,1,oldgmArray.strDx);
		pFrame->pPanelView->listView.SetItemText(3,2,pPData->gr.strUnit);

		CString str;
		str += oldmagArray[0].strDx;
		str += _T(",");
		str += oldmagArray[1].strDx;
		str += _T(",");
		str += oldmagArray[2].strDx;
		pFrame->pPanelView->listView.SetItemText(4,1,str);
		pFrame->pPanelView->listView.SetItemText(4,2,pPData->mag[0].strUnit);

		pFrame->pPanelView->listView.SetItemText(5,1,oldcclArray.strDx);
		pFrame->pPanelView->listView.SetItemText(5,2,pPData->ccl.strUnit);

		pFrame->pPanelView->listView.SetRedraw(TRUE);
		pFrame->pPanelView->listView.Invalidate();
		pFrame->pPanelView->listView.UpdateWindow();
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
			if(theApp.processType == FILEDATA_PROCESSING)
			{				
				Invalidate();
				return;
			}
			else if(theApp.processType == REALTIME_PROCESSING)
			{
				if(!theApp.petroList.IsEmpty())
				{
					CalculateParam();
					Invalidate();
				}
				SetTimer(TIMER_CMD_DRAW,TIME_REFRESH_REALTIME,NULL);
			}
			else if(theApp.processType == NO_PROCESSING)
			{
				
			}
		}
		break;
	default:
		break;
	}
	CScrollView::OnTimer(nIDEvent);
}
void CDataMonitorView::CalculateParam()
{
	CPetroData * pHeadData = theApp.petroList.GetHead();
	CPetroData * pTailData = theApp.petroList.GetTail();
	if(pHeadData->dept.iData < pTailData->dept.iData)
	{
		m_bDirectDown = true;
	}
	else
	{
		m_bDirectDown = false;
	}
	
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

		magLimitArray[0][0] = GetMinData(pData->mag[0],magLimitArray[0][0]);
		magLimitArray[0][1] = max(magLimitArray[0][1], pData->mag[0].iData/10*10+10);

		magLimitArray[1][0] = GetMinData(pData->mag[1],magLimitArray[1][0]);
		magLimitArray[1][1] = max(magLimitArray[1][1], pData->mag[1].iData/10*10+10);

		magLimitArray[2][0] = GetMinData(pData->mag[2],magLimitArray[2][0]);
		magLimitArray[2][1] = max(magLimitArray[2][1], pData->mag[2].iData/10*10+10);
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
		magLimitArray[i][0] = 0;
		magLimitArray[i][1] = 0;
	}
}
void CDataMonitorView::InitOldArrayData()
{
	olddeptArray.dx = 0;
	olddeptArray.dy = 0;
	olddeptArray.bAssign = false;
	olddeptArray.strDx = _T("");

	oldtempArray.dx = 0;
	oldtempArray.dy = 0;
	oldtempArray.bAssign = false;
	oldtempArray.strDx = _T("");

	oldgmArray.dx = 0;
	oldgmArray.dy = 0;
	oldgmArray.bAssign = false;
	oldgmArray.strDx = _T("");

	oldrmArray.dx = 0;
	oldrmArray.dy = 0;
	oldrmArray.bAssign = false;
	oldrmArray.strDx = _T("");

	oldcclArray.dx = 0;
	oldcclArray.dy = 0;
	oldcclArray.bAssign = false;
	oldcclArray.strDx = _T("");

	oldmagArray[0].dx = 0;
	oldmagArray[0].dy = 0;
	oldmagArray[0].bAssign = false;
	oldmagArray[0].strDx = _T("");

	oldmagArray[1].dx = 0;
	oldmagArray[1].dy = 0;
	oldmagArray[1].bAssign = false;
	oldmagArray[1].strDx = _T("");

	oldmagArray[2].dx = 0;
	oldmagArray[2].dy = 0;
	oldmagArray[2].bAssign = false;
	oldmagArray[2].strDx = _T("");
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


void CDataMonitorView::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	
	CScrollView::OnClose();
}


void CDataMonitorView::OnDestroy()
{
	CScrollView::OnDestroy();

	// TODO: 在此处添加消息处理程序代码
	TRACE(_T("CDataMonitorView::OnClose() \r\n"));
	KillTimer(TIMER_CMD_DRAW);
	TRACE(_T("KillTimer(TIMER_CMD_DRAW) \r\n"));
}
