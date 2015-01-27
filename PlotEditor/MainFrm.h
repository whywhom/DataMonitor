
// MainFrm.h : CMainFrame 类的接口
//

#pragma once
#include "PlotEditorView.h"
#include "CoordinateView.h"
#include "CurveSheet.h"
#include "CurveInfo.h"
#include "..\DataMonitor\tinyxml.h"

class CMainFrame : public CFrameWnd
{
	
protected: // 仅从序列化创建
	CMainFrame();
	DECLARE_DYNCREATE(CMainFrame)
	void PaintCurve();
// 特性
protected:
	CSplitterWnd m_wndSplitter;
public:
	CMainFrame*   pFrame; 
	CPlotEditorView*   pPlotEditorView; 
	CCoordinateView * pCoodinateView; 
// 操作
public:

// 重写
public:
	virtual BOOL OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext);
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

// 实现
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // 控件条嵌入成员
	CStatusBar        m_wndStatusBar;
	void ParseCurve();
	void ClearList();
	CCoordinateView* GetCoordinateView();
	CPlotEditorView* GetPlotEditorView();
// 生成的消息映射函数
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnFileNewplot();
	afx_msg void OnFileSaveplot();
	afx_msg void OnClose();
};


