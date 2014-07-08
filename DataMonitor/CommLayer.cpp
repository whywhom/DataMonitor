// CommLayer.cpp : 实现文件
//

#include "stdafx.h"
#include <math.h>
#include "DataMonitor.h"
#include "CommLayer.h"
#include "SerialPort.h"


#define CRC_16_L_SEED           0xFFFF
#define CRC_16_L_POLYNOMIAL     0x8408
/* 2^CRC_TAB_BITS      */
#define CRC_TAB_SIZE    256

/* CRC table for 16 bit CRC, with generator polynomial 0x8408,
** calculated 8 bits at a time, LSB first.  This table is used
** from a macro in sio.c, so it is indirected through the jump
** table and renamed with prepended bb_.  This is the only CRC
** table that is referenced from outside the Boot Block.
*/
WORD bb_crc_16_l_table[ CRC_TAB_SIZE ] =
    {
        0x0000, 0x1189, 0x2312, 0x329b, 0x4624, 0x57ad, 0x6536, 0x74bf,
        0x8c48, 0x9dc1, 0xaf5a, 0xbed3, 0xca6c, 0xdbe5, 0xe97e, 0xf8f7,
        0x1081, 0x0108, 0x3393, 0x221a, 0x56a5, 0x472c, 0x75b7, 0x643e,
        0x9cc9, 0x8d40, 0xbfdb, 0xae52, 0xdaed, 0xcb64, 0xf9ff, 0xe876,
        0x2102, 0x308b, 0x0210, 0x1399, 0x6726, 0x76af, 0x4434, 0x55bd,
        0xad4a, 0xbcc3, 0x8e58, 0x9fd1, 0xeb6e, 0xfae7, 0xc87c, 0xd9f5,
        0x3183, 0x200a, 0x1291, 0x0318, 0x77a7, 0x662e, 0x54b5, 0x453c,
        0xbdcb, 0xac42, 0x9ed9, 0x8f50, 0xfbef, 0xea66, 0xd8fd, 0xc974,
        0x4204, 0x538d, 0x6116, 0x709f, 0x0420, 0x15a9, 0x2732, 0x36bb,
        0xce4c, 0xdfc5, 0xed5e, 0xfcd7, 0x8868, 0x99e1, 0xab7a, 0xbaf3,
        0x5285, 0x430c, 0x7197, 0x601e, 0x14a1, 0x0528, 0x37b3, 0x263a,
        0xdecd, 0xcf44, 0xfddf, 0xec56, 0x98e9, 0x8960, 0xbbfb, 0xaa72,
        0x6306, 0x728f, 0x4014, 0x519d, 0x2522, 0x34ab, 0x0630, 0x17b9,
        0xef4e, 0xfec7, 0xcc5c, 0xddd5, 0xa96a, 0xb8e3, 0x8a78, 0x9bf1,
        0x7387, 0x620e, 0x5095, 0x411c, 0x35a3, 0x242a, 0x16b1, 0x0738,
        0xffcf, 0xee46, 0xdcdd, 0xcd54, 0xb9eb, 0xa862, 0x9af9, 0x8b70,
        0x8408, 0x9581, 0xa71a, 0xb693, 0xc22c, 0xd3a5, 0xe13e, 0xf0b7,
        0x0840, 0x19c9, 0x2b52, 0x3adb, 0x4e64, 0x5fed, 0x6d76, 0x7cff,
        0x9489, 0x8500, 0xb79b, 0xa612, 0xd2ad, 0xc324, 0xf1bf, 0xe036,
        0x18c1, 0x0948, 0x3bd3, 0x2a5a, 0x5ee5, 0x4f6c, 0x7df7, 0x6c7e,
        0xa50a, 0xb483, 0x8618, 0x9791, 0xe32e, 0xf2a7, 0xc03c, 0xd1b5,
        0x2942, 0x38cb, 0x0a50, 0x1bd9, 0x6f66, 0x7eef, 0x4c74, 0x5dfd,
        0xb58b, 0xa402, 0x9699, 0x8710, 0xf3af, 0xe226, 0xd0bd, 0xc134,
        0x39c3, 0x284a, 0x1ad1, 0x0b58, 0x7fe7, 0x6e6e, 0x5cf5, 0x4d7c,
        0xc60c, 0xd785, 0xe51e, 0xf497, 0x8028, 0x91a1, 0xa33a, 0xb2b3,
        0x4a44, 0x5bcd, 0x6956, 0x78df, 0x0c60, 0x1de9, 0x2f72, 0x3efb,
        0xd68d, 0xc704, 0xf59f, 0xe416, 0x90a9, 0x8120, 0xb3bb, 0xa232,
        0x5ac5, 0x4b4c, 0x79d7, 0x685e, 0x1ce1, 0x0d68, 0x3ff3, 0x2e7a,
        0xe70e, 0xf687, 0xc41c, 0xd595, 0xa12a, 0xb0a3, 0x8238, 0x93b1,
        0x6b46, 0x7acf, 0x4854, 0x59dd, 0x2d62, 0x3ceb, 0x0e70, 0x1ff9,
        0xf78f, 0xe606, 0xd49d, 0xc514, 0xb1ab, 0xa022, 0x92b9, 0x8330,
        0x7bc7, 0x6a4e, 0x58d5, 0x495c, 0x3de3, 0x2c6a, 0x1ef1, 0x0f78
    };
