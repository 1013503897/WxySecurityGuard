#include "pch.h"
#include "CSecurityGuard.h"
#include "CDlgServer.h"
#include "afxdialogex.h"
IMPLEMENT_DYNAMIC(CDlgServer, CDialogEx)

CDlgServer::CDlgServer(CWnd* pParent)
	: CDialogEx(IDD_DIALOG_SERVER, pParent)
{

}

CDlgServer::~CDlgServer()
{
}

void CDlgServer::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_cListCtrl);
}

BEGIN_MESSAGE_MAP(CDlgServer, CDialogEx)
	ON_WM_SIZE()
	ON_NOTIFY(NM_RCLICK, IDC_LIST1, &CDlgServer::OnListRClick)
	ON_COMMAND(ID_SERVER_START, &CDlgServer::OnStratServer)
	ON_COMMAND(ID_SERVER_CLOSE, &CDlgServer::OnCloseServer)
	ON_WM_SHOWWINDOW()
END_MESSAGE_MAP()

void CDlgServer::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);
	CRect rc;
	this->GetClientRect(rc);
	if (m_cListCtrl.m_hWnd)
	{
		m_cListCtrl.MoveWindow(rc);
	}
}

BOOL CDlgServer::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	m_cListCtrl.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_SUBITEMIMAGES);
	m_cListCtrl.InsertColumn(0, _T("名称"), LVCFMT_LEFT, 160);
	m_cListCtrl.InsertColumn(1, _T("显示名称"), LVCFMT_LEFT, 200);
	m_cListCtrl.InsertColumn(2, _T("状态"), LVCFMT_LEFT, 90);
	m_cListCtrl.InsertColumn(3, _T("描述"), LVCFMT_LEFT, 380);
	m_cListCtrl.InsertColumn(4, _T("启动类型"), LVCFMT_LEFT, 60);
	m_cListCtrl.InsertColumn(5, _T("启动参数"), LVCFMT_LEFT, 300);
	
	return TRUE;    
}

void CDlgServer::OnListRClick(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	if (pNMListView->iItem != -1)
	{
		DWORD dwPos = GetMessagePos();
		CPoint point(LOWORD(dwPos), HIWORD(dwPos));
		CMenu menu;
		VERIFY(menu.LoadMenu(IDR_MENU_SERVER));			
		CMenu* popup = menu.GetSubMenu(0);
		ASSERT(popup != NULL);
		popup->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, point.x, point.y, this);
	}
	*pResult = 0;
}

void CDlgServer::OnStratServer()
{
	POSITION pos = m_cListCtrl.GetFirstSelectedItemPosition();
	int nItem = m_cListCtrl.GetNextSelectedItem(pos);
	m_cListCtrl.SetItemText(nItem, 2, _T("正在运行"));
    BOOL ret;
    CString ErrorCode;
    LPCWSTR lpMachineName = NULL;
    LPCWSTR lpDatabaseName = NULL;
    DWORD dwSCManagerDesiredAccess = SC_MANAGER_ALL_ACCESS;
    SC_HANDLE scHandle = OpenSCManager(lpMachineName, lpDatabaseName, dwSCManagerDesiredAccess);
    if (scHandle == NULL) 
    {
        ErrorCode.Format(_T("OpenSCManager -> fail(%ld)"),GetLastError());
        MessageBox(ErrorCode);
    }
    else
    {
        CString lpServiceName = m_cListCtrl.GetItemText(nItem, 0);
        SC_HANDLE serviceHandle = OpenService(scHandle, lpServiceName, SERVICE_START);
        if (serviceHandle == NULL) 
        {
            ErrorCode.Format(_T("OpenService -> fail(%ld)"), GetLastError());
            MessageBox(ErrorCode);
        }
        else 
        {
            ret = StartService(serviceHandle, NULL, NULL);
            if (!ret) {
                ErrorCode.Format(_T("StartService  -> fail(%ld)\n",  GetLastError()));
                MessageBox(ErrorCode);
            }
        }
        CloseServiceHandle(scHandle);
        DeleteService(serviceHandle);
    }
}

void CDlgServer::OnCloseServer()
{
	POSITION pos = m_cListCtrl.GetFirstSelectedItemPosition();
	int nItem = m_cListCtrl.GetNextSelectedItem(pos); 
	m_cListCtrl.SetItemText(nItem, 2, _T("已停止"));
	BOOL ret;
	CString ErrorCode;
	LPCWSTR lpMachineName = NULL;
	LPCWSTR lpDatabaseName = NULL;
	DWORD dwSCManagerDesiredAccess = SC_MANAGER_ALL_ACCESS;
	SC_HANDLE scHandle = OpenSCManager(lpMachineName, lpDatabaseName, dwSCManagerDesiredAccess);
	if (scHandle == NULL)
	{
		ErrorCode.Format(_T("OpenSCManager -> fail(%ld)"), GetLastError());
		MessageBox(ErrorCode);
	}
	else
	{
		CString lpServiceName = m_cListCtrl.GetItemText(nItem, 0);
		SC_HANDLE serviceHandle = OpenService(scHandle, lpServiceName, SERVICE_STOP);
		if (serviceHandle == NULL)
		{
			ErrorCode.Format(_T("OpenService -> fail(%ld)"),  GetLastError());
			MessageBox(ErrorCode);
		}
		else
		{
			DWORD dwControl = SERVICE_CONTROL_STOP;
			SERVICE_STATUS serviceStatus = { 0 };
			ret = ControlService(serviceHandle, dwControl, &serviceStatus);
			if (!ret)
			{
				ErrorCode.Format(_T("ControlService -> fail(%ld)"), GetLastError());
				MessageBox(ErrorCode);
			}
			CloseServiceHandle(serviceHandle);
			DeleteService(serviceHandle);
		}
	}
}

void CDlgServer::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialogEx::OnShowWindow(bShow, nStatus);
	if (bShow)
	{
		WxyEnumServers(m_ServerInfoVec);
		m_cListCtrl.DeleteAllItems();
		for (size_t i = 0; i < m_ServerInfoVec.size(); i++)
		{
			m_cListCtrl.InsertItem(i, _T(""));
			m_cListCtrl.SetItemText(i, 0, m_ServerInfoVec[i]->ServerName);
			m_cListCtrl.SetItemText(i, 1, m_ServerInfoVec[i]->DisPlayName);
			if (m_ServerInfoVec[i]->IsRunning)
			{
				m_cListCtrl.SetItemText(i, 2, _T("正在运行"));
			}
			else
			{
				m_cListCtrl.SetItemText(i, 2, _T("已停止"));
			}
			m_cListCtrl.SetItemText(i, 3, m_ServerInfoVec[i]->Description);
			if (m_ServerInfoVec[i]->StartType == 3)
			{
				m_cListCtrl.SetItemText(i, 4, _T("手动"));
			}
			else if (m_ServerInfoVec[i]->StartType == 4)
			{
				m_cListCtrl.SetItemText(i, 4, _T("禁用"));
			}
			else
			{
				m_cListCtrl.SetItemText(i, 4, _T("自动"));
			}
			m_cListCtrl.SetItemText(i, 5, m_ServerInfoVec[i]->BinaryPsth);
		}
	}
}
