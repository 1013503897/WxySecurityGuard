#include "pch.h"
#include <afxwin.h>
#include "CSecurityGuard.h"
#include "CDlgModule.h"
#include "afxdialogex.h"

IMPLEMENT_DYNAMIC(CDlgModule, CDialogEx)

CDlgModule::CDlgModule(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_MODULE, pParent)
{

}

CDlgModule::~CDlgModule()
{
}

void CDlgModule::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_MODULE, m_cListModule);
}


BEGIN_MESSAGE_MAP(CDlgModule, CDialogEx)
	ON_WM_SIZE()
END_MESSAGE_MAP()


void CDlgModule::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);
	if (m_cListModule.m_hWnd)
	{
		CRect rc;
		this->GetClientRect(rc);
		m_cListModule.MoveWindow(rc);
	}
}


BOOL CDlgModule::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	m_cListModule.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_SUBITEMIMAGES);
	m_cListModule.InsertColumn(0, _T("名称"), LVCFMT_LEFT, 150);
	m_cListModule.InsertColumn(1, _T("大小"), LVCFMT_LEFT, 70);
	m_cListModule.InsertColumn(2, _T("模块加载基址"), LVCFMT_LEFT, 100);
	m_cListModule.InsertColumn(3, _T("路径"), LVCFMT_LEFT, 300);
	m_cListModule.InsertColumn(4, _T("公司名"), LVCFMT_LEFT, 200);
	m_cListModule.InsertColumn(5, _T("描述"), LVCFMT_LEFT, 250);
	return TRUE; 
}

VOID CDlgModule::EnumModule(DWORD ProcessID)
{
	WxyEnumModule(ProcessID, m_vModuleInfo);
	ShowModuleInfo();
}

VOID CDlgModule::ShowModuleInfo()
{
	m_cListModule.DeleteAllItems();
	for (size_t i = 0; i < m_vModuleInfo.size(); i++)
	{
		m_cListModule.InsertItem(i, _T(""));
		CString ModuleSize;
		CString ModuleBaseAddr;
		ModuleSize.Format(_T("%d"), m_vModuleInfo[i]->ModuleSize);
#ifndef _WIN64
		ModuleBaseAddr.Format(_T("0x%08X"), m_vModuleInfo[i]->ImageBase);
#else
		ModuleBaseAddr.Format(_T("0x%016X"), m_ModuleInfo[i]->ImageBase);
#endif
		m_cListModule.SetItemText(i, 0, m_vModuleInfo[i]->ModuleName);
		m_cListModule.SetItemText(i, 1, ModuleSize);
		m_cListModule.SetItemText(i, 2, ModuleBaseAddr);
		m_cListModule.SetItemText(i, 3, m_vModuleInfo[i]->FileFullPath);
		m_cListModule.SetItemText(i, 4, m_vModuleInfo[i]->CompanyName);
		m_cListModule.SetItemText(i, 5, m_vModuleInfo[i]->FileDescription);
	}
}