// CCommLayer

IMPLEMENT_DYNAMIC(CCommLayer, CCmdTarget)


CCommLayer::CCommLayer()
{
}

CCommLayer::~CCommLayer()
{
}


BEGIN_MESSAGE_MAP(CCommLayer, CCmdTarget)
END_MESSAGE_MAP()



// CCommLayer 消息处理程序
/**************************************************************************/
/***  specifications;              ***/
/***  NAME   = FromGBToUNICODE;       ***/
/***  FUNCTION  = GB码转换为UNICODE   ***/
/***  DATE   = 2008/11/18;           ***/
/***  AUTHOR  = wuhaoyong;              ***/
/***  INPUT   = *pData-GB字符串,*nDataLen-GB字符串有效长度,*pwDataLen-转换为UNICODE的长度;               ***/
/***  OUTPUT  = *pwBuffer-UNICODE字符串;               ***/
/***  END OF SPECIFICATIONS;            ***/
/**************************************************************************/
wchar_t *CCommLayer::FromGBToUNICODE(char *pData, int *nDataLen,
                                     int*pwDataLen)
{
    UINT nCodePage = 936; //GBK
    int nLength = MultiByteToWideChar(nCodePage, 0, pData,  - 1, NULL, 0);
    wchar_t *pwBuffer = new wchar_t[nLength + 1];
    *pwDataLen = MultiByteToWideChar(nCodePage, 0, pData,  - 1, pwBuffer, nLength)
                 ;
    return pwBuffer;
}
/**************************************************************************/
/***  specifications;              ***/
/***  NAME   = FromUNICODEToGB;       ***/
/***  FUNCTION  = UNICODE转换为GB码   ***/
/***  DATE   = 2008/11/18;           ***/
/***  AUTHOR  = wuhaoyong;              ***/
/***  INPUT   = *pData-UNICODE字符串,*nwDataLen-UNICODE字符串有效长度,*pDataLen-转换为GB的长度;               ***/
/***  OUTPUT  = *pBuffer-GB字符串;               ***/
/***  END OF SPECIFICATIONS;            ***/
/**************************************************************************/
char *CCommLayer::FromUNICODEToGB(wchar_t *pData, int *nwDataLen,
                                  int*pDataLen)
{
    int m_WDateLen=*nwDataLen;

    UINT nCodePage = 936; //GB2312
    *pDataLen = WideCharToMultiByte(nCodePage, 0, pData,  (m_WDateLen/2), NULL, 0, NULL,
                                    NULL);
    char *pBuffer = new char[ *pDataLen + 1];
    memset(pBuffer, 0,  *pDataLen + 1);
    WideCharToMultiByte(nCodePage, 0, pData,  - 1, pBuffer,  *pDataLen, NULL,
                        NULL);
    delete []pData;
    pData = NULL;
    return pBuffer;
}
/**************************************************************************/
/***  specifications;              ***/
/***  NAME   = GetRegisterdComPort;       ***/
/***  FUNCTION  = enumerates the values for the specified open registry key   ***/
/***  DATE   = 2008/11/07;           ***/
/***  AUTHOR  = wuhaoyong;              ***/
/***  INPUT   = ;               ***/
/***  OUTPUT  = ;               ***/
/***  END OF SPECIFICATIONS;            ***/
/**************************************************************************/
int CCommLayer::GetRegisterdComPort(SubKeyInfo_type* SubKey, DWORD* number)
{
    const HKEY registerkey=HKEY_LOCAL_MACHINE;
    const LPCTSTR kename=_T("HARDWARE\\DEVICEMAP\\SERIALCOMM");
    HKEY qusbregisterkey;   //father key handle

    /////////////////////////////////////
    //加入这些语句防止RegEnumValue函数在release版本下工作不正常！2008-11-07
    for(int i=0;i<array_size;i++)
    {
        SubKey[i].SubKeyNameLength=80;
        SubKey[i].SubKeyValueLength=80;
    }
    /////////////////////////////////////

    DWORD subkeyindex=0;         //temp index
    LONG returnvalue;
    DWORD type_1=REG_SZ;

    //opens the specified registry key. Note that key names are not case sensitive.
    returnvalue=RegOpenKeyEx(registerkey,kename,0,KEY_READ,&qusbregisterkey);
    //If the function succeeds, the return value is ERROR_SUCCESS.
    if(returnvalue!=ERROR_SUCCESS)
    {
        return 1;//error
    }

    do
    {
        //enumerates the values for the specified open registry key. The function copies one indexed value name and data block for the key each time it is called.
        returnvalue=RegEnumValue(qusbregisterkey,subkeyindex,
                                 SubKey[subkeyindex].SubKeyName,
                                 &(SubKey[subkeyindex].SubKeyNameLength),
                                 NULL,&type_1,(LPBYTE)(SubKey[subkeyindex].SubKeyValue),
                                 &(SubKey[subkeyindex].SubKeyValueLength));
        if(returnvalue!=ERROR_SUCCESS)  //If the function succeeds, the return value is ERROR_SUCCESS.
        {
            if(returnvalue==ERROR_NO_MORE_ITEMS)  //there are no more values
            {
                break;
            }
            else
            {   //取子键名出错！
                RegCloseKey(qusbregisterkey);
                return 1;
            }
        }
        subkeyindex++;
    }
    while(1);
    RegCloseKey(qusbregisterkey);
    *number=subkeyindex;
    return 0;
}

