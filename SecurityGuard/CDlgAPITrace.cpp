#include "pch.h"
#include "CDlgAPITrace.h"
#include "afxdialogex.h"
#include <stdlib.h>  
#include <stddef.h>  
#include <string.h>  
#include "syelog.h"  
#include ".\Detours\detours.h"
#include ".\Detours\detver.h"

#pragma comment(lib,".\\Detours\\detours.lib")
#pragma comment(lib,"ws2_32.lib")  
#pragma comment(lib,"mswsock.lib")  
#pragma comment(lib,"advapi32.lib")  

#if (_MSC_VER < 1299)  
typedef ULONG* PULONG_PTR;
typedef ULONG ULONG_PTR;
typedef LONG* PLONG_PTR;
typedef LONG LONG_PTR;
#endif  

enum {
    CLIENT_AWAITING_PIPE_ACCEPT = 0x21,
    CLIENT_AWAITING_PIPE_DATA = 0x22,
};

typedef struct _CLIENT : OVERLAPPED
{
    HANDLE          hPipe;
    BOOL            fAwaitingAccept;
    PVOID           Zero;
    SYELOG_MESSAGE  Message;
} CLIENT, * PCLIENT;

BOOL        s_fLogToScreen = TRUE;     
BOOL        s_fExitAfterOne = FALSE;
BOOL        s_fDeltaTime = FALSE;
HANDLE      s_hOutFile = INVALID_HANDLE_VALUE;
LONG        s_nActiveClients = 0;
LONGLONG    s_llStartTime = 0;
LONGLONG    s_llLastTime = 0;
CListBox    *g_ListBox;

BOOL LogMessageV(BYTE nSeverity, PCHAR pszMsg, ...);

void Msg(TCHAR* p)
{
    g_ListBox->AddString(CString(p));
}
void Msg(CString msg)
{
    g_ListBox->AddString(msg);
}
VOID MyErrExit(PCSTR pszMsg)
{
    DWORD error = GetLastError();
    LogMessageV(SYELOG_SEVERITY_FATAL, "Error %d in %s.", error, pszMsg);
    fprintf(stderr, "SYELOGD: Error %d in %s.\n", error, pszMsg);
    fflush(stderr);
    exit(1);
}

static PCSTR FileTimeToString(PCHAR pszBuffer, DWORD cbBuffer, FILETIME ftTime)
{
    (void)cbBuffer;

    static BOOL bGotTzi = FALSE;
    static DWORD dwTzi = TIME_ZONE_ID_UNKNOWN;
    static TIME_ZONE_INFORMATION tzi;
    if (!bGotTzi) {
        dwTzi = GetTimeZoneInformation(&tzi);
        if (dwTzi == TIME_ZONE_ID_UNKNOWN) {
            ZeroMemory(&tzi, sizeof(tzi));
        }
        bGotTzi = TRUE;
    }
    SYSTEMTIME stUtc;
    SYSTEMTIME stLocal;

    pszBuffer[0] = '\0';

    if (s_fDeltaTime) {
        if (s_llLastTime == 0) {
            s_llLastTime = s_llStartTime;
        }

        ULARGE_INTEGER ul;
        ul.LowPart = ftTime.dwLowDateTime;
        ul.HighPart = ftTime.dwHighDateTime;

        ULONG64 delta = ul.QuadPart - s_llLastTime;
        s_llLastTime = ul.QuadPart;
        delta /= 10000;

#ifdef _CRT_INSECURE_DEPRECATE  
        sprintf_s(pszBuffer, cbBuffer, "%7I64d", delta);
#else  
        sprintf(pszBuffer, "%7I64d", delta);
#endif  
    }
    else {
        if (!FileTimeToSystemTime(&ftTime, &stUtc)) {
#ifdef _CRT_INSECURE_DEPRECATE  
            sprintf_s(pszBuffer, cbBuffer, "ft:%16I64d", *(LONGLONG*)&ftTime);
#else  
            sprintf(pszBuffer, "ft:%16I64d", *(LONGLONG*)&ftTime);
#endif  
            return pszBuffer;
        }
        else if (!SystemTimeToTzSpecificLocalTime(&tzi, &stUtc, &stLocal)) {
            CopyMemory(&stLocal, &stUtc, sizeof(stLocal));
        }

#ifdef _CRT_INSECURE_DEPRECATE  
        sprintf_s(pszBuffer, cbBuffer, "%4d%02d%02d%02d%02d%02d%03d",
            stLocal.wYear,
            stLocal.wMonth,
            stLocal.wDay,
            stLocal.wHour,
            stLocal.wMinute,
            stLocal.wSecond,
            stLocal.wMilliseconds);
#else  
        sprintf(pszBuffer, "%4d%02d%02d%02d%02d%02d%03d",
            stLocal.wYear,
            stLocal.wMonth,
            stLocal.wDay,
            stLocal.wHour,
            stLocal.wMinute,
            stLocal.wSecond,
            stLocal.wMilliseconds);
#endif  
    }
    return pszBuffer;
}

