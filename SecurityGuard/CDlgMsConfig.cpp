// CDlgMsConfig.cpp: 实现文件
//

#include "pch.h"
#include "CSecurityGuard.h"
#include "CDlgMsConfig.h"
#include "afxdialogex.h"


// CDlgMsConfig 对话框

IMPLEMENT_DYNAMIC(CDlgMsConfig, CDialogEx)

CDlgMsConfig::CDlgMsConfig(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_MSCONFIG, pParent)
{

}

CDlgMsConfig::~CDlgMsConfig()
{
}

void CDlgMsConfig::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_cListCtrl);
}


BEGIN_MESSAGE_MAP(CDlgMsConfig, CDialogEx)
	ON_WM_SIZE()
END_MESSAGE_MAP()


void CDlgMsConfig::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);
	if (m_cListCtrl.m_hWnd)
	{
		CRect rc;
		GetClientRect(rc);
		m_cListCtrl.MoveWindow(rc);
	}
}


BOOL CDlgMsConfig::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	m_cListCtrl.InsertColumn(0, _T("启动项"), LVCFMT_LEFT, 120);
	m_cListCtrl.InsertColumn(1, _T("注册表"), LVCFMT_LEFT, 300);
	m_cListCtrl.InsertColumn(2, _T("描述"), LVCFMT_LEFT, 90);
	m_cListCtrl.InsertColumn(3, _T("公司名"), LVCFMT_LEFT, 200);
	m_cListCtrl.InsertColumn(4, _T("路径"), LVCFMT_LEFT, 360);	
	WxyEnumMsConfig(m_vMSConfigInfo);
	for (int i = 0; i < m_vMSConfigInfo.size(); i++)
	{
		m_cListCtrl.InsertItem(i, m_vMSConfigInfo[i]->ConfigName);
		m_cListCtrl.SetItemText(i, 1, m_vMSConfigInfo[i]->Key);
		m_cListCtrl.SetItemText(i, 2, m_vMSConfigInfo[i]->Description);
		m_cListCtrl.SetItemText(i, 3, m_vMSConfigInfo[i]->CompanyName);
		m_cListCtrl.SetItemText(i, 4, m_vMSConfigInfo[i]->FileFullPath);
	}
	return TRUE;  
}
