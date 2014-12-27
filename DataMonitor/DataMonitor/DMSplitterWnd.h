#pragma once


// CDMSplitterWnd

class CDMSplitterWnd : public CSplitterWnd
{
	DECLARE_DYNAMIC(CDMSplitterWnd)

public:
	CDMSplitterWnd();
	virtual ~CDMSplitterWnd();
protected:
	CSplitterWnd    m_wndSplitter;
	virtual BOOL OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext);
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
};