/**************************************************************************/
/***  specifications;              ***/
/***  NAME   = SelectComPort;       ***/
/***  FUNCTION  = enumerates the com port   ***/
/***  DATE   = 2008/11/07;           ***/
/***  AUTHOR  = wuhaoyong;              ***/
/***  INPUT   = ;               ***/
/***  OUTPUT  = ;               ***/
/***  END OF SPECIFICATIONS;            ***/
/**************************************************************************/
int CCommLayer::SelectComPort(void)
{
    BOOL m_bReturnvalue;
    CString comportname;
    num--;
    if(-1==num ||0xFFFFFFFF==num)
    {
        return 2;//no valid com port
    }
    //try initial,连接当前端口成功，发送验证码
    m_bReturnvalue=theApp.commLayer.m_SerialPort.InitPort(SubKey[num].SubKeyValue);
    TRACE(_T("Initial COM\n"));

    if(TRUE == m_bReturnvalue)
    {
        //连接当前端口成功
        if(theApp.commLayer.m_SerialPort.StartMonitoring())
        {
            return 0;
        }
        else
        {
            //the MS is FAIL
            theApp.commLayer.m_SerialPort.ClosePort();//关闭串口
            theApp.commLayer.m_bComPortEffective=FALSE;
            theApp.commLayer.m_bConnectEffective=FALSE;
            return 1;
        }
    }
    else
    {
        return 1;
    }
    return 0;
}

