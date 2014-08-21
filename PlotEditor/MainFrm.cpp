
// MainFrm.cpp : CMainFrame 类的实现
//

#include "stdafx.h"
#include "PlotEditor.h"

#include "MainFrm.h"
#include <string>
#include <iostream> 
using namespace std; 

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	ON_WM_CREATE()
	ON_COMMAND(ID_FILE_NEWPLOT, &CMainFrame::OnFileNewplot)
	ON_COMMAND(ID_FILE_SAVEPLOT, &CMainFrame::OnFileSaveplot)
	ON_WM_CLOSE()
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // 状态行指示器
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};

// CMainFrame 构造/析构

CMainFrame::CMainFrame()
{
	// TODO: 在此添加成员初始化代码
	ClearList();
}

CMainFrame::~CMainFrame()
{
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	if (!m_wndStatusBar.Create(this))
	{
		TRACE0("未能创建状态栏\n");
		return -1;      // 未能创建
	}
	m_wndStatusBar.SetIndicators(indicators, sizeof(indicators)/sizeof(UINT));
	if(theApp.bCurveFileExist == true)
	{
		ParseCurve();
	}
	return 0;
}

BOOL CMainFrame::OnCreateClient(LPCREATESTRUCT /*lpcs*/,
	CCreateContext* pContext)
{
	if(!m_wndSplitter.CreateStatic(this,2,1))
	{
		return FALSE;
	}
	CRect rect;
	GetClientRect(&rect);
	

	pFrame=(CMainFrame*)AfxGetMainWnd;

	if(!m_wndSplitter.CreateView(0,0,RUNTIME_CLASS(CPlotEditorView),CSize(rect.Width(),rect.Height()-200),pContext))
	{
		return FALSE;
	}
	pPlotEditorView = (CPlotEditorView*)m_wndSplitter.GetPane(0,0); 
	if(!m_wndSplitter.CreateView(1,0,RUNTIME_CLASS(CCoordinateView),CSize(rect.Width(),200),pContext))
	{
		return FALSE;
	}
	pCoodinateView = (CCoordinateView*)m_wndSplitter.GetPane(1,0); 
	return TRUE;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式
	cs.style = WS_OVERLAPPED | WS_CAPTION /*| FWS_ADDTOTITLE*/
		 | WS_THICKFRAME | WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_MAXIMIZE | WS_SYSMENU;
	return TRUE;
}

// CMainFrame 诊断

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}
#endif //_DEBUG

