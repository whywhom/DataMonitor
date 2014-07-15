#pragma once


// CJobEditDlgTool 对话框

class CJobEditDlgTool : public CDialog
{
	DECLARE_DYNAMIC(CJobEditDlgTool)

public:
	CJobEditDlgTool(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CJobEditDlgTool();

// 对话框数据
	enum { IDD = IDD_JOBEDIT_TOOL };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
};
