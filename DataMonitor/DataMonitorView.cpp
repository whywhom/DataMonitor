
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
	InitArrayData();

	InitOldArrayData();

	base = 0;
	bias = 0;
	pPData = NULL;
	pOldPData = NULL;
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
		dept = pPData->dept.integer;
		dold_iy=pPData->dept.integer;
		cur_dy=pPData->dept.decimal;
		dold_iy-=base;

		if(pOldPData != NULL)
		{
			pDC->SelectObject(&pen);
			old_iy=pOldPData->dept.integer;
			old_dy=pOldPData->dept.decimal;
			old_iy-=base;

			y1=pOldPData->temp.integer;
			y2=pOldPData->temp.decimal;

			int iOldDept_y = (int)(old_dy*gap/10);
			int iOld_ix = (int)(pOldPData->temp.integer*gap1/(tempArray[1]-tempArray[0])+pOldPData->temp.decimal*gap1/(10*(tempArray[1]-tempArray[0])));

			int iCurDept_y = (int)(cur_dy*gap/10);
			int iCur_ix = (int)(pPData->temp.integer*gap1/(tempArray[1]-tempArray[0])+pPData->temp.decimal*gap1/(10*(tempArray[1]-tempArray[0])));

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
				
				cur_iy=pPData->dept.integer;
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
						iCur_ix = (int)((pPData->temp.integer-tempArray[0])*gap1/(tempArray[1]-tempArray[0])+pPData->temp.decimal*gap1/(10*(tempArray[1]-tempArray[0])));

						pDC->MoveTo(iOld_ix,oldtempArray.dy); // [0,0]
						pDC->LineTo(iCur_ix,cur_iy*gap+iCurDept_y);
						TRACE(_T("TEMP %d : from (%d , %d ) to (%d , %d) \r\n"),counter,iOld_ix,oldtempArray.dy,iCur_ix,cur_iy*gap+iCurDept_y);
					}
					
					oldtempArray.dx_i = pPData->temp.integer;
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
						iCur_ix = (int)((pPData->gr.integer-gmArray[0])*gap1/(gmArray[1]-gmArray[0])+pPData->gr.decimal*gap1/(10*(gmArray[1]-gmArray[0])));

						pDC->MoveTo(iOld_ix,oldgmArray.dy); // [0,0]
						pDC->LineTo(iCur_ix,cur_iy*gap+iCurDept_y);
						TRACE(_T("GR %d : from (%d , %d ) to (%d , %d) \r\n"),counter,iOld_ix,oldgmArray.dy,iCur_ix,cur_iy*gap+iCurDept_y);
					}
					
					oldgmArray.dx_i = pPData->gr.integer;
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
						iCur_ix = (int)((pPData->rm.integer-rmArray[0])*gap1/(rmArray[1]-rmArray[0])+pPData->rm.decimal*gap1/(10*(rmArray[1]-rmArray[0])));

						pDC->MoveTo(iOld_ix,oldrmArray.dy); // [0,0]
						pDC->LineTo(iCur_ix,cur_iy*gap+iCurDept_y);
						TRACE(_T("RM %d : from (%d , %d ) to (%d , %d) \r\n"),counter,iOld_ix,oldgmArray.dy,iCur_ix,cur_iy*gap+iCurDept_y);
					}
					
					oldrmArray.dx_i = pPData->rm.integer;
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
						iCur_ix = (int)((pPData->ccl.integer-cclArray[0])*gap1/(cclArray[1]-cclArray[0])+pPData->ccl.decimal*gap1/(10*(cclArray[1]-cclArray[0])));

						pDC->MoveTo(iOld_ix,oldcclArray.dy); // [0,0]
						pDC->LineTo(iCur_ix,cur_iy*gap+iCurDept_y);
						TRACE(_T("CCL %d : from (%d , %d ) to (%d , %d) \r\n"),counter,iOld_ix,oldcclArray.dy,iCur_ix,cur_iy*gap+iCurDept_y);
					}
					
					oldcclArray.dx_i = pPData->ccl.integer;
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
						iCur_ix = (int)((pPData->mag[0].integer-magArray[0])*gap1/(magArray[1]-magArray[0])+pPData->mag[0].decimal*gap1/(10*(magArray[1]-magArray[0])));

						pDC->MoveTo(iOld_ix,oldmagArray.dy); // [0,0]
						pDC->LineTo(iCur_ix,cur_iy*gap+iCurDept_y);
						TRACE(_T("CCL %d : from (%d , %d ) to (%d , %d) \r\n"),counter,iOld_ix,oldmagArray.dy,iCur_ix,cur_iy*gap+iCurDept_y);
					}
					
					oldmagArray.dx_i = pPData->mag[0].integer;
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
	DrawData();
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
				KillTimer(nIDEvent);
				GetDataLimit();
				DrawData();
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
void CDataMonitorView::GetDataLimit()
{
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

void CDataMonitorView::InitArrayData()
{
	for(int i = 0;i<3;i++)
	{
		tempArray[i] = 0;//0-整数部分;1-小数部分;2-对应的深度坐标
		gmArray[i] = 0;
		rmArray[i] = 0;
		cclArray[i] = 0;
		magArray[i] = 0;
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