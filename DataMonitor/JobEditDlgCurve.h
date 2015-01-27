#pragma once
#include "afxwin.h"


// CJobEditDlgCurve �Ի���

class CJobEditDlgCurve : public CDialog
{
	DECLARE_DYNAMIC(CJobEditDlgCurve)

public:
	CJobEditDlgCurve(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CJobEditDlgCurve();

// �Ի�������
	enum { IDD = IDD_JOBEDIT_CURVE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	CComboBox m_unitbox;
	CComboBox m_filterbox;
	CString m_Label;
	int m_unit;
	int m_filter;
	afx_msg void OnEnChangeLabel();
};
