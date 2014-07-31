
#include "StdAfx.h"
#include "SerialPort.h"
#include "DataMonitor.h"


extern CDataMonitorApp theApp;

CSerialPort::CSerialPort(void)
{
    m_hComm = NULL;
    // initialize overlapped structure members to zero
    m_ov.Offset = 0;
    m_ov.OffsetHigh = 0;

    // create events
    m_ov.hEvent = NULL;
    m_hWriteEvent = NULL;
    m_hShutdownEvent = NULL;
    m_nWriteSize=1;
    m_bThreadAlive = FALSE;
}

CSerialPort::~CSerialPort(void)
{
    if (m_bThreadAlive)
    {
        do
        {
            SetEvent(m_hShutdownEvent);
        }
        while (m_bThreadAlive);
        TRACE(_T("~CSerialPort Thread ended\n"));
    }

    // if the port is still opened: close it
    if (m_hComm != NULL)
    {
        CloseHandle(m_hComm);
        m_hComm = NULL;
    }
    // Close Handles
    if(m_hShutdownEvent!=NULL)
        CloseHandle( m_hShutdownEvent);
    if(m_ov.hEvent!=NULL)
        CloseHandle( m_ov.hEvent );
    if(m_hWriteEvent!=NULL)
        CloseHandle( m_hWriteEvent );

}