void CMainFrame::ClearList()
{
	CCurveInfo* pCurrent = NULL;
	while(theApp.curveList.IsEmpty()==false)
	{
		pCurrent=theApp.curveList.RemoveHead();
		delete pCurrent;
		pCurrent=NULL;
	}
}
// CMainFrame 消息处理程序
void CMainFrame::ParseCurve()
{
	WCHAR pwBuffer[100];
	CString str;
	ClearList();
	string fileXml = theApp.IniFilePath + "Curve.xml";
	TiXmlDocument doc(fileXml.c_str());
	bool loadOkay = doc.LoadFile();
	if (loadOkay)
	{
		if ( !doc.Parse( fileXml.c_str() ) )
		{
			cout << doc.ErrorDesc() << endl;
		}
		//step 1 : Get the RootElement
		TiXmlElement *root = doc.RootElement();
		for ( TiXmlNode *child = root->FirstChild(); child; child = child->NextSibling() )
		{
			if ( ( child->Type() == TiXmlNode::TINYXML_ELEMENT ) && ( !strcmp( child->Value(), "CurveInfo" ) ) )
			{
				CCurveInfo* plist = new CCurveInfo();
				for ( TiXmlNode *subchild = child->FirstChild(); subchild; subchild = subchild->NextSibling() )
				{
					if ( ( subchild->Type() == TiXmlNode::TINYXML_ELEMENT ) && ( !strcmp( subchild->Value(), "Name" ) ) )
					{
						TiXmlNode* subsubchild = subchild->FirstChild();
						memset(pwBuffer,0,sizeof(pwBuffer));
						const char* strValue = subsubchild->Value();
						int len = strlen(strValue);
						theApp.FromUTF8ToUnicode((BYTE*)strValue,  len, (WORD*)pwBuffer, sizeof(pwBuffer));
						str = pwBuffer;
						plist->strCurveName = str;
					}
					else if ( ( subchild->Type() == TiXmlNode::TINYXML_ELEMENT ) && ( !strcmp( subchild->Value(), "Track" ) ) )
					{
						TiXmlNode* subsubchild = subchild->FirstChild();
						memset(pwBuffer,0,sizeof(pwBuffer));
						const char* strValue = subsubchild->Value();
						int len = strlen(strValue);
						theApp.FromUTF8ToUnicode((BYTE*)strValue,  len, (WORD*)pwBuffer, sizeof(pwBuffer));
						str = pwBuffer;
						plist->curveTrack = _tstoi(pwBuffer);;
					}
					else if ( ( subchild->Type() == TiXmlNode::TINYXML_ELEMENT ) && ( !strcmp( subchild->Value(), "LeftLimit" ) ) )
					{
						TiXmlNode* subsubchild = subchild->FirstChild();
						memset(pwBuffer,0,sizeof(pwBuffer));
						const char* strValue = subsubchild->Value();
						int len = strlen(strValue);
						theApp.FromUTF8ToUnicode((BYTE*)strValue,  len, (WORD*)pwBuffer, sizeof(pwBuffer));
						str = pwBuffer;
						plist->leftLimit = _tstoi(pwBuffer);;
					}
					else if ( ( subchild->Type() == TiXmlNode::TINYXML_ELEMENT ) && ( !strcmp( subchild->Value(), "RightLimit" ) ) )
					{
						TiXmlNode* subsubchild = subchild->FirstChild();
						memset(pwBuffer,0,sizeof(pwBuffer));
						const char* strValue = subsubchild->Value();
						int len = strlen(strValue);
						theApp.FromUTF8ToUnicode((BYTE*)strValue,  len, (WORD*)pwBuffer, sizeof(pwBuffer));
						str = pwBuffer;
						plist->rightLimit = _tstoi(pwBuffer);;
					}
					else if ( ( subchild->Type() == TiXmlNode::TINYXML_ELEMENT ) && ( !strcmp( subchild->Value(), "Color" ) ) )
					{
						TiXmlNode* subsubchild = subchild->FirstChild();
						memset(pwBuffer,0,sizeof(pwBuffer));
						const char* strValue = subsubchild->Value();
						int len = strlen(strValue);
						theApp.FromUTF8ToUnicode((BYTE*)strValue,  len, (WORD*)pwBuffer, sizeof(pwBuffer));
						str = pwBuffer;
						plist->curveColor = _tstoi(pwBuffer);;
					}
					else if ( ( subchild->Type() == TiXmlNode::TINYXML_ELEMENT ) && ( !strcmp( subchild->Value(), "Graduation" ) ) )
					{
						TiXmlNode* subsubchild = subchild->FirstChild();
						memset(pwBuffer,0,sizeof(pwBuffer));
						const char* strValue = subsubchild->Value();
						int len = strlen(strValue);
						theApp.FromUTF8ToUnicode((BYTE*)strValue,  len, (WORD*)pwBuffer, sizeof(pwBuffer));
						str = pwBuffer;
						plist->graduation = _tstoi(pwBuffer);;
					}
					else if ( ( subchild->Type() == TiXmlNode::TINYXML_ELEMENT ) && ( !strcmp( subchild->Value(), "LineType" ) ) )
					{
						TiXmlNode* subsubchild = subchild->FirstChild();
						memset(pwBuffer,0,sizeof(pwBuffer));
						const char* strValue = subsubchild->Value();
						int len = strlen(strValue);
						theApp.FromUTF8ToUnicode((BYTE*)strValue,  len, (WORD*)pwBuffer, sizeof(pwBuffer));
						str = pwBuffer;
						plist->lineType = _tstoi(pwBuffer);;
					}
					else if ( ( subchild->Type() == TiXmlNode::TINYXML_ELEMENT ) && ( !strcmp( subchild->Value(), "LineWidth" ) ) )
					{
						TiXmlNode* subsubchild = subchild->FirstChild();
						memset(pwBuffer,0,sizeof(pwBuffer));
						const char* strValue = subsubchild->Value();
						int len = strlen(strValue);
						theApp.FromUTF8ToUnicode((BYTE*)strValue,  len, (WORD*)pwBuffer, sizeof(pwBuffer));
						str = pwBuffer;
						plist->lineWidth = _tstoi(pwBuffer);;
					}
					else if ( ( subchild->Type() == TiXmlNode::TINYXML_ELEMENT ) && ( !strcmp( subchild->Value(), "WrapMode" ) ) )
					{
						TiXmlNode* subsubchild = subchild->FirstChild();
						memset(pwBuffer,0,sizeof(pwBuffer));
						const char* strValue = subsubchild->Value();
						int len = strlen(strValue);
						theApp.FromUTF8ToUnicode((BYTE*)strValue,  len, (WORD*)pwBuffer, sizeof(pwBuffer));
						str = pwBuffer;
						plist->wrapMode = _tstoi(pwBuffer);;
					}
					else if ( ( subchild->Type() == TiXmlNode::TINYXML_ELEMENT ) && ( !strcmp( subchild->Value(), "WrapCount" ) ) )
					{
						TiXmlNode* subsubchild = subchild->FirstChild();
						memset(pwBuffer,0,sizeof(pwBuffer));
						const char* strValue = subsubchild->Value();
						int len = strlen(strValue);
						theApp.FromUTF8ToUnicode((BYTE*)strValue,  len, (WORD*)pwBuffer, sizeof(pwBuffer));
						str = pwBuffer;
						plist->wrapCount = _tstoi(pwBuffer);;
					}
				}
				theApp.curveList.AddTail(plist);
			}
		}
	}
}

void CMainFrame::OnFileNewplot()
{
	// TODO: 在此添加命令处理程序代码
	CCurveSheet PropSht(theApp.GetResString(IDS_CURVE_PROPERTY));
	int result = PropSht.DoModal();
	if(result == IDOK)
	{
		ParseCurve();
		if(!theApp.curveList.IsEmpty())
		{
			PaintCurve();
		}
	}
}


void CMainFrame::OnFileSaveplot()
{
	// TODO: 在此添加命令处理程序代码
}


void CMainFrame::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	CFrameWnd::OnClose();
}

void CMainFrame::PaintCurve()
{

}