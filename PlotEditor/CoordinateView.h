#pragma once


// CCoordinateView ��ͼ

class CCoordinateView : public CScrollView
{
	DECLARE_DYNCREATE(CCoordinateView)
private:
	CRect rect;
	CRect rectTotal;
protected:
	CCoordinateView();           // ��̬������ʹ�õ��ܱ����Ĺ��캯��
	virtual ~CCoordinateView();
	void DrawCoordinateSystem(CDC* pDC);
	void SetScrollTotalSizes(CRect rect);
public:
	virtual void OnDraw(CDC* pDC);      // ��д�Ի��Ƹ���ͼ
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
	virtual void OnInitialUpdate();
};


