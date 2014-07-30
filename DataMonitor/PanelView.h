#pragma once



// CPanelView 窗体视图

class CPanelView : public CFormView
{
	DECLARE_DYNCREATE(CPanelView)

protected:
	CPanelView();           // 动态创建所使用的受保护的构造函数
	virtual ~CPanelView();

public:
	enum { IDD = IDD_PANEL_VIEW };
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
};


