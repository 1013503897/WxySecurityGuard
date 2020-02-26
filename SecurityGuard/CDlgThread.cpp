// CDlgThread.cpp: 实现文件
//

#include "pch.h"
#include "CSecurityGuard.h"
#include "CDlgThread.h"
#include "afxdialogex.h"

IMPLEMENT_DYNAMIC(CDlgThread, CDialogEx)

CDlgThread::CDlgThread(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_THREAD, pParent)
{

}

CDlgThread::~CDlgThread()
{
}

void CDlgThread::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_THREAD, m_cListThread);
}


BEGIN_MESSAGE_MAP(CDlgThread, CDialogEx)
	ON_WM_SIZE()
	ON_NOTIFY(NM_RCLICK, IDC_LIST_THREAD, &CDlgThread::OnNMRClickListThread)
	ON_COMMAND(ID__32773, &CDlgThread::OnSuspendThread)
	ON_COMMAND(ID__32774, &CDlgThread::OnTerminateThread)
	ON_COMMAND(ID__32775, &CDlgThread::OnResumeThread)
END_MESSAGE_MAP()

void CDlgThread::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);
	if (m_cListThread.m_hWnd)
	{
		CRect rc;
		this->GetClientRect(rc);
		m_cListThread.MoveWindow(rc);
	}
}


BOOL CDlgThread::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	m_cListThread.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_SUBITEMIMAGES);
	m_cListThread.InsertColumn(0, _T("线程ID"), LVCFMT_LEFT, 80);
	m_cListThread.InsertColumn(1, _T("线程创建时间"), LVCFMT_LEFT, 160);
	m_cListThread.InsertColumn(2, _T("线程回调地址"), LVCFMT_LEFT, 160);
	m_cListThread.InsertColumn(3, _T("线程优先级"), LVCFMT_LEFT, 150);
	return TRUE;  
}

VOID CDlgThread::EnumThread(DWORD ProcessID)
{
	WxyEnumThread(ProcessID, m_vThreadInfo);
	ShowThreadInfo();
}

VOID CDlgThread::ShowThreadInfo()
{
	m_cListThread.DeleteAllItems();
	for (size_t i = 0; i < m_vThreadInfo.size(); i++)
	{
		m_cListThread.InsertItem(i, _T(""));
		CString ThreadID;
		CString ThreadCreateTime;
		CString ThreadStartAddress;
		CString ThreadPriorty;
		ThreadID.Format(_T("%d"), m_vThreadInfo[i]->ThreadID);
		if (m_vThreadInfo[i]->CreateTime.wHour + 8 > 24)
		{
			m_vThreadInfo[i]->CreateTime.wHour -= 16;
			m_vThreadInfo[i]->CreateTime.wDay++;
		}
		else
		{
			m_vThreadInfo[i]->CreateTime.wHour += 8;
		}
		ThreadCreateTime.Format(_T("%u-%u-%u %02u:%02u:%02u"), m_vThreadInfo[i]->CreateTime.wYear, m_vThreadInfo[i]->\
			CreateTime.wMonth, m_vThreadInfo[i]->CreateTime.wDay, m_vThreadInfo[i]->\
			CreateTime.wHour, m_vThreadInfo[i]->CreateTime.wMinute, m_vThreadInfo[i]->CreateTime.wSecond);
		switch (m_vThreadInfo[i]->ThreadPriorty)
		{
		case 0:
			ThreadPriorty = _T("PRIORITY_NORMAL");
			break;
		case 1:
			ThreadPriorty = _T("PRIORITY_ABOVE_NORMAL");
			break;
		case 2:
			ThreadPriorty = _T("PRIORITY_HIGHEST");
			break;
		case -1:
			ThreadPriorty = _T("PRIORITY_BELOW_NORMAL");
			break;
		case -2:
			ThreadPriorty = _T("PRIORITY_LOWEST");
			break;
		case 8:
			ThreadPriorty = _T("PRIORITY_HIGHEST");
			break;
		case 4:
			ThreadPriorty = _T("PRIORITY_LOWEST");
			break;
		case 15:
			ThreadPriorty = _T("PRIORITY_TIME_CRITICAL");
			break;
		case 14:
			ThreadPriorty = _T("PRIORITY_ABOVE_NORMAL");
			break;
		default:
			ThreadPriorty = _T("PRIORITY_UNKNOW");
		}
#ifndef _WIN64
		ThreadStartAddress.Format(_T("0x%08X"), m_vThreadInfo[i]->ThreadStartAddress);
#else
		ThreadStartAddress.Format(_T("0x%016X"), m_ThreadInfo[i]->ThreadStartAddress);
#endif
		m_cListThread.SetItemText(i, 0, ThreadID);
		m_cListThread.SetItemText(i, 1, ThreadCreateTime);
		m_cListThread.SetItemText(i, 2, ThreadStartAddress);
		m_cListThread.SetItemText(i, 3, ThreadPriorty);
	}
}

void CDlgThread::OnNMRClickListThread(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	if (pNMListView->iItem != -1)
	{
		DWORD dwPos = GetMessagePos();
		CPoint point(LOWORD(dwPos), HIWORD(dwPos));
		CMenu menu;
		VERIFY(menu.LoadMenu(IDR_MENU_THREAD));
		CMenu* popup = menu.GetSubMenu(0);
		ASSERT(popup != NULL);
		popup->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, point.x, point.y, this);
	}
	*pResult = 0;
}
void CDlgThread::OnTerminateThread()
{
	POSITION pos = m_cListThread.GetFirstSelectedItemPosition();
	if (pos != 0)
	{
		HANDLE ThreadHandle = OpenThread(THREAD_TERMINATE, NULL, m_vThreadInfo[(int)pos - 1]->ThreadID);
		if (ThreadHandle)
		{
			TerminateThread(ThreadHandle, -1);
			CloseHandle(ThreadHandle);
		}
	}
}
void CDlgThread::OnSuspendThread()
{
	POSITION pos = m_cListThread.GetFirstSelectedItemPosition();
	if (pos != 0)
	{
		HANDLE ThreadHandle = OpenThread(THREAD_TERMINATE, NULL, m_vThreadInfo[(int)pos - 1]->ThreadID);
		if (ThreadHandle)
		{
			SuspendThread(ThreadHandle);
			CloseHandle(ThreadHandle);
		}
	}
}
void CDlgThread::OnResumeThread()
{
	POSITION pos = m_cListThread.GetFirstSelectedItemPosition();
	if (pos != 0)
	{
		HANDLE ThreadHandle = OpenThread(THREAD_TERMINATE, NULL, m_vThreadInfo[(int)pos - 1]->ThreadID);
		if (ThreadHandle)
		{
			ResumeThread(ThreadHandle);
			CloseHandle(ThreadHandle);
		}
	}
}