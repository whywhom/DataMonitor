#pragma once
#include "afxwin.h"
#include "afxcmn.h"
#include "ColorButton.h"
#include "ColorStatic.h"
// CCurvePage 对话框

class CCurvePage : public CPropertyPage
{
	DECLARE_DYNAMIC(CCurvePage)

public:
	CCurvePage();
	virtual ~CCurvePage();
	void AddCurveInfo(CCurveInfo* pCurveInfo, int item);
	bool SetCurveInfo(int item);
	void InseertCurveInfo( );
	void DeleteCurveInfo( );
	void GetCurveData(CCurveInfo* pCurveInfo, CString strTemp);
	void SetCurveData(int index);
	int GetCurLineIndex(int lineWidth);
	COLORREF curveSelectColor; //颜色

	CCurveInfo* pCurList;//记录当前的项
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
	//CColorButton btColor;
	CButton btColor;
	CComboBox combKeDu;
	CComboBoxEx combXianXing;
	CComboBox combXianKuan;
	CComboBox combHuiRaoMoShi;
	CEdit combHuiRaoCiShu;
	CSpinButtonCtrl spinEdit;
	CColorStatic mStaticColor;
	afx_msg void OnBnClickedButtonColor();
	afx_msg void OnCbnSelchangeComboBiaoshi();
	afx_msg void OnCbnSelchangeComboShuju();
	afx_msg void OnNMThemeChangedComboXianxing(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMThemeChangedComboXiankuan(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMThemeChangedComboHuiraomoshi(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMThemeChangedComboKedu(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMThemeChangedComboGuidao(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnCbnSelchangeComboGuidao();
	afx_msg void OnCbnSelchangeComboKedu();
	afx_msg void OnCbnSelchangeComboXianxing();
	afx_msg void OnCbnSelchangeComboXiankuan();
	afx_msg void OnCbnSelchangeComboHuiraomoshi();
	afx_msg void OnEnChangeEditHuiraocishu();
	afx_msg void OnEnChangeEditYoubianjie();
	afx_msg void OnEnChangeEditZuobianjie();
};
