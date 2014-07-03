
// MonitorDlg.h : 头文件
//

#pragma once


// CMonitorDlg 对话框
class CMonitorDlg : public CDialogEx
{
public:
	CToolBar m_ToolBar;
// 构造
public:
	CMonitorDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_DATAMONITOR_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnMenuAbout();
	afx_msg void OnUpdateMenuAbout(CCmdUI *pCmdUI);
	afx_msg void OnMenuExit();
};
