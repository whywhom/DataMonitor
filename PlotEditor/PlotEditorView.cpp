
// PlotEditorView.cpp : CPlotEditorView ���ʵ��
//

#include "stdafx.h"
// SHARED_HANDLERS ������ʵ��Ԥ��������ͼ������ɸѡ�������
// ATL ��Ŀ�н��ж��壬�����������Ŀ�����ĵ����롣
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
	// ��׼��ӡ����
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_WM_PAINT()
END_MESSAGE_MAP()

// CPlotEditorView ����/����

CPlotEditorView::CPlotEditorView()
{
	// TODO: �ڴ˴���ӹ������

}

CPlotEditorView::~CPlotEditorView()
{
}

BOOL CPlotEditorView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: �ڴ˴�ͨ���޸�
	//  CREATESTRUCT cs ���޸Ĵ��������ʽ

	return CView::PreCreateWindow(cs);
}

// CPlotEditorView ����

void CPlotEditorView::OnDraw(CDC* pDC)
{
	CPlotEditorDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: �ڴ˴�Ϊ����������ӻ��ƴ���
	DrawCoordinateSystem(pDC);
}


// CPlotEditorView ��ӡ

BOOL CPlotEditorView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// Ĭ��׼��
	return DoPreparePrinting(pInfo);
}

void CPlotEditorView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: ��Ӷ���Ĵ�ӡǰ���еĳ�ʼ������
}

void CPlotEditorView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: ��Ӵ�ӡ����е��������
}


// CPlotEditorView ���

#ifdef _DEBUG
void CPlotEditorView::AssertValid() const
{
	CView::AssertValid();
}

void CPlotEditorView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CPlotEditorDoc* CPlotEditorView::GetDocument() const // �ǵ��԰汾��������
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CPlotEditorDoc)));
	return (CPlotEditorDoc*)m_pDocument;
}
#endif //_DEBUG


// CPlotEditorView ��Ϣ�������


void CPlotEditorView::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: �ڴ˴������Ϣ����������
	// ��Ϊ��ͼ��Ϣ���� CView::OnPaint()
    OnPrepareDC(&dc);
	OnDraw(&dc);
}

void CPlotEditorView::DrawCoordinateSystem(CDC* pDC)
{
	TRACE(_T("DrawCoordinateSystem"));
	//����ߵ���ɫ��ɫ
	COLORREF color = RGB(127,127,127);
	
	//����ߵķ��
	int iDrawType = PS_SOLID;
	//iDrawType = _wtoi(_T("1"));
	
    //�����ߵĻ���"���"��"��ɫ"����"��ϸ"
	CPen pen(iDrawType, 1,color); //����
	CPen* pOldPen = pDC->SelectObject(&pen);//���ʺͻ���������
	
	CRect rect;
	//�������佲��ÿؼ�����Ļ�е�λ��
	//GetDlgItem(IDC_STATIC_PIC)->GetWindowRect(&rect);//��ȡ�ؼ��������Ļ��λ��,����ؼ���������
	//ScreenToClient(rect);//ת��Ϊ�Ի����ϵ����λ��
	GetClientRect(rect);
	rect.left = rect.left+10;
	rect.right = rect.right-10;
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
	//����ˢ��
	pDC->SelectObject(pOldPen);  //[���Բ���Ҫ]
}