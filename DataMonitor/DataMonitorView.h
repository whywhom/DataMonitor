
// DMonitorView.h : CDataMonitorView ��Ľӿ�
//

#pragma once

#include "DataMonitorDoc.h"
#include "clPlot.h"
class CDataMonitorView : public CScrollView
{
protected: // �������л�����
	CDataMonitorView();
	DECLARE_DYNCREATE(CDataMonitorView)

// ����
public:
	CDataMonitorDoc* GetDocument() const;

// ����
public:

protected:
	//why add ��ͼ����
	int			unitPixel;//ÿ�׶�Ӧ����ֵ
	int			unitRatio;//ÿ�׶�Ӧ����ֵ��ϵ��
	int			m_iterator;//��ͼ������
	CRect		m_screenRect;// m_screenRect - view�Ŀɼ���Χ�����Ļ���Ͻ�����
	CRect		m_clientRect;// m_clientRect - view�Ŀɼ���Χ�ͻ���
	CRect		m_totalRect;// m_totalRect - ����Ҫ��������ķ�Χ���������ɼ�

	CRect		m_plot1Rect;// m_plot1Rect - borderspace1
	CRect		m_plot2Rect;// m_plot2Rect - borderspace2
	CRect		m_plot3Rect;// m_plot3Rect - borderspace3

	SIZE sizeTotal;//����Ҫ��������ķ�Χ���������ɼ�

	double		minDepth;//�ɼ���Χ��С���
	double		maxDepth;//�ɼ���Χ������
	double		maxPreDepth;//�ɼ���Χ�ƶ�֮ǰ��������
	double		minDepthLimit;//��ҵ����С���
	double		maxDepthLimit;//��ҵ��������
	bool		m_bAutoScrollY;		// automatic y range scrolling
	bool		m_bDirectDown;		// ���»���
	bool		bScroll;//���Ƶ��ײ���ʼ����ʱ����Ϊtrue
	int			m_drawCount;//ÿ�λ��ƶ��ٸ�����
	int			m_step;//ÿ���ƶ����룬����Ϊ��λ
	bool		bTimer;
	//��ɫ
	COLORREF	m_gridColor;		// grid line color
	//��������
	CFont		m_font;
	LOGFONT		m_zoomFont;
	//˫����
	CDC   MemDC;          //����һ����ʾ�豸����  
	CBitmap   MemBitmap;  //����һ��λͼ����  
	//��������ɲ�����������Сֵ
	long tempLimitArray[2];
	long gmLimitArray[2];
	long rmLimitArray[2];
	long cclLimitArray[2];
	long magLimitArray[3][2];

	DATA_TEMP olddeptArray;
	DATA_TEMP oldtempArray;
	DATA_TEMP oldgmArray;
	DATA_TEMP oldrmArray;
	DATA_TEMP oldcclArray;
	DATA_TEMP oldmagArray[3];
#ifdef FEATURE_TEST_DRAW
	clPlot m_Plot;
#endif

protected:
	//why add ��ͼ����
	void DrawBasic(CDC * pDC);
	void DrawGrid(CDC * pDC);//���ƺ�����������
	void DrawPlot(CDC* pDC);//������������
	//void DrawCurve(CDC* pDC);//���Ʋ������
	void DrawCurve(CDC* pDC , double upDepth, double DownDepth);//���Ʋ������
	void DrawDeptData(CDC* pDC ,CPetroData* pPData,CPen* pPpen);//���Ƹ߶�����
	void DrawTempData(CDC* pDC ,CPetroData* pPData,CPen* pPpen);//�����¶�����
	void DrawRmData(CDC* pDC ,CPetroData* pPData,CPen* pPpen);//����Rm����
	void DrawMagxData(CDC* pDC ,CPetroData* pPData,CPen* pPpen);//���ƴ�����������
	void DrawGmData(CDC* pDC ,CPetroData* pPData,CPen* pPpen);//����Gm����
	void DrawCclData(CDC* pDC ,CPetroData* pPData,CPen* pPpen);//����Ccl����
	void DrawData(CDC* pDC);
	void DrawDataFile(CDC* pDC);
	void SetScrollTotalSizes(CRect rect);
	void InitArrayData();
	void InitOldArrayData();
	void GetMaxMinData();
	void AddPanelListView( );
	void UpdatePanelListView(CPetroData* pPData);
	void CalculateParam();
#ifdef FEATURE_TEST_DRAW
	void InitPlot(CRect Rect);
#endif
	unsigned long GetMinData(DATA_PART tmp, unsigned long m);
	
// ��д
public:
	virtual void OnDraw(CDC* pDC);  // ��д�Ի��Ƹ���ͼ
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	void StartTimer();
	void StopTimer();
	void KillDrawTimer();
	void SetDirectionDown(bool bDown);//���ò�淽��
	bool GetDirectionDown( );
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

private:
	unsigned long base;//������
	unsigned long bias;//���ƫ����
	long counter;//������

	POSITION pos;//��ǰ��¼λ��
	CPetroData* pPData;//�õ���������
	CPetroData* pOldPData;//����ǰһ�����ݶ���

	CRect rect;
	CRect rectTotal;
	
	
// ʵ��
public:
	virtual ~CDataMonitorView();
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
	virtual void OnInitialUpdate();
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnClose();
	afx_msg void OnDestroy();
};

#ifndef _DEBUG  // DMonitorView.cpp �еĵ��԰汾
inline CDataMonitorDoc* CDataMonitorView::GetDocument() const
   { return reinterpret_cast<CDataMonitorDoc*>(m_pDocument); }
#endif