BOOL CloseConnection(PCLIENT pClient)
{
    LogMessageV(SYELOG_SEVERITY_INFORMATION, "Client closed pipe.");

    InterlockedDecrement(&s_nActiveClients);
    if (pClient != NULL) {
        if (pClient->hPipe != INVALID_HANDLE_VALUE) {
            FlushFileBuffers(pClient->hPipe);
            if (!DisconnectNamedPipe(pClient->hPipe)) {
                MyErrExit("DisconnectNamedPipe");
            }
            CloseHandle(pClient->hPipe);
            pClient->hPipe = INVALID_HANDLE_VALUE;
        }
        GlobalFree(pClient);
        pClient = NULL;
    }

    if (s_fExitAfterOne) {
        ExitProcess(0);
    }
    return TRUE;
}

PCLIENT CreatePipeConnection(HANDLE hCompletionPort)
{
    HANDLE hPipe = CreateNamedPipe(SYELOG_PIPE_NAME,         
        PIPE_ACCESS_INBOUND |      
        FILE_FLAG_OVERLAPPED,      
        PIPE_TYPE_MESSAGE |        
        PIPE_READMODE_MESSAGE |    
        PIPE_WAIT,                 
        PIPE_UNLIMITED_INSTANCES,  
        0,                         
        0,                         
        20000,                     
        NULL);                     
    if (hPipe == INVALID_HANDLE_VALUE) {
        MyErrExit("CreatePipe");
    }

    PCLIENT pClient = (PCLIENT)GlobalAlloc(GPTR, sizeof(CLIENT));
    if (pClient == NULL) {
        MyErrExit("GlobalAlloc pClient");
    }

    ZeroMemory(pClient, sizeof(*pClient));
    pClient->hPipe = hPipe;
    pClient->fAwaitingAccept = TRUE;

    if (!CreateIoCompletionPort(pClient->hPipe, hCompletionPort, (ULONG_PTR)pClient, 0)) {
        MyErrExit("CreateIoComplietionPort pClient");
    }

    if (!ConnectNamedPipe(hPipe, pClient)) {
        if (GetLastError() != ERROR_IO_PENDING &&
            GetLastError() != ERROR_PIPE_LISTENING) {
            MyErrExit("ConnectNamedPipe");
        }
    }
    else {
        LogMessageV(SYELOG_SEVERITY_INFORMATION,
            "ConnectNamedPipe accepted immediately.");
    }
    return pClient;
}

