#include "pch.h"
#include "CSecurityGuard.h"
#include "CDlgUninstall.h"
#include "afxdialogex.h"


IMPLEMENT_DYNAMIC(CDlgUninstall, CDialogEx)


CDlgUninstall::CDlgUninstall(CWnd* pParent)
	: CDialogEx(IDD_DIALOG_UNINSTALL, pParent)
{

}

CDlgUninstall::~CDlgUninstall()
{
}

void CDlgUninstall::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_cListCtrl);
}


BEGIN_MESSAGE_MAP(CDlgUninstall, CDialogEx)
	ON_WM_SIZE()
	ON_NOTIFY(NM_DBLCLK, IDC_LIST1, &CDlgUninstall::OnNMDblclkListUninstall)
	ON_WM_SHOWWINDOW()
END_MESSAGE_MAP()

BOOL CDlgUninstall::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	m_cListCtrl.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	m_cListCtrl.InsertColumn(0, _T("软件名称"), LVCFMT_LEFT, 200);
	m_cListCtrl.InsertColumn(1, _T("公司名称"), LVCFMT_LEFT, 200);
	m_cListCtrl.InsertColumn(2, _T("卸载命令"), LVCFMT_LEFT, 400);

	return TRUE;  
}

void CDlgUninstall::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	if (m_cListCtrl.m_hWnd)
	{
		CRect rc;
		GetClientRect(rc);
		m_cListCtrl.MoveWindow(rc);
	}
}

void CDlgUninstall::OnNMDblclkListUninstall(NMHDR* pNMHDR, LRESULT* pResult)
{
	POSITION ps;
	int nIndex;
	ps = m_cListCtrl.GetFirstSelectedItemPosition();
	nIndex = m_cListCtrl.GetNextSelectedItem(ps);
	CString command = m_cListCtrl.GetItemText(nIndex, 2);
	USES_CONVERSION;
	char command2[255] = { 0 }; 
	strcpy(command2, T2A(command));
	WinExec(T2A(command), FALSE);
	*pResult = 0;
}


void CDlgUninstall::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialogEx::OnShowWindow(bShow, nStatus);

	WxyEnumUninstall(m_vSoftInfo);
	m_cListCtrl.DeleteAllItems();
	for (int i = 0; i < m_vSoftInfo.size(); i++)
	{
		m_cListCtrl.InsertItem(i, m_vSoftInfo[i].szSoftName);
		m_cListCtrl.SetItemText(i, 1, m_vSoftInfo[i].strSoftVenRe);
		m_cListCtrl.SetItemText(i, 2, m_vSoftInfo[i].strSoftUniPath);
	}
}