//
// Initialize the port. This can be port 1 to 4.
//
BOOL CSerialPort::InitPort(LPCTSTR  pCommPort,  // portnumber (COM1..COM4)
                           UINT  baud,   // baudrate
                           BYTE  parity,  // parity
                           UINT  databits,  // databits
                           UINT  stopbits,  // stopbits
                           DWORD dwCommEvents // EV_RXCHAR, EV_CTS etc)
                          )
{
    // if the thread is alive: Kill
    if (m_bThreadAlive)
    {
        do
        {
            SetEvent(m_hShutdownEvent);
        }
        while (m_bThreadAlive);
        TRACE(_T("InitPort Thread ended\n"));
    }

    // create events
    if (m_ov.hEvent != NULL)
    {
        ResetEvent(m_ov.hEvent);
    }
    else
    {
        m_ov.hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
    }
    if (m_hWriteEvent != NULL)
    {
        ResetEvent(m_hWriteEvent);
    }
    else
    {
        m_hWriteEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
    }
    if (m_hShutdownEvent != NULL)
    {
        ResetEvent(m_hShutdownEvent);
    }
    else
    {
        m_hShutdownEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
    }
    // initialize the event objects
    m_hEventArray[0] = m_hShutdownEvent; // highest priority
    m_hEventArray[1] = m_ov.hEvent;
    m_hEventArray[2] = m_hWriteEvent;

    // initialize critical section
    InitializeCriticalSection(&m_csCommunicationSync);

    // set buffersize for writing and save the owner
    memset(m_szWriteBuffer,0,sizeof(m_szWriteBuffer));
    m_nWriteBufferSize = sizeof(m_szWriteBuffer);
    m_dwCommEvents = dwCommEvents;

    BOOL bResult = FALSE;

    // now it critical!
    EnterCriticalSection(&m_csCommunicationSync);

    // if the port is already opened: close it
    if (m_hComm != NULL)
    {
        CloseHandle(m_hComm);
        m_hComm = NULL;
    }

    // prepare port strings
    m_nPortNr.Format(_T("\\\\.\\%s"),pCommPort);

    // get a handle to the port
    m_hComm = CreateFile(m_nPortNr,      // communication port string (COMX)
                         GENERIC_READ | GENERIC_WRITE, // read/write types
                         0,        // comm devices must be opened with exclusive access
                         NULL,       // no security attributes
                         OPEN_EXISTING,     // comm devices must use OPEN_EXISTING
                         FILE_FLAG_OVERLAPPED,   // Async I/O
                         0);       // template must be 0 for comm devices

    if (m_hComm == INVALID_HANDLE_VALUE)
    {
        // port not found
        return FALSE;
    }

    // set the timeout values
    m_CommTimeouts.ReadIntervalTimeout = 1000;
    m_CommTimeouts.ReadTotalTimeoutMultiplier = 0;
    m_CommTimeouts.ReadTotalTimeoutConstant = 1000;
    m_CommTimeouts.WriteTotalTimeoutMultiplier = 5;
    m_CommTimeouts.WriteTotalTimeoutConstant = 1000;

    // configure
    if (SetCommTimeouts(m_hComm, &m_CommTimeouts))
    {
        if (SetCommMask(m_hComm, dwCommEvents))
        {
            if (GetCommState(m_hComm, &m_dcb))
            {
                //DCB
                m_dcb.BaudRate = baud;
                m_dcb.fBinary = TRUE;
                m_dcb.fParity = FALSE;
                m_dcb.fOutxCtsFlow = FALSE;
                m_dcb.fOutxDsrFlow = FALSE;
                m_dcb.fDtrControl = DTR_CONTROL_ENABLE;
                m_dcb.fRtsControl = RTS_CONTROL_ENABLE;
                m_dcb.fOutX = FALSE;
                m_dcb.fInX = FALSE;
                m_dcb.ByteSize = 8;
                m_dcb.Parity = parity;
                m_dcb.StopBits = ONESTOPBIT;
                m_dcb.EvtChar = 0x0;
                SetCommState(m_hComm, &m_dcb);
                if (!SetCommState(m_hComm, &m_dcb))
                {
                    ProcessErrorMessage(0);//"SetCommState()"
                }
            }
            else
            {
                ProcessErrorMessage(1);//"GetCommState()"
            }
        }
        else
        {
            ProcessErrorMessage(2);//"SetCommMask()"
        }
    }
    else
    {
        ProcessErrorMessage(3);//"SetCommTimeouts()"
    }

    // flush the port
    PurgeComm(m_hComm, PURGE_RXCLEAR | PURGE_TXCLEAR | PURGE_RXABORT | PURGE_TXABORT);

    // release critical section
    LeaveCriticalSection(&m_csCommunicationSync);

    TRACE(_T("Initialisation for communicationport completed.\nUse Startmonitor to communicate.\n"));

    return TRUE;
}
//
//  The CommThread Function.
//
UINT CSerialPort::CommThread(LPVOID pParam)
{
    // Cast the void pointer passed to the thread back to
    // a pointer of CSerialPort class
    CSerialPort *port = (CSerialPort*)pParam;

    // Set the status variable in the dialog class to
    // TRUE to indicate the thread is running.
    port->m_bThreadAlive = TRUE;

    // Misc. variables
    DWORD BytesTransfered = 0;
    DWORD Event = 0;
    DWORD CommEvent = 0;
    DWORD dwError = 0;
    static COMSTAT comstat;
    BOOL  bResult = TRUE;

    // Clear comm buffers at startup
    if (port->m_hComm)  // check if the port is opened
        PurgeComm(port->m_hComm, PURGE_RXCLEAR | PURGE_TXCLEAR | PURGE_RXABORT | PURGE_TXABORT);

    // begin forever loop.  This loop will run as long as the thread is alive.
    for (;;)
    {
        // Make a call to WaitCommEvent().  This call will return immediatly
        // because our port was created as an async port (FILE_FLAG_OVERLAPPED
        // and an m_OverlappedStructerlapped structure specified).  This call will cause the
        // m_OverlappedStructerlapped element m_OverlappedStruct.hEvent, which is part of the m_hEventArray to
        // be placed in a non-signeled state if there are no bytes available to be read,
        // or to a signeled state if there are bytes available.  If this event handle
        // is set to the non-signeled state, it will be set to signeled when a
        // character arrives at the port.

        // we do this for each port!
        bResult = WaitCommEvent(port->m_hComm, &Event, &port->m_ov);
        if (!bResult)
        {
            // If WaitCommEvent() returns FALSE, process the last error to determin
            // the reason..
            switch (dwError = GetLastError())
            {
            case ERROR_IO_PENDING:
                {
                    // This is a normal return value if there are no bytes
                    // to read at the port.
                    // Do nothing and continue
                    break;
                }
            case 87:
                {
                    // Under Windows NT, this value is returned for some reason.
                    // I have not investigated why, but it is also a valid reply
                    // Also do nothing and continue.
                    break;
                }
            default:
                {
                    // All other error codes indicate a serious error has
                    // occured.  Process this error.
                    port->ProcessErrorMessage(4);//"WaitCommEvent()"
                    break;
                }
            }
        }
        else
        {
            // If WaitCommEvent() returns TRUE, check to be sure there are
            // actually bytes in the buffer to read.
            //
            // If you are reading more than one byte at a time from the buffer
            // (which this program does not do) you will have the situation occur
            // where the first byte to arrive will cause the WaitForMultipleObjects()
            // function to stop waiting.  The WaitForMultipleObjects() function
            // resets the event handle in m_OverlappedStruct.hEvent to the non-signelead state
            // as it returns.
            //
            // If in the time between the reset of this event and the call to
            // ReadFile() more bytes arrive, the m_OverlappedStruct.hEvent handle will be set again
            // to the signeled state. When the call to ReadFile() occurs, it will
            // read all of the bytes from the buffer, and the program will
            // loop back around to WaitCommEvent().
            //
            // At this point you will be in the situation where m_OverlappedStruct.hEvent is set,
            // but there are no bytes available to read.  If you proceed and call
            // ReadFile(), it will return immediatly due to the async port setup, but
            // GetOverlappedResults() will not return until the next character arrives.
            //
            // It is not desirable for the GetOverlappedResults() function to be in
            // this state.  The thread shutdown event (event 0) and the WriteFile()
            // event (Event2) will not work if the thread is blocked by GetOverlappedResults().
            //
            // The solution to this is to check the buffer with a call to ClearCommError().
            // This call will reset the event handle, and if there are no bytes to read
            // we can loop back through WaitCommEvent() again, then proceed.
            // If there are really bytes to read, do nothing and proceed.

            bResult = ClearCommError(port->m_hComm, &dwError, &comstat);
            if (comstat.cbInQue == 0)
                continue;
        } // end if bResult

        // Main wait function.  This function will normally block the thread
        // until one of nine events occur that require action.
        Sleep(20);
        Event = WaitForMultipleObjects(3, port->m_hEventArray, FALSE, INFINITE);

        switch (Event)
        {
        case 0:
            {
                // Shutdown event.  This is event zero so it will be
                // the higest priority and be serviced first.
				SetCommMask(port->m_hComm, 0);
                CloseHandle(port->m_hComm);
                port->m_hComm=NULL;
                port->m_bThreadAlive = FALSE;
                // Kill this thread.  break is not needed, but makes me feel better.
				TRACE(_T("Exit CommThread \n"));
                AfxEndThread(100);
                break;
            }
        case 1: // read event
            {
                GetCommMask(port->m_hComm, &CommEvent);
                if (CommEvent & EV_RXCHAR)
                {
                    // Receive character event from port.
                    Sleep(10);
					TRACE(_T("*** Receive data ***\n"));
                    ReceivePort(port, comstat);
                }
                break;
            }
        case 2: // write event
            {
                // Write character event from port
                WriteData(port);
                break;
            }

        } // end switch

    } // close forever loop

    return 0;
}

