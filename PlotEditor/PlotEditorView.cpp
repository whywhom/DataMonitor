
// PlotEditorView.cpp : CPlotEditorView 类的实现
//

#include "stdafx.h"
// SHARED_HANDLERS 可以在实现预览、缩略图和搜索筛选器句柄的
// ATL 项目中进行定义，并允许与该项目共享文档代码。
#ifndef SHARED_HANDLERS
#include "PlotEditor.h"
#endif

#include "PlotEditorDoc.h"
#include "PlotEditorView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CPlotEditorView

IMPLEMENT_DYNCREATE(CPlotEditorView, CView)

BEGIN_MESSAGE_MAP(CPlotEditorView, CView)
	// 标准打印命令
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_WM_PAINT()
END_MESSAGE_MAP()

// CPlotEditorView 构造/析构

CPlotEditorView::CPlotEditorView()
{
	// TODO: 在此处添加构造代码

}

CPlotEditorView::~CPlotEditorView()
{
}

BOOL CPlotEditorView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	return CView::PreCreateWindow(cs);
}

// CPlotEditorView 绘制

void CPlotEditorView::OnDraw(CDC* pDC)
{
	CPlotEditorDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: 在此处为本机数据添加绘制代码
	DrawCoordinateSystem(pDC);
}


// CPlotEditorView 打印

BOOL CPlotEditorView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 默认准备
	return DoPreparePrinting(pInfo);
}

void CPlotEditorView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加额外的打印前进行的初始化过程
}

void CPlotEditorView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加打印后进行的清理过程
}


// CPlotEditorView 诊断

#ifdef _DEBUG
void CPlotEditorView::AssertValid() const
{
	CView::AssertValid();
}

void CPlotEditorView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CPlotEditorDoc* CPlotEditorView::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CPlotEditorDoc)));
	return (CPlotEditorDoc*)m_pDocument;
}
#endif //_DEBUG


// CPlotEditorView 消息处理程序


void CPlotEditorView::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: 在此处添加消息处理程序代码
	// 不为绘图消息调用 CView::OnPaint()
    OnPrepareDC(&dc);
	OnDraw(&dc);
}

void CPlotEditorView::DrawCoordinateSystem(CDC* pDC)
{
	TRACE(_T("DrawCoordinateSystem"));
	//获得线的颜色颜色
	COLORREF color = RGB(127,127,127);
	
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
	UpdateData(FALSE);

	int iCount=0;

	for (int i=0,iCount = 0;i<=rect.Width();i+=20,iCount++)
	{
		pDC->MoveTo(i,0); // [0,0]
		pDC->LineTo(i,rect.Height());
		if(iCount != 0 && iCount%5 == 0)
		{
			pDC->MoveTo(i+1,0); // [0,0]
			pDC->LineTo(i+1,rect.Height());
		}
	}
	for (int j=0,iCount = 0;j<=rect.Height();j+=20,iCount++)
	{
		pDC->MoveTo(0,j); // [0,0]
		pDC->LineTo(rect.Width(),j);
		if(iCount != 0 && iCount%5 == 0)
		{
			pDC->MoveTo(0,j+1); // [0,0]
			pDC->LineTo(rect.Width(),j+1);
		}
	}
	//画笔刷新
	pDC->SelectObject(pOldPen);  //[可以不需要]
}