/**************************************************************************/
/***  specifications;              ***/
/***  NAME   = SendVerification;       ***/
/***  FUNCTION  = send verification data  ***/
/***  DATE   = 2008/11/07;           ***/
/***  AUTHOR  = wuhaoyong;              ***/
/***  INPUT   = ;               ***/
/***  OUTPUT  = ;               ***/
/***  END OF SPECIFICATIONS;            ***/
/**************************************************************************/
int CCommLayer::SendVerification(void)
{
    if(m_CallType==TYPE_DLL)
    {
        BYTE m_verify[4];
        m_verify[0]=0x16;
        m_verify[1]=0x00;
        m_verify[2]=0x20;
        m_verify[3]=0x00;
        TransData(m_verify,4);
    }
    else if(m_CallType==TYPE_COMM)
    {
        m_req.maincmd=0x16;
        m_req.subcmd=0x00;
        m_req.childcmd=0x0005;
        m_req.state=0x0000;
        TransData((BYTE *)&m_req,sizeof(m_req));
    }
    else
    {}
    return 0;
}
/**************************************************************************/
/***  specifications;              ***/
/***  NAME   = TransData;       ***/
/***  FUNCTION  = send data to the com port  ***/
/***  DATE   = 2008/11/07;           ***/
/***  AUTHOR  = wuhaoyong;              ***/
/***  INPUT   = ;               ***/
/***  OUTPUT  = ;               ***/
/***  END OF SPECIFICATIONS;            ***/
/**************************************************************************/
WORD CCommLayer::TransData(BYTE* outbuff, DWORD dwSize)
{
    WORD returnvalue;
    memset(m_WriteBuffer,0,sizeof(m_WriteBuffer));
    m_WriteBufferSize = MakePacket(m_WriteBuffer, outbuff, (WORD)dwSize);
	returnvalue=m_SerialPort.WriteToPort(m_WriteBuffer,m_WriteBufferSize);
    //此处不能立即重置发送buffer，因为wirtefile还没有完成，此时重置，
    //会导致发送不成功，因而收不到相应的响应。
    return WORD(returnvalue);
}

/**************************************************************************/
/***  specifications;              ***/
/***  NAME   = MakePacket;       ***/
/***  FUNCTION  = HDLC    ***/
/***  HDLC is a bit-oriented, synchronous protocol that     ***/
/***  applies to the data-link (message-packaging) layer     ***/
/***  (layer 2 of the ISO/OSI reference model) for computer-to-microcomputer     ***/
/***  communications. Messages are transmitted in units called frames,     ***/
/***  which can contain differing amounts of data     ***/
/***  but which must be organized in a particular way    ***/
/***  DATE   = 2008/11/07;           ***/
/***  AUTHOR  = wuhaoyong;              ***/
/***  INPUT   = ;               ***/
/***  OUTPUT  = ;               ***/
/***  END OF SPECIFICATIONS;            ***/
/**************************************************************************/
WORD CCommLayer::MakePacket(BYTE* pDest, BYTE* pSource, WORD wSize)
{
    WORD wSize2 = wSize;
    WORD i;
    WORD j = 0;

    //CRC
    WORD wCRC = CalcCRC(pSource, wSize * 8);

    for(j = 0; j < 1; ++j)
    {
        pDest[j] = 0x7e;
    }
    for(i = 0; i < wSize; ++i)
    {
        if(pSource[i] == 0x7d || pSource[i] == 0x7e)
        {
            pDest[j] = 0x7d;
            ++j;
            pDest[j] = pSource[i] ^ 0x20;
        }
        else
        {
            pDest[j] = pSource[i];
        }
        ++j;
    }
    pDest[j] = wCRC & 0xff;
    if(pDest[j] == 0x7d || pDest[j] == 0x7e)
    {
        pDest[j + 1] = pDest[j] ^ 0x20;
        pDest[j] = 0x7d;
        ++j;
    }
    ++j;
    pDest[j] = (wCRC >> 8) & 0xff;
    if(pDest[j] == 0x7d || pDest[j] == 0x7e)
    {
        pDest[j + 1] = pDest[j] ^ 0x20;
        pDest[j] = 0x7d;
        ++j;
    }
    ++j;
    pDest[j] = 0x7e;
    ++j;
    wSize2 = j;

    return wSize2;
}

