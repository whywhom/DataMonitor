#pragma once


// CCoordinateView 视图

class CCoordinateView : public CView
{
	DECLARE_DYNCREATE(CCoordinateView)

protected:
	CCoordinateView();           // 动态创建所使用的受保护的构造函数
	virtual ~CCoordinateView();
	void DrawCoordinateSystem(CDC* pDC);
public:
	virtual void OnDraw(CDC* pDC);      // 重写以绘制该视图
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnPaint();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnFileSaveplot();
	afx_msg void OnFileNewplot();
	afx_msg void OnClose();
};


