#pragma once



// CDrawFormView ������ͼ

class CDrawFormView : public CFormView
{
	DECLARE_DYNCREATE(CDrawFormView)

protected:
	CDrawFormView();           // ��̬������ʹ�õ��ܱ����Ĺ��캯��
	virtual ~CDrawFormView();

public:
	enum { IDD = IDD_DRAWFORMVIEW };
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
};