//
// start comm watching
//
BOOL CSerialPort::StartMonitoring()
{
    if (!(m_Thread = AfxBeginThread(CommThread, this)))
    {
    	m_Thread=NULL;
        return FALSE;
    }
	TRACE(_T("Thread started\n"));
    return TRUE;
}

//
// Restart the comm thread
//
BOOL CSerialPort::RestartMonitoring()
{
    TRACE(_T("Thread resumed\n"));
    m_Thread->ResumeThread();
    return TRUE;
}


//
// Suspend the comm thread
//
BOOL CSerialPort::StopMonitoring()
{
    TRACE(_T("Thread suspended\n"));
    m_Thread->SuspendThread();
    return TRUE;
}


//
// If there is a error, give the right message
//
void CSerialPort::ProcessErrorMessage(WORD ErrorTag)
{
}

//
// Write a character.
//
void CSerialPort::WriteData(CSerialPort* port)
{
    BOOL bWrite = TRUE;
    BOOL bResult = TRUE;

    DWORD BytesSent = 0;

    ResetEvent(port->m_hWriteEvent);

    // Gain ownership of the critical section
    EnterCriticalSection(&port->m_csCommunicationSync);

    if (bWrite)
    {
        // Initailize variables
        port->m_ov.Offset = 0;
        port->m_ov.OffsetHigh = 0;

        // Clear buffer
        PurgeComm(port->m_hComm, PURGE_RXCLEAR | PURGE_TXCLEAR );

        bResult = WriteFile(port->m_hComm,       // Handle to COMM Port
                            port->m_szWriteBuffer,     // Pointer to message buffer in calling finction
                            port->m_nWriteSize, // Length of message to send
                            &BytesSent,        // Where to store the number of bytes sent
                            &port->m_ov);       // Overlapped structure
        /************************************************/
        /*If hFile was opened with FILE_FLAG_OVERLAPPED, */
        /*the lpOverlapped parameter must not be NULL. */
        /*It must point to a valid OVERLAPPED structure. */
        /*If hFile was opened with FILE_FLAG_OVERLAPPED */
        /*and lpOverlapped is NULL, the function can */
        /*incorrectly report that the write operation is */
        /*complete. If hFile was opened with FILE_FLAG_OVERLAPPED */
        /*and lpOverlapped is not NULL, the write operation starts */
        /*at the offset specified in the OVERLAPPED structure and */
        /*WriteFile may return before the write operation has been */
        /*completed. In this case, WriteFile returns FALSE and the */
        /*GetLastError function returns ERROR_IO_PENDING. This allows */
        /*the calling process to continue processing while the write */
        /*operation is being completed. The event specified in the */
        /*OVERLAPPED structure is set to the signaled state upon */
        /*completion of the write operation. If hFile was not opened */
        /*with FILE_FLAG_OVERLAPPED and lpOverlapped is NULL, the write */
        /*operation starts at the current file position and WriteFile*/
        /*does not return until the operation has been completed. */
        /*时非常重要，直接影响通讯的稳定性*/
        /*请参考MSDN，或者阅读上面说明*/
        /************************************************/
        Sleep (20);

        TRACE0("TX = ");
        for(WORD cont = 0; cont < port->m_nWriteSize ; cont++)
        {
            TRACE(_T(" %02X"),port->m_szWriteBuffer[cont]);
        }
        TRACE0("\n");

        // deal with any error codes
        if (!bResult)
        {
            DWORD dwError = GetLastError();
            switch (dwError)
            {
            case ERROR_IO_PENDING:
                {
                    // continue to GetOverlappedResults()
                    BytesSent = 0;
                    bWrite = FALSE;
                    break;
                }
            default:
                {
                    // all other error codes
                    port->ProcessErrorMessage(5);//"WriteFile()"
                }
            }
        }
        else
        {
            LeaveCriticalSection(&port->m_csCommunicationSync);
        }
    } // end if(bWrite)

    if (!bWrite)
    {
        bWrite = TRUE;

        bResult = GetOverlappedResult(port->m_hComm, // Handle to COMM port
                                      &port->m_ov,  // Overlapped structure
                                      &BytesSent,  // Stores number of bytes sent
                                      TRUE);    // Wait flag

        LeaveCriticalSection(&port->m_csCommunicationSync);

    } // end if (!bWrite)

    //Verify that the data size send equals what we tried to send
    if (BytesSent != port->m_nWriteSize) // Length of message to send)
    {
        TRACE(_T("WARNING: WriteFile() error.. Bytes Sent: %d; Message Length: %d\n"));
    }
}