BOOL LogMessageV(BYTE nSeverity, PCHAR pszMsg, ...)
{
    FILETIME ftOccurance;
    CHAR szTime[64];
    GetSystemTimeAsFileTime(&ftOccurance);
    FileTimeToString(szTime, sizeof(szTime), ftOccurance);

    if (s_fLogToScreen) {
        CString strMsg;
        strMsg.Format(s_fDeltaTime
            ? _T("%-7.7s ---- --.%02x: ")
            : _T("%-17.17s ---- --.%02x: ")
            , szTime, nSeverity);
        va_list args;
        va_start(args, pszMsg);
        char* p = pszMsg;
        while (*p)
        {
            CString tmp;
            USES_CONVERSION;
            tmp.Format(_T("%s"), A2T(p));
            strMsg += tmp;
            p++;
        }
        va_end(args);
        strMsg += _T("\n");
        Msg(strMsg);
    }
    if (s_hOutFile != INVALID_HANDLE_VALUE) {
        DWORD cbWritten = 0;
        CHAR szBuf[4096];
        INT cbToWrite;

#ifdef _CRT_INSECURE_DEPRECATE  
        cbToWrite = _snprintf_s(szBuf, sizeof(szBuf), sizeof(szBuf) - 1,
            s_fDeltaTime
            ? "%-7.7s ---- --.%02x: "
            : "%-17.17s ---- --.%02x: "
            , szTime, nSeverity);
#else  
        cbToWrite = _snprintf(szBuf, sizeof(szBuf),
            s_fDeltaTime
            ? "%-7.7s ---- --.%02x: "
            : "%-17.17s ---- --.%02x: "
            , szTime, nSeverity);
#endif  

        va_list args;
        va_start(args, pszMsg);
#ifdef _CRT_INSECURE_DEPRECATE  
        cbToWrite += _vsnprintf_s(szBuf + cbToWrite,
            sizeof(szBuf) - cbToWrite,
            sizeof(szBuf) - cbToWrite - 1,
            pszMsg, args);
#else  
        cbToWrite += _vsnprintf(szBuf + cbToWrite, sizeof(szBuf) - cbToWrite, pszMsg, args);
#endif  
        va_end(args);

#ifdef _CRT_INSECURE_DEPRECATE  
        cbToWrite += _snprintf_s(szBuf + cbToWrite,
            sizeof(szBuf) - cbToWrite,
            sizeof(szBuf) - cbToWrite - 1,
            "\n");
#else  
        cbToWrite += _snprintf(szBuf + cbToWrite, sizeof(szBuf) - cbToWrite, "\n");
#endif  

        if (cbToWrite < 0) {
            szBuf[sizeof(szBuf) - 2] = '\n';
            szBuf[sizeof(szBuf) - 1] = '\0';
            cbToWrite = sizeof(szBuf);
        }
        WriteFile(s_hOutFile, szBuf, cbToWrite, &cbWritten, NULL);
    }
    return TRUE;
}

BOOL LogMessage(PSYELOG_MESSAGE pMessage, DWORD nBytes)
{
    if (nBytes > pMessage->nBytes) {
        nBytes = pMessage->nBytes;
    }
    if (nBytes >= sizeof(*pMessage)) {
        nBytes = sizeof(*pMessage) - 1;
    }

    if (nBytes <= offsetof(SYELOG_MESSAGE, szMessage)) {
        return FALSE;
    }

    CHAR szTime[64];
    FileTimeToString(szTime, sizeof(szTime), pMessage->ftOccurance);

    PCHAR pszMsg = pMessage->szMessage;
    while (*pszMsg) {
        pszMsg++;
    }
    while (pszMsg > pMessage->szMessage&& isspace(pszMsg[-1])) {
        *--pszMsg = '\0';
    }
    USES_CONVERSION;
    if (s_fLogToScreen) {
        CString strMsg;
        strMsg.Format(s_fDeltaTime
            ? _T("%-7.7s %4d %02x.%02x: %s\n")
            : _T("%-17.17s %4d %02x.%02x: %s\n"),
            A2T(szTime),
            pMessage->nProcessId,
            pMessage->nFacility,
            pMessage->nSeverity,
            A2T(pMessage->szMessage));
        Msg(strMsg);
    }
    if (s_hOutFile != INVALID_HANDLE_VALUE) {
        DWORD cbWritten = 0;
        CHAR szBuf[4096];
        INT cbToWrite;

#ifdef _CRT_INSECURE_DEPRECATE  
        cbToWrite = _snprintf_s(szBuf, sizeof(szBuf), sizeof(szBuf) - 1,
            s_fDeltaTime
            ? "%-7.7s %4d %02x.%02x: %s\n"
            : "%-17.17s %4d %02x.%02x: %s\n",
            szTime,
            pMessage->nProcessId,
            pMessage->nFacility,
            pMessage->nSeverity,
            pMessage->szMessage);
#else  
        cbToWrite = _snprintf(szBuf, sizeof(szBuf),
            s_fDeltaTime
            ? "%-7.7s %4d %02x.%02x: %s\n"
            : "%-17.17s %4d %02x.%02x: %s\n",
            szTime,
            pMessage->nProcessId,
            pMessage->nFacility,
            pMessage->nSeverity,
            pMessage->szMessage);
#endif  

        if (cbToWrite < 0) {
            szBuf[sizeof(szBuf) - 2] = '\n';
            szBuf[sizeof(szBuf) - 1] = '\0';
            cbToWrite = sizeof(szBuf);
        }
        WriteFile(s_hOutFile, szBuf, cbToWrite, &cbWritten, NULL);
    }
    return TRUE;
}

