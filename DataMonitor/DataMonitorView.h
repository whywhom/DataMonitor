
// DMonitorView.h : CDataMonitorView 类的接口
//

#pragma once

#include "DataMonitorDoc.h"

class CDataMonitorView : public CScrollView
{
protected: // 仅从序列化创建
	CDataMonitorView();
	DECLARE_DYNCREATE(CDataMonitorView)

	void DrawData(CDC* pDC);
	void DrawDataFile(CDC* pDC);
	void SetScrollTotalSizes(CRect rect);
	void InitArrayData();
	void InitOldArrayData();
	void GetDataLimit();
// 特性
public:
	CDataMonitorDoc* GetDocument() const;

// 操作
public:
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

// 重写
public:
	virtual void OnDraw(CDC* pDC);  // 重写以绘制该视图
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	void StartTimer();
	void StopTimer();
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	void DrawCoordinateSystem(CDC* pDC);
	void DrawCoordinateSystemFile(CDC* pDC);
	void DrawDataArray(CDC* pDC);
	void DrawDataArrayFile(CDC* pDC);
	void DrawData();
	unsigned long GetMinData(DATA_PART tmp, unsigned long m);
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
};

#ifndef _DEBUG  // DMonitorView.cpp 中的调试版本
inline CDataMonitorDoc* CDataMonitorView::GetDocument() const
   { return reinterpret_cast<CDataMonitorDoc*>(m_pDocument); }
#endif

