#pragma once
#include "afxwin.h"
#include "afxcmn.h"
#include "CanvasView.h"

// CChildDraw 对话框

class CChildDraw : public CDialogEx
{
	DECLARE_DYNAMIC(CChildDraw)

public:
	CChildDraw(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CChildDraw();
private:
	CCanvasView *pCanvasView;
// 对话框数据
	enum { IDD = IDD_CHILD_DRAW };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	CStatic cs01;
	CString value01;
	CStatic cs02;
	CString value02;
	CStatic cs03;
	CString value03;
	CStatic cs04;
	CString value04;
	CEdit Edit01;
	CString strEdit01;
	CEdit Edit02;
	CString strEdit02;
	CEdit Edit03;
	CString strEdit03;
	CEdit Edit04;
	CString strEdit04;
	CButton btSetup;
	CListCtrl listView;
};
