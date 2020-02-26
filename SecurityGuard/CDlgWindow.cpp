#include "pch.h"
#include "CSecurityGuard.h"
#include "CDlgWindow.h"
#include "afxdialogex.h"
#include "Helper.h"

IMPLEMENT_DYNAMIC(CDlgWindow, CDialogEx)

CDlgWindow::CDlgWindow(CWnd* pParent)
	: CDialogEx(IDD_DIALOG_WINDOW, pParent)
{
}

CDlgWindow::~CDlgWindow()
{
}

void CDlgWindow::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_WndInfo);
}


BEGIN_MESSAGE_MAP(CDlgWindow, CDialogEx)
	ON_WM_SIZE()
	ON_NOTIFY(NM_RCLICK, IDC_LIST1, &CDlgWindow::OnNMRClickList1)
	ON_COMMAND(ID_32787, &CDlgWindow::OnCloseWindow)
	ON_WM_SHOWWINDOW()
END_MESSAGE_MAP()



BOOL CDlgWindow::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	m_WndInfo.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_SUBITEMIMAGES);
	m_WndInfo.InsertColumn(0, _T("窗口名称"), LVCFMT_LEFT, 300); 
	m_WndInfo.InsertColumn(1, _T("窗口类名称"), LVCFMT_LEFT, 300);
	m_WndInfo.InsertColumn(2, _T("进程ID"), LVCFMT_LEFT, 100);
	m_WndInfo.InsertColumn(3, _T("窗口属性"), LVCFMT_LEFT, 100);
	
	return TRUE;
}

void CDlgWindow::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);
	CRect rc;
	this->GetClientRect(rc);
	if (m_WndInfo.m_hWnd)
	{
		m_WndInfo.MoveWindow(rc);
	}
}

void CDlgWindow::OnNMRClickList1(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);

	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	if (pNMListView->iItem != -1)
	{
		DWORD dwPos = GetMessagePos();
		CPoint point(LOWORD(dwPos), HIWORD(dwPos));
		CMenu menu;
		VERIFY(menu.LoadMenu(IDR_MENU_WINDOW));
		CMenu* popup = menu.GetSubMenu(0);
		ASSERT(popup != NULL);
		popup->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, point.x, point.y, this);
	}
	*pResult = 0;
}

void CDlgWindow::OnCloseWindow()
{
	POSITION pos = m_WndInfo.GetFirstSelectedItemPosition();
	int nItem = m_WndInfo.GetNextSelectedItem(pos);
	if (nItem != 0)
	{
		DWORD ProcessID;
		HWND h = ::FindWindow(NULL, m_WndInfoVec[nItem]->WndName);
		GetWindowThreadProcessId((HWND)h, &ProcessID);
		HANDLE ProcessHandle = OpenProcess(PROCESS_TERMINATE, NULL, ProcessID);
		if (ProcessHandle)
		{
			TerminateProcess(ProcessHandle, -1);
			m_WndInfo.DeleteItem(nItem);
			CloseHandle(ProcessHandle);
		}
	}
}


void CDlgWindow::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialogEx::OnShowWindow(bShow, nStatus);

	if (bShow)
	{
		for (auto it : m_WndInfoVec)
		{
			delete it;
		}
		m_WndInfoVec.clear();
		EnumWindows(&WxyEnumWndProc, (LPARAM)&m_WndInfoVec);
		m_WndInfo.DeleteAllItems();
		for (size_t i = 0; i < m_WndInfoVec.size(); i++)
		{
			m_WndInfo.InsertItem(i, _T(""));
			m_WndInfo.SetItemText(i, 0, m_WndInfoVec[i]->WndName);
			m_WndInfo.SetItemText(i, 1, m_WndInfoVec[i]->ClassName);
			m_WndInfo.SetItemText(i, 2, Int2CString(m_WndInfoVec[i]->ProcessID));
			m_WndInfo.SetItemText(i, 3, m_WndInfoVec[i]->IsVisable ? _T("可视") : _T("隐藏"));
		}
	}
}