/**************************************************************************/
/***  specifications;              ***/
/***  NAME   = CalcCRC;       ***/
/***  FUNCTION  = Cyclic Redundancy Check    ***/
/***  DATE   = 2008/11/07;           ***/
/***  AUTHOR  = wuhaoyong;              ***/
/***  INPUT   = ;               ***/
/***  OUTPUT  = ;               ***/
/***  END OF SPECIFICATIONS;            ***/
/**************************************************************************/
WORD CCommLayer::CalcCRC(BYTE* buf_ptr, WORD len)
{
    WORD data, crc_16;

    /* Generate a CRC-16 by looking up the transformation in a table and
    ** XOR-ing it into the CRC, one byte at a time.
    */
    for (crc_16 = CRC_16_L_SEED ; len >= 8; len -= 8, buf_ptr++)
    {
        crc_16 = bb_crc_16_l_table[ (crc_16 ^ *buf_ptr) & 0x00ff ] ^ (crc_16 >> 8);
    }

    /* Finish calculating the CRC over the trailing data bits
    **
    ** XOR the MS bit of data with the MS bit of the CRC.
    ** Shift the CRC and data left 1 bit.
    ** If the XOR result is 1, XOR the generating polynomial in with the CRC.
    */
    if (len != 0)
    {

        data = ((WORD) (*buf_ptr)) << (16-8); /* Align data MSB with CRC MSB */

        while (len-- != 0)
        {
            if ( ((crc_16 ^ data) & 0x01) != 0 )
            {   /* Is LSB of XOR a 1 */

                crc_16 >>= 1;                   /* Right shift CRC         */
                crc_16 ^= CRC_16_L_POLYNOMIAL;  /* XOR polynomial into CRC */

            }
            else
            {

                crc_16 >>= 1;                   /* Right shift CRC         */

            }

            data >>= 1;                       /* Right shift data        */
        }
    }

    return( ~crc_16 );            /* return the 1's complement of the CRC */

}

