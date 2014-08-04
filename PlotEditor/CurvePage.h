#pragma once
#include "afxwin.h"
#include "afxcmn.h"


// CCurvePage 对话框

class CCurvePage : public CPropertyPage
{
	DECLARE_DYNAMIC(CCurvePage)

public:
	CCurvePage();
	virtual ~CCurvePage();

// 对话框数据
	enum { IDD = IDD_CURVEPAGE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedButtonAdd();
	afx_msg void OnBnClickedButtonDel();
	CComboBox combBiaoShi;
	CComboBox combShuJu;
	CComboBox combGuiDao;
	CEdit editZuoBianJie;
	CEdit editYouBianJie;
	CButton btColor;
	CComboBox combKeDu;
	CComboBox combXianXing;
	CComboBox combXianKuan;
	CComboBox combHuiRaoMoShi;
	CEdit combHuiRaoCiShu;
	CSpinButtonCtrl spinEdit;
};
