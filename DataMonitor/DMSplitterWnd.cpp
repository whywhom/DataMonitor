// DMSplitterWnd.cpp : ʵ���ļ�
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



// CDMSplitterWnd ��Ϣ�������

BOOL CDMSplitterWnd::OnCreateClient(LPCREATESTRUCT /*lpcs*/, CCreateContext* pContext)
{
    return m_wndSplitter.Create(this,
                                1, 2,       // TODO: adjust the number of rows, columns
                                CSize(10, 10),  // TODO: adjust the minimum pane size
                                pContext);
}


void CDMSplitterWnd::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	//���������������ֹ�зִ����û�������С��
	//CSplitterWnd::OnLButtonDown(nFlags, point);
	return;
}


void CDMSplitterWnd::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	//���������������ֹ���λ���зִ���֮��ʱ�ı���״��
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