/**************************************************************************/
/***  specifications;              ***/
/***  NAME   = ReceiveData;       ***/
/***  FUNCTION  = PCSuite receive data from Dll  ***/
/***  DATE   = 2008/12/29;           ***/
/***  AUTHOR  = wuhaoyong;              ***/
/***  INPUT   = ;               ***/
/***  OUTPUT  = ;               ***/
/***  END OF SPECIFICATIONS;            ***/
/**************************************************************************/
void CCommLayer::RecvDataForPCSuite(BYTE* inbuff, DWORD* dwSize)
{
    *dwSize=(DWORD)m_ReceiveBufferSize;
    memcpy(inbuff, m_ReceiveBuff, m_ReceiveBufferSize);
    memset(m_ReceiveBuff,0,sizeof(m_ReceiveBuff));
    m_ReceiveBufferSize=0;
    return;
}
/**************************************************************************/
/***  specifications;              ***/
/***  NAME   = RecvData;       ***/
/***  FUNCTION  = receive data from com port  ***/
/***  DATE   = 2008/11/07;           ***/
/***  AUTHOR  = wuhaoyong;              ***/
/***  INPUT   = ;               ***/
/***  OUTPUT  = ;               ***/
/***  END OF SPECIFICATIONS;            ***/
/**************************************************************************/
WORD CCommLayer::RecvData(BYTE* inbuff, WORD dwSize)
{
    BYTE localReceiveBuff[4*1024];     //接收到的原始数据的缓冲
    BOOL bEndPacket = TRUE;  //数据尾
    BOOL crcright;
    m_wlocalReceivePtr = 0;
    m_ReceiveBufferSize=0;
    memset(m_ReceiveBuff,0,sizeof(m_ReceiveBuff));
    memset(localReceiveBuff, 0, sizeof(localReceiveBuff));
    memcpy(localReceiveBuff, inbuff, dwSize);
    //解包并做CRC校验
    while(bEndPacket == TRUE)
    {
        bEndPacket = DecodeReceivePacket(m_ReceiveBuff, &m_ReceiveBufferSize, &localReceiveBuff[m_wlocalReceivePtr], &dwSize);
        if(ReceivewholePacket==FALSE)
        {
            if(theApp.commLayer.m_CallType==TYPE_PIM)
            {
                //数据包解码错误
                ::SendMessage(theApp.commLayer.fatherHwnd,WM_USER_RECEIVEDATA, 0, 0x0001);
            }
            return 1;
        }
        if(bEndPacket == TRUE)
        {
            if(m_ReceiveBufferSize > 0)
            {
                crcright=CheckReceivePacketStatus(m_ReceiveBuff, &m_ReceiveBufferSize);
                if(crcright==FALSE)
                {
                    memset(m_ReceiveBuff, 0, sizeof(m_ReceiveBuff));
                    m_ReceiveBufferSize=0;
                    if(theApp.commLayer.m_CallType==TYPE_PIM)
                    {
                        //数据包CRC校验错误
                        ::SendMessage(theApp.commLayer.fatherHwnd,WM_USER_RECEIVEDATA, 0, 0x0002);
                    }
                    return 0;
                }
            }
        }
    }
    theApp.commLayer.RecvDataAnalysis(m_ReceiveBuff,m_ReceiveBufferSize);
    return WORD(dwSize);
}
/**************************************************************************/
/***  specifications;              ***/
/***  NAME   = DecodeReceivePacket;       ***/
/***  FUNCTION  = decode received HDLC data  ***/
/***  DATE   = 2008/11/07;           ***/
/***  AUTHOR  = wuhaoyong;              ***/
/***  INPUT   = ;               ***/
/***  OUTPUT  = ;               ***/
/***  END OF SPECIFICATIONS;            ***/
/**************************************************************************/
BOOL CCommLayer::DecodeReceivePacket(BYTE* pDest, WORD *pwSize2, BYTE *pSource, WORD *pwSize)
{
    WORD wSize = *pwSize;
    WORD wSize2 = *pwSize2;
    WORD i;
    WORD j = 0;
    BOOL bEndPacket = FALSE;

    for(i = 0; i < wSize; ++i)
    {
        if(pSource[i] == 0x7e)
        {
            if(m_bTopFlagByte == TRUE)
            {
                ReceivewholePacket=FALSE;
                continue;
            }
            else
            {
                ++i;
                bEndPacket = TRUE;
                ReceivewholePacket=TRUE;
                m_bTopFlagByte = TRUE;
                break;
            }
        }
        else if(pSource[i] == 0x7d)
        {
            m_bTopFlagByte = FALSE;
            m_bEscapeByte = TRUE;
        }
        else
        {
            m_bTopFlagByte = FALSE;
            if(m_bEscapeByte == TRUE)
            {
                pDest[wSize2 + j] = pSource[i] ^ 0x20;
            }
            else
            {
                pDest[wSize2 + j] = pSource[i];
                ReceivewholePacket=FALSE;
            }
            m_bEscapeByte = FALSE;
            ++j;
        }
    }
    *pwSize2 = wSize2 + j;

    *pwSize = wSize - i;

    m_wlocalReceivePtr += i;

    return bEndPacket;
}
/**************************************************************************/
/***  specifications;              ***/
/***  NAME   = CheckReceivePacketStatus;       ***/
/***  FUNCTION  = check crc of receive data   ***/
/***  DATE   = 2008/11/07;           ***/
/***  AUTHOR  = wuhaoyong;              ***/
/***  INPUT   = ;               ***/
/***  OUTPUT  = ;               ***/
/***  END OF SPECIFICATIONS;            ***/
/**************************************************************************/
BOOL CCommLayer::CheckReceivePacketStatus(BYTE *pbBuff, WORD *wSize)
{
    WORD wCRC;
    WORD wCRC2;
    int crcptr;

    crcptr = *wSize;

    if(crcptr <= 2)
    {
        wCRC = 1;
        wCRC2 = 0;
    }
    else
    {
        wCRC = CalcCRC(pbBuff, (crcptr - 2) * 8);
        wCRC2 = (pbBuff[crcptr - 2] & 0x00ff) | ((pbBuff[crcptr - 1] << 8) & 0xff00);
    }

    if(wCRC != wCRC2)
    {
        *wSize-=2;
        memset(&pbBuff[*wSize],0x00,2);
        return FALSE;
    }
    else
    {
        *wSize-=2;
        memset(&pbBuff[*wSize],0x00,2);
        return TRUE;
    }
}

