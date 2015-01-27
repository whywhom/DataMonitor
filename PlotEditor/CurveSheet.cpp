// CurveSheet.cpp : 实现文件
//

#include "stdafx.h"
#include "PlotEditor.h"
#include "CurveSheet.h"


// CCurveSheet

IMPLEMENT_DYNAMIC(CCurveSheet, CPropertySheet)

CCurveSheet::CCurveSheet(UINT nIDCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(nIDCaption, pParentWnd, iSelectPage)
{
	AddControlPages();
}

CCurveSheet::CCurveSheet(LPCTSTR pszCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(pszCaption, pParentWnd, iSelectPage)
{
	AddControlPages();
}

CCurveSheet::~CCurveSheet()
{
}


BEGIN_MESSAGE_MAP(CCurveSheet, CPropertySheet)
	ON_COMMAND(IDOK, &CCurveSheet::OnIdok)
	ON_COMMAND(IDNO, &CCurveSheet::OnIdno)
	ON_COMMAND(IDCANCEL, &CCurveSheet::OnIdcancel)
END_MESSAGE_MAP()


// CCurveSheet 消息处理程序


BOOL CCurveSheet::OnInitDialog()
{
	BOOL bResult = CPropertySheet::OnInitDialog();

	// TODO:  在此添加您的专用代码
	
	CWnd *pWnd = GetDlgItem (ID_APPLY_NOW);
	pWnd->ShowWindow (FALSE);
	pWnd = GetDlgItem (IDHELP);
	pWnd->ShowWindow (FALSE);
	int ids[] = { IDOK, IDCANCEL };
	CRect rectWnd;
	CRect rectBtn;
	GetWindowRect (rectWnd);
	GetDlgItem (IDOK)->GetWindowRect (rectBtn);
	int btnWidth = rectBtn.Width();
	int btnHeight = rectBtn.Height();
	int btnOffset = rectWnd.bottom - rectBtn.bottom;
	int btnLeft = rectWnd.right - rectWnd.left;
	rectWnd.bottom = rectBtn.top;
	rectWnd.right = rectWnd.right + btnWidth + btnOffset;
	MoveWindow(rectWnd);
	rectBtn.left = btnLeft;
	rectBtn.right = btnLeft + btnWidth;
	for (int i = 0; i < sizeof (ids) / sizeof (int); i++)
	{
		rectBtn.top = (i + 1) * btnOffset + btnHeight * i;
		rectBtn.bottom = rectBtn.top + btnHeight;
		GetDlgItem (ids [i])->MoveWindow (rectBtn);
	}
	return bResult;
}

void CCurveSheet::AddControlPages()
{
    m_psh.dwFlags &= (~PSH_HASHELP);
    AddPage(&m_PageCurve);
}



void CCurveSheet::OnIdok()
{
	// TODO: 在此添加命令处理程序代码
	CreateCurveFile(theApp.strCurveFile);
	CPropertySheet::EndDialog(IDOK);
}


void CCurveSheet::OnIdno()
{
	// TODO: 在此添加命令处理程序代码
	CPropertySheet::EndDialog(IDNO);
}


void CCurveSheet::OnIdcancel()
{
	// TODO: 在此添加命令处理程序代码
	CPropertySheet::EndDialog(IDCANCEL);
}
int CCurveSheet::CreateCurveFile(CString strFile)
{

	TiXmlDocument doc;
	TiXmlDeclaration * decl = new TiXmlDeclaration( "1.0", "UTF-8", "yes" );
	doc.LinkEndChild( decl ); 
	TiXmlElement * element = new TiXmlElement( "Curves" );
	POSITION pos = theApp.curveList.GetHeadPosition();
	while(pos)
	{
		CCurveInfo* plist = theApp.curveList.GetNext(pos);
		LinkElementeFuns(element,plist);	
	}
	doc.LinkEndChild( element );
	////
	char * pFileName = theApp.FromUNICODEToANSI(strFile);
	doc.SaveFile(pFileName);

	delete []pFileName;
	pFileName = NULL;
	return 0;
}

void CCurveSheet::LinkElementeFuns(TiXmlElement * element,CCurveInfo* plist)
{
	TiXmlElement* msgs = new TiXmlElement( "CurveInfo" ); 
	element->LinkEndChild( msgs );
	TiXmlElement* msg = new TiXmlElement( "Name" ); 
	CString str = plist->strCurveName;
	////********/////transfer//////********//////
	const int maxBufferSize = 64;
	unsigned short UnicodeStr[maxBufferSize];
	unsigned short utf8StrLen;
	DWORD dwLength;//转换后的UTF－8编码的长度in BYTEs
	char utf8Str[64];
	memset(utf8Str,0,maxBufferSize*sizeof(char));
	memset(UnicodeStr,0,maxBufferSize*sizeof(unsigned short));
	short UnicodeStrLen=2*str.GetLength();
	memcpy(UnicodeStr,str,UnicodeStrLen);
	utf8StrLen=6*UnicodeStrLen;
	memset(utf8Str,0,maxBufferSize*sizeof(char));
	dwLength=theApp.FromUnicodeToUTF8( utf8Str, utf8StrLen, UnicodeStr, UnicodeStrLen);//Unicode to UTF-8
	//////////********///////////
	msg->LinkEndChild( new TiXmlText( utf8Str )); 
	msgs->LinkEndChild( msg ); 

	msg = new TiXmlElement( "Track" ); 
	str.Format(_T("%d"),plist->curveTrack);
	////********/////transfer//////********//////
	memset(utf8Str,0,maxBufferSize*sizeof(char));
	memset(UnicodeStr,0,maxBufferSize*sizeof(unsigned short));
	UnicodeStrLen=2*str.GetLength();
	memcpy(UnicodeStr,str,UnicodeStrLen);
	utf8StrLen=6*UnicodeStrLen;
	memset(utf8Str,0,maxBufferSize*sizeof(char));
	dwLength=theApp.FromUnicodeToUTF8( utf8Str, utf8StrLen, UnicodeStr, UnicodeStrLen);//Unicode to UTF-8
	//////////********///////////
	msg->LinkEndChild( new TiXmlText( utf8Str )); 
	msgs->LinkEndChild( msg ); 

	msg = new TiXmlElement( "LeftLimit" ); 
	str.Format(_T("%d"),plist->leftLimit);
	////********/////transfer//////********//////
	memset(utf8Str,0,maxBufferSize*sizeof(char));
	memset(UnicodeStr,0,maxBufferSize*sizeof(unsigned short));
	UnicodeStrLen=2*str.GetLength();
	memcpy(UnicodeStr,str,UnicodeStrLen);
	utf8StrLen=6*UnicodeStrLen;
	memset(utf8Str,0,maxBufferSize*sizeof(char));
	dwLength=theApp.FromUnicodeToUTF8( utf8Str, utf8StrLen, UnicodeStr, UnicodeStrLen);//Unicode to UTF-8
	//////////********///////////
	msg->LinkEndChild( new TiXmlText( utf8Str )); 
	msgs->LinkEndChild( msg ); 

	msg = new TiXmlElement( "RightLimit" ); 
	str.Format(_T("%d"),plist->rightLimit);
	////********/////transfer//////********//////
	memset(utf8Str,0,maxBufferSize*sizeof(char));
	memset(UnicodeStr,0,maxBufferSize*sizeof(unsigned short));
	UnicodeStrLen=2*str.GetLength();
	memcpy(UnicodeStr,str,UnicodeStrLen);
	utf8StrLen=6*UnicodeStrLen;
	memset(utf8Str,0,maxBufferSize*sizeof(char));
	dwLength=theApp.FromUnicodeToUTF8( utf8Str, utf8StrLen, UnicodeStr, UnicodeStrLen);//Unicode to UTF-8
	//////////********///////////
	msg->LinkEndChild( new TiXmlText( utf8Str )); 
	msgs->LinkEndChild( msg ); 

	msg = new TiXmlElement( "Color" ); 
	str.Format(_T("%d"),plist->curveColor);
	////********/////transfer//////********//////
	memset(utf8Str,0,maxBufferSize*sizeof(char));
	memset(UnicodeStr,0,maxBufferSize*sizeof(unsigned short));
	UnicodeStrLen=2*str.GetLength();
	memcpy(UnicodeStr,str,UnicodeStrLen);
	utf8StrLen=6*UnicodeStrLen;
	memset(utf8Str,0,maxBufferSize*sizeof(char));
	dwLength=theApp.FromUnicodeToUTF8( utf8Str, utf8StrLen, UnicodeStr, UnicodeStrLen);//Unicode to UTF-8
	//////////********///////////
	msg->LinkEndChild( new TiXmlText( utf8Str )); 
	msgs->LinkEndChild( msg ); 

	msg = new TiXmlElement( "Graduation" ); 
	str.Format(_T("%d"),plist->graduation);
	////********/////transfer//////********//////
	memset(utf8Str,0,maxBufferSize*sizeof(char));
	memset(UnicodeStr,0,maxBufferSize*sizeof(unsigned short));
	UnicodeStrLen=2*str.GetLength();
	memcpy(UnicodeStr,str,UnicodeStrLen);
	utf8StrLen=6*UnicodeStrLen;
	memset(utf8Str,0,maxBufferSize*sizeof(char));
	dwLength=theApp.FromUnicodeToUTF8( utf8Str, utf8StrLen, UnicodeStr, UnicodeStrLen);//Unicode to UTF-8
	//////////********///////////
	msg->LinkEndChild( new TiXmlText( utf8Str )); 
	msgs->LinkEndChild( msg ); 

	msg = new TiXmlElement( "LineType" ); 
	str.Format(_T("%d"),plist->lineType);
	////********/////transfer//////********//////
	memset(utf8Str,0,maxBufferSize*sizeof(char));
	memset(UnicodeStr,0,maxBufferSize*sizeof(unsigned short));
	UnicodeStrLen=2*str.GetLength();
	memcpy(UnicodeStr,str,UnicodeStrLen);
	utf8StrLen=6*UnicodeStrLen;
	memset(utf8Str,0,maxBufferSize*sizeof(char));
	dwLength=theApp.FromUnicodeToUTF8( utf8Str, utf8StrLen, UnicodeStr, UnicodeStrLen);//Unicode to UTF-8
	//////////********///////////
	msg->LinkEndChild( new TiXmlText( utf8Str )); 
	msgs->LinkEndChild( msg ); 

	msg = new TiXmlElement( "LineWidth" ); 
	str.Format(_T("%d"),plist->lineWidth);
	////********/////transfer//////********//////
	memset(utf8Str,0,maxBufferSize*sizeof(char));
	memset(UnicodeStr,0,maxBufferSize*sizeof(unsigned short));
	UnicodeStrLen=2*str.GetLength();
	memcpy(UnicodeStr,str,UnicodeStrLen);
	utf8StrLen=6*UnicodeStrLen;
	memset(utf8Str,0,maxBufferSize*sizeof(char));
	dwLength=theApp.FromUnicodeToUTF8( utf8Str, utf8StrLen, UnicodeStr, UnicodeStrLen);//Unicode to UTF-8
	//////////********///////////
	msg->LinkEndChild( new TiXmlText( utf8Str )); 
	msgs->LinkEndChild( msg ); 

	msg = new TiXmlElement( "WrapMode" ); 
	str.Format(_T("%d"),plist->wrapMode);
	////********/////transfer//////********//////
	memset(utf8Str,0,maxBufferSize*sizeof(char));
	memset(UnicodeStr,0,maxBufferSize*sizeof(unsigned short));
	UnicodeStrLen=2*str.GetLength();
	memcpy(UnicodeStr,str,UnicodeStrLen);
	utf8StrLen=6*UnicodeStrLen;
	memset(utf8Str,0,maxBufferSize*sizeof(char));
	dwLength=theApp.FromUnicodeToUTF8( utf8Str, utf8StrLen, UnicodeStr, UnicodeStrLen);//Unicode to UTF-8
	//////////********///////////
	msg->LinkEndChild( new TiXmlText( utf8Str )); 
	msgs->LinkEndChild( msg ); 

	msg = new TiXmlElement( "WrapCount" ); 
	str.Format(_T("%d"),plist->wrapCount);
	////********/////transfer//////********//////
	memset(utf8Str,0,maxBufferSize*sizeof(char));
	memset(UnicodeStr,0,maxBufferSize*sizeof(unsigned short));
	UnicodeStrLen=2*str.GetLength();
	memcpy(UnicodeStr,str,UnicodeStrLen);
	utf8StrLen=6*UnicodeStrLen;
	memset(utf8Str,0,maxBufferSize*sizeof(char));
	dwLength=theApp.FromUnicodeToUTF8( utf8Str, utf8StrLen, UnicodeStr, UnicodeStrLen);//Unicode to UTF-8
	//////////********///////////
	msg->LinkEndChild( new TiXmlText( utf8Str )); 
	msgs->LinkEndChild( msg ); 
}