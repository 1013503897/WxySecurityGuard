// ResourceInfoDlg.cpp: 实现文件
//

#include "pch.h"
#include "CSecurityGuard.h"
#include "CResourceInfoDlg.h"
#include "afxdialogex.h"
#include "CPEInfoDlg.h"

// ResourceInfoDlg 对话框

IMPLEMENT_DYNAMIC(ResourceInfoDlg, CDialogEx)

ResourceInfoDlg::ResourceInfoDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_RESOURCE, pParent)
{

}

ResourceInfoDlg::~ResourceInfoDlg()
{
}

void ResourceInfoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TREE2, m_cTreeCtrl);
	DDX_Control(pDX, IDC_LIST2, m_cListCtrl);
}


BEGIN_MESSAGE_MAP(ResourceInfoDlg, CDialogEx)
	ON_WM_SIZE()
	ON_NOTIFY(TVN_SELCHANGED, IDC_TREE2, &ResourceInfoDlg::OnTvnSelchangedTree1)
END_MESSAGE_MAP()


// ResourceInfoDlg 消息处理程序


void ResourceInfoDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	CDialogEx::OnSize(nType, cx, cy);
	if (m_cTreeCtrl)
	{
		CRect rect;
		GetClientRect(rect); 
		rect.bottom /= 2;
		rect.bottom -= 2;
		m_cTreeCtrl.MoveWindow(rect);
	}
	if (m_cListCtrl)
	{
		CRect rect;
		GetClientRect(rect); 
		rect.top = rect.bottom / 2 + 2;
		m_cListCtrl.MoveWindow(rect);
	}
}


void ResourceInfoDlg::OnTvnSelchangedTree1(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	CPEInfoDlg* Parent = (CPEInfoDlg*)GetParent();
	if (m_cTreeCtrl.GetCount() > 1)
	{
		Parent->PrintResourceItem(pNMHDR, pResult);
	}
	*pResult = 0;
}


BOOL ResourceInfoDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	m_cListCtrl.InsertColumn(0, _T("成员"), LVCFMT_LEFT, 100);
	m_cListCtrl.InsertColumn(1, _T("偏移量"), LVCFMT_LEFT, 100);
	m_cListCtrl.InsertColumn(2, _T("大小"), LVCFMT_LEFT, 100);
	m_cListCtrl.InsertColumn(3, _T("值"), LVCFMT_LEFT, 100);
	return TRUE;  
				  
}
