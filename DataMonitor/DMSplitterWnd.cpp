// DMSplitterWnd.cpp : 实现文件
//

#include "stdafx.h"
#include "DataMonitor.h"
#include "DMSplitterWnd.h"


// CDMSplitterWnd

IMPLEMENT_DYNAMIC(CDMSplitterWnd, CWnd)

CDMSplitterWnd::CDMSplitterWnd()
{

}

CDMSplitterWnd::~CDMSplitterWnd()
{
}


BEGIN_MESSAGE_MAP(CDMSplitterWnd, CWnd)
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
END_MESSAGE_MAP()



// CDMSplitterWnd 消息处理程序

BOOL CDMSplitterWnd::OnCreateClient(LPCREATESTRUCT /*lpcs*/, CCreateContext* pContext)
{
    return m_wndSplitter.Create(this,
                                1, 2,       // TODO: adjust the number of rows, columns
                                CSize(10, 10),  // TODO: adjust the minimum pane size
                                pContext);
}


void CDMSplitterWnd::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	//重载这个函数，禁止切分窗口用户调整大小。
	//CSplitterWnd::OnLButtonDown(nFlags, point);
	return;
}


void CDMSplitterWnd::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	//重载这个函数，禁止鼠标位于切分窗口之间时改变形状。
	//CSplitterWnd::OnMouseMove(nFlags, point);
	if(point.x<248||point.x>252) 
	{ 
		CWnd::OnMouseMove(nFlags, point); 
	} 
	else 
	{ 
		CSplitterWnd::OnMouseMove(nFlags, point); 
	} 
}