DWORD WINAPI WorkerThread(LPVOID pvVoid)
{
    PCLIENT pClient;
    BOOL b;
    LPOVERLAPPED lpo;
    DWORD nBytes;
    HANDLE hCompletionPort = (HANDLE)pvVoid;

    for (BOOL fKeepLooping = TRUE; fKeepLooping;) {
        pClient = NULL;
        lpo = NULL;
        nBytes = 0;
        b = GetQueuedCompletionStatus(hCompletionPort,
            &nBytes, (PULONG_PTR)&pClient, &lpo, INFINITE);

        if (!b || lpo == NULL) {
            fKeepLooping = FALSE;
            MyErrExit("GetQueuedCompletionState");
            break;
        }
        else if (!b) {
            if (pClient) {
                if (GetLastError() == ERROR_BROKEN_PIPE) {
                    LogMessageV(SYELOG_SEVERITY_INFORMATION, "Client closed pipe.");
                }
                else {
                    LogMessageV(SYELOG_SEVERITY_ERROR,
                        "GetQueuedCompletionStatus failed %d [%p]",
                        GetLastError(), pClient);
                }
                CloseConnection(pClient);
            }
            continue;
        }

        if (pClient->fAwaitingAccept) {
            InterlockedIncrement(&s_nActiveClients);
            pClient->fAwaitingAccept = FALSE;
            b = ReadFile(pClient->hPipe,
                &pClient->Message,
                sizeof(pClient->Message),
                &nBytes,
                pClient);
            if (!b) {
                if (GetLastError() != ERROR_IO_PENDING) {
                    LogMessageV(SYELOG_SEVERITY_ERROR,
                        "ReadFile failed %d.", GetLastError());
                    continue;
                }
            }

            CreatePipeConnection(hCompletionPort);
        }
        else {
            if (nBytes < offsetof(SYELOG_MESSAGE, szMessage)) {
                CloseConnection(pClient);
            }

            if (pClient->Message.fTerminate) {
                LogMessageV(SYELOG_SEVERITY_NOTICE,
                    "Client requested terminate on next connection close.");
                s_fExitAfterOne = TRUE;
            }

            LogMessage(&pClient->Message, nBytes);

            b = ReadFile(pClient->hPipe,
                &pClient->Message,
                sizeof(pClient->Message),
                &nBytes,
                pClient);
            if (!b && GetLastError() == ERROR_BROKEN_PIPE) {
                CloseConnection(pClient);
            }
        }
    }
    return 0;
}

BOOL CreateWorkers(HANDLE hCompletionPort)
{
    DWORD dwThread;
    HANDLE hThread;
    DWORD i;
    SYSTEM_INFO SystemInfo;

    GetSystemInfo(&SystemInfo);

    for (i = 0; i < 2 * SystemInfo.dwNumberOfProcessors; i++) {
        hThread = CreateThread(NULL, 0, WorkerThread, hCompletionPort, 0, &dwThread);
        if (!hThread) {
            MyErrExit("CreateThread WorkerThread");
        }
        CloseHandle(hThread);
    }
    return TRUE;
}

