
// MonitorDlg.h : 头文件
//

#pragma once
#include "jobdlg.h"
#include "jobeditdlg.h"
#include "ChildDraw.h"

extern bool bRunning;
extern CTypedPtrList < CPtrList, SEND_COMMAND_LIST * >senCommandList;

// CMonitorDlg 对话框
class CMonitorDlg : public CDialogEx
{
public:
	CToolBar m_ToolBar;
	CChildDraw* childDrawWnd;
private:
	
	CWinThread*   m_Thread;
	CTypedPtrList < CPtrList, CDialog * >wndList;//子窗体管理列表
// 构造
public:
	CMonitorDlg(CWnd* pParent = NULL);	// 标准构造函数
	~CMonitorDlg();
	DWORD CreatConnect( );//建立连接
	DWORD DestroyConnect( );//断开连接
	void DataReceive(BYTE* inbuff, DWORD* dwSize);
	WORD DataSend(BYTE* outbuff, DWORD dwSize);
	bool ChildWndInitialize();
	bool ShowActiveDlg(CWnd* activeWnd,int id);
// 对话框数据
	enum { IDD = IDD_DATAMONITOR_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持
	static UINT MainThread(LPVOID pParam);
	bool StartThread();
	void initCmdList();
	void initWndList();
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
	afx_msg LRESULT OnCommReceive(WPARAM wParam, LPARAM lParam);//接收端口消息
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnClose();
	afx_msg void OnMenuNew();
	afx_msg void OnMenuOpen();
	afx_msg void OnMenuWork();
	afx_msg void OnMenuInstrument();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnGetMinMaxInfo(MINMAXINFO* lpMMI);
};
