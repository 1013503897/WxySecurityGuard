#include "pch.h"
#include "CSecurityGuard.h"
#include "CDlgFile.h"
#include "CDlgWindow.h"
#include "CDlgSecurityGuard.h"
#include "afxdialogex.h"
#include "CPEInfoDlg.h"
#include "CDlgServer.h"
#include "CDlgScanVirus.h"
#include "CDlgFileCleaner.h"
#include "CDlgMsConfig.h"
#include "CDlgUninstall.h"
#include "CDlgHookDetect.h"
#include "CDlgHackTools.h"
<<<<<<< HEAD

=======
#include "CDlgDriver.h"
#include "CDlgAPITrace.h"
>>>>>>> v1.0
#include "Helper.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

static UINT indicators[] =
{
	IDS_STRING165,
	IDS_STRING166,
	IDS_STRING167,
	IDS_STRING167,
};

CDlgSecurityGuard::CDlgSecurityGuard(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_CSecurityGuard_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CDlgSecurityGuard::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TAB_TYPE, m_cMainTable);
}

BEGIN_MESSAGE_MAP(CDlgSecurityGuard, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_SIZE()
	ON_WM_TIMER()
	ON_COMMAND(ID_32784, &CDlgSecurityGuard::OnShutDown)
	ON_COMMAND(ID_32785, &CDlgSecurityGuard::OnSleep)
	ON_COMMAND(ID_32786, &CDlgSecurityGuard::OnReset)
END_MESSAGE_MAP()

BOOL CDlgSecurityGuard::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	// _beginthread(ProcessProtect, NULL, NULL);
	SetIcon(m_hIcon, TRUE);			
	SetIcon(m_hIcon, FALSE);		
	WxyEnableSeDebugPrivilege();
	CRect rc;
	GetClientRect(&rc);
	rc.bottom -= 18;
	m_cMainTable.MoveWindow(rc);
	m_cMainTable.InsertItem(0, _T("进程信息"));
	m_cMainTable.InsertItem(1, _T("PE信息"));
	m_cMainTable.InsertItem(2, _T("文件信息"));
	m_cMainTable.InsertItem(3, _T("窗口信息"));
	m_cMainTable.InsertItem(4, _T("服务信息"));
	m_cMainTable.InsertItem(5, _T("文件清理"));
	m_cMainTable.InsertItem(6, _T("病毒扫描"));
	m_cMainTable.InsertItem(7, _T("启动项扫描"));
	m_cMainTable.InsertItem(8, _T("软件卸载"));
	m_cMainTable.InsertItem(9, _T("钩子扫描"));
<<<<<<< HEAD
	m_cMainTable.InsertItem(10, _T("其它工具"));
=======
	m_cMainTable.InsertItem(10, _T("驱动扫描"));
	m_cMainTable.InsertItem(11, _T("API监控"));
	m_cMainTable.InsertItem(12, _T("其它工具"));
>>>>>>> v1.0
	m_cMainTable.m_Dia.push_back(new CDlgProcess);
	m_cMainTable.m_Dia.push_back(new CPEInfoDlg);
	m_cMainTable.m_Dia.push_back(new CDlgFile);
	m_cMainTable.m_Dia.push_back(new CDlgWindow);
	m_cMainTable.m_Dia.push_back(new CDlgServer);
	m_cMainTable.m_Dia.push_back(new CDlgFileCleaner);
	m_cMainTable.m_Dia.push_back(new CDlgScanVirus);
	m_cMainTable.m_Dia.push_back(new CDlgMsConfig);
	m_cMainTable.m_Dia.push_back(new CDlgUninstall);
	m_cMainTable.m_Dia.push_back(new CDlgHookDetect);
<<<<<<< HEAD
=======
	m_cMainTable.m_Dia.push_back(new CDlgDriver);
	m_cMainTable.m_Dia.push_back(new CDlgAPITrace);
>>>>>>> v1.0
	m_cMainTable.m_Dia.push_back(new CDlgHackTools);
	m_cMainTable.m_Dia[0]->Create(IDD_DIALOG_PROCESS, &m_cMainTable);
	m_cMainTable.m_Dia[1]->Create(IDD_PEINFO_DIALOG, &m_cMainTable);
	m_cMainTable.m_Dia[2]->Create(IDD_DIALOG_FILE, &m_cMainTable);
	m_cMainTable.m_Dia[3]->Create(IDD_DIALOG_WINDOW, &m_cMainTable);
	m_cMainTable.m_Dia[4]->Create(IDD_DIALOG_SERVER, &m_cMainTable);
	m_cMainTable.m_Dia[5]->Create(IDD_DIALOG_CLEANER, &m_cMainTable);
	m_cMainTable.m_Dia[6]->Create(IDD_DIALOG_SCAN, &m_cMainTable);
	m_cMainTable.m_Dia[7]->Create(IDD_DIALOG_MSCONFIG, &m_cMainTable);
	m_cMainTable.m_Dia[8]->Create(IDD_DIALOG_UNINSTALL, &m_cMainTable);
	m_cMainTable.m_Dia[9]->Create(IDD_DIALOG_HOOKDETECT, &m_cMainTable);
<<<<<<< HEAD
	m_cMainTable.m_Dia[10]->Create(IDD_DIALOG_HACKTOOLS, &m_cMainTable);
=======
	m_cMainTable.m_Dia[10]->Create(IDD_DIALOG_DRIVER, &m_cMainTable);
	m_cMainTable.m_Dia[11]->Create(IDD_DIALOG_APITRACE, &m_cMainTable);
	m_cMainTable.m_Dia[12]->Create(IDD_DIALOG_HACKTOOLS, &m_cMainTable);
>>>>>>> v1.0
	m_cMainTable.m_Dia[0]->ShowWindow(SW_SHOW);
	InitStatusBar();
	SetTimer(100, 1000, NULL);
	CMenu menu;
	menu.LoadMenu(IDR_MENU_OPTION);
	SetMenu(&menu);
	return TRUE; 
}

