#include "pch.h"
#include "CSecurityGuard.h"
#include "CDisasmDlg.h"
#include "CPEInfoDlg.h"

IMPLEMENT_DYNAMIC(CDisasmDlg, CDialogEx)

<<<<<<< HEAD
=======
#define BEA_ENGINE_STATIC
#define BEA_USE_STDCALL

#ifndef _WIN64
#include "BeaEngine_4.1/Win32/headers/BeaEngine.h"
#pragma comment (lib , "BeaEngine_4.1/Win32/Win32/Lib/BeaEngine.lib")
#else
#include "BeaEngine_4.1/Win64/headers/BeaEngine.h"
#pragma comment (lib , "BeaEngine_4.1/Win64/Win64/Lib/BeaEngine.lib")
#endif
#pragma comment(linker, "/NODEFAULTLIB:\"crt.lib\"")

>>>>>>> v1.0
CDisasmDlg::CDisasmDlg(CWnd* pParent)
	: CDialogEx(IDD_DIALOG_DISASM, pParent)
	, m_edit_size(_T(""))
	, m_edit_offset(_T(""))
	, m_check(FALSE)
	, m_edit_base(_T(""))
{

}

CDisasmDlg::~CDisasmDlg()
{
}

void CDisasmDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST2, m_cListCtrl);
	DDX_Check(pDX, IDC_CHECK1, m_check);
	DDX_Control(pDX, IDC_COMBO1, m_combobox);
	DDX_Text(pDX, IDC_EDIT12, m_edit_base);
	DDX_Text(pDX, IDC_EDIT9, m_edit_offset);
	DDX_Text(pDX, IDC_EDIT10, m_edit_size);
}

BEGIN_MESSAGE_MAP(CDisasmDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &CDisasmDlg::OnBnClickedButtonEnumFile)
	ON_WM_SIZE()
END_MESSAGE_MAP()


void CDisasmDlg::OnBnClickedButtonEnumFile()
{
	CPEInfoDlg* Parent = (CPEInfoDlg*)GetParent();
	UpdateData(true);
	Parent->StartDisasm();
}

void CDisasmDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);
	
	if (m_cListCtrl)
	{
		CRect rectDlg;
		GetClientRect(rectDlg);
		rectDlg.DeflateRect(0, 210, 0, 0);
		GetDlgItem(IDC_STATIC3)->MoveWindow(rectDlg);
		rectDlg.DeflateRect(1, 24, 1, 1);
		m_cListCtrl.MoveWindow(rectDlg);
	}
}

BOOL CDisasmDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	m_cListCtrl.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	m_check = true;
	m_edit_size = _T("1000");	
	m_combobox.AddString(_T("x86"));
	m_combobox.AddString(_T("x64"));
	m_combobox.SetCurSel(1);
	m_cListCtrl.InsertColumn(0, _T("地址"), LVCFMT_LEFT, 100);
	m_cListCtrl.InsertColumn(1, _T("机器码"), LVCFMT_LEFT, 200);
	m_cListCtrl.InsertColumn(2, _T("指令"), LVCFMT_LEFT, 250);

	UpdateData(false);
	return TRUE;  	  
}


void CDisasmDlg::InitEditAddress(int OEP, int OEP_VA)
{
	m_edit_base.Format(_T("%08X"), OEP_VA);
	m_edit_offset.Format(_T("%X"), OEP);
	UpdateData(false);
}