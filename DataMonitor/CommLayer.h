
#pragma once

#include "SerialPort.h"

#define WM_USER_RECEIVEDATA (WM_USER+7)
#define WM_CONNECT_ERR      (WM_USER+8)
#define WM_CONNECT_OK      (WM_USER+9)
const int array_size =100;
const BYTE diag_maincmd =0x16;

typedef struct
{
    DWORD  SubKeyIndex;
    TCHAR   SubKeyName[80];
    DWORD  SubKeyNameLength;
    TCHAR   SubKeyValue[80];
    DWORD  SubKeyValueLength;
}SubKeyInfo_type;

typedef struct _DIAG_PIM_HEADCMD
{
    BYTE maincmd;
    BYTE subcmd;
    WORD childcmd;
    WORD state;
}DIAG_PIM_HEADCMD;

typedef enum
{
    UNSIGNALED,
    SIGNALED,
}Event_type;

typedef enum
{
    TYPE_NONE,
    TYPE_PIM,
    TYPE_DLL,
    TYPE_COMM
}Calling_type;
// CCommLayer 命令目标

class CCommLayer : public CCmdTarget
{
	DECLARE_DYNAMIC(CCommLayer)
public:
	HWND fatherHwnd;  //调用dll的应用程序窗口指针。
	CSerialPort m_SerialPort;
    SubKeyInfo_type SubKey[array_size];//sub keys
    DWORD m_PhoneUsedNum;//话机已存记录数量
    DWORD m_UimUsedNum;//UIM卡已存记录数量
    DWORD m_SimUsedNum;//SIM卡已存记录数量
    DWORD m_TDUsedNum;//TD备用,已存记录数量
    int m_FuncReturnValue;
    DWORD comnumber;//有效的COM口数量
    DWORD num;//有效的COM口数量
    WORD m_RetryTimes;
    BYTE m_WriteBuffer[4*1024];//send buffer
    WORD m_WriteBufferSize;
    BYTE m_ReceiveBuff[4*1024];//receive buffer
    WORD m_ReceiveBufferSize;
    BOOL ReceivewholePacket;     //接收的是整个packet
    BOOL m_bTopFlagByte;             //第一个0x7E标志
    BOOL m_bEscapeByte;              //数据中有0x7E\0x7D的置换标志
    BOOL m_bComPortEffective; //端口有效
    BOOL m_bConnectEffective; //连接有效,断开连接后此值为FALSE
    BOOL bTaskCanceled;//执行取消函数接口
    WORD m_wlocalReceivePtr;         //数据bytes接收位置
    BYTE m_Mingroup;//组号最小序号
	BYTE m_Maxgroup;//组号最大序号

    DIAG_PIM_HEADCMD m_req;

    CEvent g_event;
    CEvent g_eventcom;
    Event_type m_EventStatus;
    Calling_type m_CallType;

	BOOL m_bISAddField;  
    CString m_SFamilyname;
    CString m_SGivenname;
	CString m_SCountry;
	CString m_SProvince;
	CString m_SCity;
	CString m_SStreet;
	CString m_SPostbox;
	CString m_SPostcode;
	CString m_SLabel;
	CString m_SExtension;
	CString m_SRegion;
public:
	CCommLayer();
	virtual ~CCommLayer();

	wchar_t * FromGBToUNICODE(char *pData,int *nDataLen,int *pwDataLen);
    char * FromUNICODEToGB(wchar_t *pData, int *nwDataLen, int *pDataLen);
    int GetRegisterdComPort(SubKeyInfo_type* SubKey, DWORD* number);
    int SelectComPort(void);
    int SendVerification(void);
    WORD TransData(BYTE* outbuff, DWORD dwSize);
    WORD MakePacket(BYTE* pDest, BYTE* pSource, WORD wSize);
    WORD CalcCRC(BYTE* buf_ptr, WORD len);
	void RecvDataForPCSuite(BYTE* inbuff, DWORD* dwSize);
    WORD RecvData(BYTE* inbuff, WORD dwSize);
    BOOL DecodeReceivePacket(BYTE* pDest, WORD *pwSize2, BYTE *pSource, WORD *pwSize);
    BOOL CheckReceivePacketStatus(BYTE *pbBuff, WORD *wSize);
    WORD RecvDataAnalysis(BYTE* m_ReceiveBuff, WORD m_ReceiveBufferSize);
    WORD ChangeCStringToWORD (CString strChange);
	DWORD ChangeCStringToDWORD (CString strChange);

protected:
	DECLARE_MESSAGE_MAP()
};


