
// DMonitorView.h : CDataMonitorView 类的接口
//

#pragma once

#include "DataMonitorDoc.h"
#include "clPlot.h"
class CDataMonitorView : public CScrollView
{
protected: // 仅从序列化创建
	CDataMonitorView();
	DECLARE_DYNCREATE(CDataMonitorView)

// 特性
public:
	CDataMonitorDoc* GetDocument() const;

// 操作
public:

protected:
	//why add 绘图变量
	int			unitPixel;//每米对应像素值
	CRect		m_screenRect;// m_screenRect - view的可见范围相对屏幕左上角坐标
	CRect		m_clientRect;// m_clientRect - view的可见范围客户区
	CRect		m_totalRect;// m_totalRect - 整个要绘制区域的范围，包括不可见

	CRect		m_plot1Rect;// m_plot1Rect - borderspace1
	CRect		m_plot2Rect;// m_plot2Rect - borderspace2
	CRect		m_plot3Rect;// m_plot3Rect - borderspace3

	SIZE sizeTotal;//整个要绘制区域的范围，包括不可见

	double		minDepth;//可见范围最小深度
	double		maxDepth;//可见范围最大深度

	bool		m_bAutoScrollY;		// automatic y range scrolling
	bool		m_bDirectDown;		// automatic x range scrolling
	//颜色
	COLORREF	m_gridColor;		// grid line color
	//字体设置
	CFont		m_font;
	LOGFONT		m_zoomFont;
	//双缓冲
	CDC   MemDC;          //定义一个显示设备对象  
	CBitmap   MemBitmap;  //定义一个位图对象  

	unsigned long tempArray[3];
	unsigned long gmArray[3];
	unsigned long rmArray[3];
	unsigned long cclArray[3];
	unsigned long magArray[3];

	DATA_TEMP olddeptArray;
	DATA_TEMP oldtempArray;
	DATA_TEMP oldgmArray;
	DATA_TEMP oldrmArray;
	DATA_TEMP oldcclArray;
	DATA_TEMP oldmagArray;
#ifdef FEATURE_TEST_DRAW
	clPlot m_Plot;
#endif

protected:
	//why add 绘图函数
	void DrawBasic(CDC * pDC);
	void DrawGrid(CDC * pDC);//绘制坐标网格
	void DrawXYAxisGrid(CDC * pDC);//绘制横纵坐标网格
	void DrawPlot(CDC* pDC);//绘制坐标数据

	void DrawData(CDC* pDC);
	void DrawDataFile(CDC* pDC);
	void SetScrollTotalSizes(CRect rect);
	void InitArrayData();
	void InitOldArrayData();
	void GetMaxMinData();
	
#ifdef FEATURE_TEST_DRAW
	void InitPlot(CRect Rect);
#endif
	void DrawCoordinateSystemFile(CDC* pDC);
#if 0
	void DrawDataArray(CDC* pDC);
	void DrawDataArrayFile(CDC* pDC);
#endif
	unsigned long GetMinData(DATA_PART tmp, unsigned long m);
	
// 重写
public:
	virtual void OnDraw(CDC* pDC);  // 重写以绘制该视图
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	void StartTimer();
	void StopTimer();
	void KillDrawTimer();

protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

private:
	unsigned long base;//深度起点
	unsigned long bias;//深度偏移量
	long counter;//计数器
	POSITION pos;//当前
	CRect rect;
	CRect rectTotal;
	CRect rectTotalFile;
	
	CPetroData* pPData;//得到队列数据
	CPetroData* pOldPData;//保存前一个数据队列
// 实现
public:
	virtual ~CDataMonitorView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 生成的消息映射函数
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPaint();
	virtual void OnInitialUpdate();
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
};

#ifndef _DEBUG  // DMonitorView.cpp 中的调试版本
inline CDataMonitorDoc* CDataMonitorView::GetDocument() const
   { return reinterpret_cast<CDataMonitorDoc*>(m_pDocument); }
#endif