//
// Character received. Inform the owner
//
void CSerialPort::ReceivePort(CSerialPort* port, COMSTAT comstat)
{
    BOOL  bRead = TRUE;
    BOOL  bResult = TRUE;
    DWORD dwError = 0;
    DWORD BytesRead = 0;
    BYTE inbuff[4*COMM_BUFFER_BASESIZE];
    DWORD nToRead;
	memset(inbuff, 0, 4*COMM_BUFFER_BASESIZE);
    for (;;)
    {
        // Gain ownership of the comm port critical section.
        // This process guarantees no other part of this program
        // is using the port object.

        EnterCriticalSection(&port->m_csCommunicationSync);

        // ClearCommError() will update the COMSTAT structure and
        // clear any other errors.

        bResult = ClearCommError(port->m_hComm, &dwError, &comstat);

        LeaveCriticalSection(&port->m_csCommunicationSync);

        // start forever loop.  I use this type of loop because I
        // do not know at runtime how many loops this will have to
        // run. My solution is to start a forever loop and to
        // break out of it when I have processed all of the
        // data available.  Be careful with this approach and
        // be sure your loop will exit.
        // My reasons for this are not as clear in this sample
        // as it is in my production code, but I have found this
        // solutiion to be the most efficient way to do this.

        if (comstat.cbInQue == 0)
        {
            // break out when all bytes have been read
            break;
        }

        EnterCriticalSection(&port->m_csCommunicationSync);

        if (bRead)
        {
            nToRead = min(comstat.cbInQue, 4*COMM_BUFFER_BASESIZE);
            bResult = ReadFile(port->m_hComm,  // Handle to COMM port
                               inbuff,    // RX Buffer Pointer
                               nToRead,     // Read one byte
                               &BytesRead,   // Stores number of bytes read
                               &port->m_ov);  // pointer to the m_ov structure
#if 1
            TRACE0("SerialPort RX = ");
            TRACE(_T(" %02X\n"),BytesRead);
            for(DWORD cont=0; cont < BytesRead; cont++)
            {
                TRACE(_T(" %02X"),inbuff[cont]);
            }
            TRACE0("\n");
#endif
            // deal with the error code
            if (!bResult)
            {
                switch (dwError = GetLastError())
                {
                case ERROR_IO_PENDING:
                    {
                        // asynchronous i/o is still in progress
                        // Proceed on to GetOverlappedResults();
                        bRead = FALSE;
                        break;
                    }
                default:
                    {
                        // Another error has occured.  Process this error.
                        port->ProcessErrorMessage(6);//"ReadFile()"
                        break;
                    }
                }
            }
            else
            {
                // ReadFile() returned complete. It is not necessary to call GetOverlappedResults()
                bRead = TRUE;
            }
        }  // close if (bRead)

        if (!bRead)
        {
            bRead = TRUE;
            bResult = GetOverlappedResult(port->m_hComm, // Handle to COMM port
                                          &port->m_ov,  // Overlapped structure
                                          &BytesRead,  // Stores number of bytes read
                                          TRUE);    // Wait flag

            // deal with the error code
            if (!bResult)
            {
                port->ProcessErrorMessage(7);//
            }
        }  // close if (!bRead)

        LeaveCriticalSection(&port->m_csCommunicationSync);

        // notify parent that a byte was received
        if(BytesRead)
        {
			theApp.commLayer.RecvData(inbuff,(WORD)BytesRead);//send data to theApp to analyse
        }
    } // end forever loop
}

//
// Write data to the port
//

WORD CSerialPort::WriteToPort(BYTE * string,DWORD  n)
{
    memset(m_szWriteBuffer, 0, sizeof(m_szWriteBuffer));
    // memset(m_szWriteBuffer, 0, n);
    // strncpy(m_szWriteBuffer, string, n);
    memcpy(m_szWriteBuffer, string, n);
    m_nWriteSize=n;
    // set event for write
    SetEvent(m_hWriteEvent);
    return WORD(m_nWriteSize);
}


//
// Return the device control block
//
DCB CSerialPort::GetDCB()
{
    return m_dcb;
}

//
// Return the communication event masks
//
DWORD CSerialPort::GetCommEvents()
{
    return m_dwCommEvents;
}

//
// Return the output buffer size
//
DWORD CSerialPort::GetWriteBufferSize()
{
    return m_nWriteBufferSize;
}


void CSerialPort::ClosePort()
{
    //TerminateThread(m_Thread->m_hThread, 0);
    //m_Thread = NULL;
    if (m_bThreadAlive)
    {
        SetEvent(m_hShutdownEvent);
    }
	Sleep(100);
}