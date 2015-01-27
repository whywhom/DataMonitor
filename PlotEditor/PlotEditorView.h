
// PlotEditorView.h : CPlotEditorView ��Ľӿ�
//

#pragma once

#include "PlotEditorDoc.h"
class CPlotEditorView : public CView
{
protected: // �������л�����
	CPlotEditorView();
	DECLARE_DYNCREATE(CPlotEditorView)

// ����
public:
	CPlotEditorDoc* GetDocument() const;
	void DrawCoordinateSystem(CDC* pDC);
// ����
public:

// ��д
public:
	virtual void OnDraw(CDC* pDC);  // ��д�Ի��Ƹ���ͼ
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// ʵ��
public:
	virtual ~CPlotEditorView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ���ɵ���Ϣӳ�亯��
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPaint();
};

#ifndef _DEBUG  // PlotEditorView.cpp �еĵ��԰汾
inline CPlotEditorDoc* CPlotEditorView::GetDocument() const
   { return reinterpret_cast<CPlotEditorDoc*>(m_pDocument); }
#endif