/**************************************************************************/
/***  specifications;              ***/
/***  NAME   = RecvDataAnalysis;       ***/
/***  FUNCTION  = analyse received data from com port  ***/
/***  DATE   = 2008/11/07;           ***/
/***  AUTHOR  = wuhaoyong;              ***/
/***  INPUT   = ;               ***/
/***  OUTPUT  = ;               ***/
/***  END OF SPECIFICATIONS;            ***/
/**************************************************************************/
WORD CCommLayer::RecvDataAnalysis(BYTE* m_ReceiveBuff, WORD m_ReceiveBufferSize)
{
    //theApp.rsp=(DIAG_PIM_HEADCMD_F_rsp_type*) m_ReceiveBuff;
	return WORD(0);
}

/**************************************************************************/
/***  specifications;              ***/
/***  NAME   = ChangeCStringToWORD;       ***/
/***  FUNCTION  = 针对组号，将字符串转为数字 ***/
/***  DATE   = 2008/11/18;           ***/
/***  AUTHOR  = wuhaoyong;              ***/
/***  INPUT   = ;               ***/
/***  OUTPUT  = ;               ***/
/***  END OF SPECIFICATIONS;            ***/
/**************************************************************************/
WORD CCommLayer::ChangeCStringToWORD (CString strChange)
{
    CString strTemp;
    WORD dwSum = 0;
    int nCharLength = strChange.GetLength ();
    for (int nTempCount = 0; nTempCount < nCharLength; nTempCount++)
    {
        if (strChange.GetAt (nTempCount) != ' ')
        {
            strTemp += strChange.GetAt (nTempCount);
        }
    }

    nCharLength = strTemp.GetLength ();
    if (nCharLength > 0)
    {
        dwSum = 0;
        for (int nTempCount = nCharLength; nTempCount > 0; nTempCount--)
        {
            dwSum = WORD(dwSum + (strTemp.GetAt (nCharLength - nTempCount) - 48) * pow ((float)10, (int)(nTempCount - 1)));
        }
    }
    return dwSum;
}


/**************************************************************************/
/***  specifications;              ***/
/***  NAME   = ChangeCStringToDWORD;       ***/
/***  FUNCTION  = 针对组号，将字符串转为数字 ***/
/***  DATE   = 2008/11/18;           ***/
/***  AUTHOR  = wuhaoyong;              ***/
/***  INPUT   = ;               ***/
/***  OUTPUT  = ;               ***/
/***  END OF SPECIFICATIONS;            ***/
/**************************************************************************/
DWORD CCommLayer::ChangeCStringToDWORD (CString strChange)
{
    CString strTemp;
    DWORD dwSum = 0;
    int nCharLength = strChange.GetLength ();
    for (int nTempCount = 0; nTempCount < nCharLength; nTempCount++)
    {
        if (strChange.GetAt (nTempCount) != ' ')
        {
            strTemp += strChange.GetAt (nTempCount);
        }
    }

    nCharLength = strTemp.GetLength ();
    if (nCharLength > 0)
    {
        dwSum = 0;
        for (int nTempCount = nCharLength; nTempCount > 0; nTempCount--)
        {
            dwSum = DWORD(dwSum + (strTemp.GetAt (nCharLength - nTempCount) - 48) * pow ((float)10, (int)(nTempCount - 1)));
        }
    }
    return dwSum;
}