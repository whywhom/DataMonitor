#pragma once
#include "afxcmn.h"
#include "afxwin.h"
#include "tinyxml.h"
#include "ColorStatic.h"
// CTestDlg 对话框

class CTestDlg : public CDialog
{
	DECLARE_DYNAMIC(CTestDlg)

public:
	CTestDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CTestDlg();
	void AddWorkInfo(CWorkInfo* pWorkInfo, int item);
	bool SetCurveInfo(int item);
	void InitCtrl();
	void InitComboBox(int id,CString signal,CString title, CString uint);
	void InitWorkInfoList(CString signal,CString title, CString uint);
// 对话框数据
	enum { IDD = IDD_TEST_DLG };
public:
	CWorkInfo* pCurList;//记录当前的项
	CColorStatic mStaticColor;
	COLORREF curveSelectColor; //颜色
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedButtonColor();
	CListCtrl mTestList;
	CComboBox mCbSignal;
	CComboBox nAbbr;
	CComboBox mTestUnit;
	CEdit nTestEditMin;
	CEdit nTestEditMax;
	CButton mTestAdd;
	CButton mTestDel;
	afx_msg void OnBnClickedButtonAdd();
	afx_msg void OnBnClickedButtonDel();
};