BOOL CDlgSecurityGuard::InitStatusBar()
{
	m_cStatusBar.Create(this);
	int statusnum = sizeof(indicators) / sizeof(UINT);
	m_cStatusBar.SetIndicators((unsigned int*)indicators, statusnum);
	CRect rect;
	GetClientRect(&rect);
	int perstulen = rect.Width() / statusnum;
	for (int i = 0; i < statusnum; i++)
	{
		m_cStatusBar.SetPaneInfo(i, NULL, NULL, perstulen);
	}
	CTime time;
	time = CTime::GetCurrentTime();
	CString CpuUsage;
	CpuUsage.Format(_T("CPU占用率:20%%"));
	MEMORYSTATUSEX memStatus = { 0 };
	memStatus.dwLength = sizeof(MEMORYSTATUSEX);
	GlobalMemoryStatusEx(&memStatus);
	memStatus.dwMemoryLoad;
	CString MemoryUsage;
	MemoryUsage.Format(_T("内存占用率:%d%%"), memStatus.dwMemoryLoad);
	m_cStatusBar.SetPaneText(0, CpuUsage);
	m_cStatusBar.SetPaneText(1, MemoryUsage);	
	CString ProcessNum;
	DWORD tmp = 0;
	PROCESSENTRY32 ProcessEntry32 = { 0 };
	ProcessEntry32.dwSize = sizeof(PROCESSENTRY32);
	HANDLE Toolhelp32SnapshotHandle = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	BOOL IsOk = Process32First(Toolhelp32SnapshotHandle, &ProcessEntry32);

	while (Process32Next(Toolhelp32SnapshotHandle, &ProcessEntry32))
	{
		tmp++;
	}
	CloseHandle(Toolhelp32SnapshotHandle);
	ProcessNum.Format(_T("进程数:%d"), tmp);
	m_cStatusBar.SetPaneText(2, ProcessNum);
	m_cStatusBar.SetPaneText(3, time.Format("  %Y-%m-%d  %H:%M:%S"));
	RepositionBars(AFX_IDW_CONTROLBAR_FIRST, AFX_IDW_CONTROLBAR_LAST, ID_INDICATOR_REC);

	return TRUE;
}

void CDlgSecurityGuard::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); 
		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

HCURSOR CDlgSecurityGuard::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CDlgSecurityGuard::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);
	if (m_cMainTable.m_hWnd)
	{
		CRect rc;
		GetClientRect(&rc);
		int perstulen = rc.Width() / 4;
		rc.bottom -= 18;
		m_cMainTable.MoveWindow(rc);
		RepositionBars(AFX_IDW_CONTROLBAR_FIRST, AFX_IDW_CONTROLBAR_LAST, ID_INDICATOR_REC);
	}
}

void CDlgSecurityGuard::OnTimer(UINT_PTR nIDEvent)
{
	if (nIDEvent == 100)
	{
		CTime t1;
		t1 = CTime::GetCurrentTime();
		CString CpuUsage;
		CpuUsage.Format(_T("CPU占用率:%d%%"), 2*WxyGetCPUUsage(idleTime, kernelTime, userTime));
		m_cStatusBar.SetPaneText(0, CpuUsage);
		MEMORYSTATUSEX memStatus = { 0 };
		memStatus.dwLength = sizeof(MEMORYSTATUSEX);
		GlobalMemoryStatusEx(&memStatus);
		memStatus.dwMemoryLoad;
		CString MemoryUsage;
		MemoryUsage.Format(_T("内存占用率:%d%%"), memStatus.dwMemoryLoad);
		CString ProcessNum;
		DWORD tmp = 0;
		PROCESSENTRY32 ProcessEntry32 = { 0 };
		ProcessEntry32.dwSize = sizeof(PROCESSENTRY32);
		HANDLE Toolhelp32SnapshotHandle = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
		BOOL IsOk = Process32First(Toolhelp32SnapshotHandle, &ProcessEntry32);

		while (Process32Next(Toolhelp32SnapshotHandle, &ProcessEntry32))
		{
			tmp++;
		}
		CloseHandle(Toolhelp32SnapshotHandle);
		ProcessNum.Format(_T("进程数:%d"), tmp);
		m_cStatusBar.SetPaneText(1, MemoryUsage);
		m_cStatusBar.SetPaneText(2, ProcessNum);
		m_cStatusBar.SetPaneText(3, t1.Format("  %Y-%m-%d  %H:%M:%S"));
	}
	CDialogEx::OnTimer(nIDEvent);
}

BOOL CDlgSecurityGuard::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->message == WM_HOTKEY && pMsg->wParam == 0Xa001) {
		if (m_bIsWindowHide == TRUE)
		{
			ShowWindow(SW_HIDE);
			m_bIsWindowHide = FALSE;
		}
		else
		{
			ShowWindow(SW_SHOW);
			m_bIsWindowHide = TRUE;
		}
	}
	return CDialogEx::PreTranslateMessage(pMsg);
}


void CDlgSecurityGuard::OnShutDown()
{
	WinExec("shutdown -s -t 0", FALSE);
}


void CDlgSecurityGuard::OnSleep()
{
	WinExec("rundll32 powrprof.dll, SetSuspendState", FALSE);
}


void CDlgSecurityGuard::OnReset()
{
	WinExec("shutdown -r", FALSE);
}