//////////////////////////////////////////////////////////////////////////////  
//  
BOOL WINAPI ControlHandler(DWORD dwCtrlType)
{
    switch (dwCtrlType) {
    case CTRL_C_EVENT:
    case CTRL_BREAK_EVENT:
    case CTRL_CLOSE_EVENT:
    case CTRL_LOGOFF_EVENT:
    case CTRL_SHUTDOWN_EVENT:
        LogMessageV(SYELOG_SEVERITY_INFORMATION, "User requested stop.");
        if (s_hOutFile != INVALID_HANDLE_VALUE) {
            FlushFileBuffers(s_hOutFile);
            CloseHandle(s_hOutFile);
            s_hOutFile = INVALID_HANDLE_VALUE;
        }
        ExitProcess(0);
    }
    return FALSE;
}
IMPLEMENT_DYNAMIC(CDlgAPITrace, CDialogEx)

CDlgAPITrace::CDlgAPITrace(CWnd* pParent)
	: CDialogEx(IDD_DIALOG_APITRACE, pParent)
{

}

CDlgAPITrace::~CDlgAPITrace()
{
}

void CDlgAPITrace::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_LIST2, m_cListBoxMsg);
    DDX_Control(pDX, IDC_EDIT1, m_cEditFileFullPath);
}


BEGIN_MESSAGE_MAP(CDlgAPITrace, CDialogEx)
	ON_WM_SIZE()
    ON_WM_DROPFILES()
    ON_BN_CLICKED(IDC_BUTTON_TRACE, &CDlgAPITrace::OnBnClickedButtonTrace)
END_MESSAGE_MAP()

void CDlgAPITrace::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

    if (m_cListBoxMsg)
    {
        CRect rc;
        GetClientRect(rc);
        rc.right -= 100;
        rc.left += 90;
        rc.bottom = rc.top + 25;
        m_cEditFileFullPath.MoveWindow(rc);
        GetClientRect(rc);
        rc.left = rc.right - 90;
        rc.bottom = rc.top + 25;
        GetDlgItem(IDC_BUTTON_TRACE)->MoveWindow(rc);
        GetClientRect(rc);
        rc.top += 30;
        m_cListBoxMsg.MoveWindow(rc);
    }
    
}


BOOL CDlgAPITrace::OnInitDialog()
{
	CDialogEx::OnInitDialog();
    m_cEditFileFullPath.SetWindowText(_T("ConsoleApplication2.exe"));
    g_ListBox = &m_cListBoxMsg;
	return TRUE;  
}



void CDlgAPITrace::OnDropFiles(HDROP hDropInfo)
{
    TCHAR m_strFilePath[MAX_PATH];
    DragQueryFile(hDropInfo, 0, m_strFilePath, sizeof(m_strFilePath));
    m_cEditFileFullPath.SetWindowText(m_strFilePath);
    DragFinish(hDropInfo);
    CDialogEx::OnDropFiles(hDropInfo);
}


void CDlgAPITrace::OnBnClickedButtonTrace()
{
    HANDLE hCompletionPort;
    BOOL fNeedHelp = FALSE;
    STARTUPINFO si;
    PROCESS_INFORMATION pi;
    LPVOID lpAppName;
    LPSTR lpDllName = "trcapi32.dll";
    GetSystemTimeAsFileTime((FILETIME*)&s_llStartTime);
    hCompletionPort = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, NULL, 0);
    if (hCompletionPort == NULL) {
        MyErrExit("CreateIoCompletionPort");
    }

    CreateWorkers(hCompletionPort);
    CreatePipeConnection(hCompletionPort);
    ZeroMemory(&si, sizeof(STARTUPINFO));
    ZeroMemory(&pi, sizeof(PROCESS_INFORMATION));
    si.cb = sizeof(STARTUPINFO);
    CString FileFullPath;
    m_cEditFileFullPath.GetWindowText(FileFullPath);
    if (!DetourCreateProcessWithDll(FileFullPath,
        NULL, NULL, NULL, FALSE,
        CREATE_DEFAULT_ERROR_MODE, NULL, NULL,
        &si, &pi, lpDllName, NULL)) {
        return;
    }

    if (s_hOutFile != INVALID_HANDLE_VALUE) {
        FlushFileBuffers(s_hOutFile);
        CloseHandle(s_hOutFile);
        s_hOutFile = INVALID_HANDLE_VALUE;
    }
}
