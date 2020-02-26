#include "pch.h"
#include "CSecurityGuard.h"
#include "CTableInfoDlg.h"
#include "afxdialogex.h"
#include "CPEInfoDlg.h"

IMPLEMENT_DYNAMIC(TableInfoDlg, CDialogEx)

TableInfoDlg::TableInfoDlg(CWnd* pParent)
	: CDialogEx(IDD_DIALOG_PE_TABLEINFO, pParent)
{

}

TableInfoDlg::~TableInfoDlg()
{
}

void TableInfoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST2, m_UpList);
	DDX_Control(pDX, IDC_LIST3, m_DownList);
}


BEGIN_MESSAGE_MAP(TableInfoDlg, CDialogEx)
	ON_WM_SIZE()
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST2, &TableInfoDlg::OnLvnItemchangedList2)
END_MESSAGE_MAP()




BOOL TableInfoDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	m_UpList.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	m_DownList.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);

	return TRUE;  
				  
}

void TableInfoDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);
	if (m_UpList)
	{
		CRect rect;
		GetClientRect(rect);
		rect.bottom /= 2;
		rect.bottom -= 2;
		m_UpList.MoveWindow(rect);
	}
	if (m_DownList)
	{
		CRect rect;
		GetClientRect(rect);
		rect.top = rect.bottom / 2 + 2;
		m_DownList.MoveWindow(rect);
	}
}

void TableInfoDlg::ClearListCtrl()
{
	m_UpList.DeleteAllItems();
	int nColumnCount = m_UpList.GetHeaderCtrl()->GetItemCount();
	for (int i = 0; i < nColumnCount; i++)
	{
		m_UpList.DeleteColumn(0);
	}
	ClearDownListCtrl();
	m_DownList.DeleteAllItems();
	nColumnCount = m_DownList.GetHeaderCtrl()->GetItemCount();
	for (int i = 0; i < nColumnCount; i++)
	{
		m_DownList.DeleteColumn(0);
	}
}

void TableInfoDlg::ClearDownListCtrl()
{
	m_DownList.DeleteAllItems();
}

void TableInfoDlg::OnLvnItemchangedList2(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	CPEInfoDlg* Parent = (CPEInfoDlg*)GetParent();
	if (m_UpList.GetItemCount() >= 1)
	{
		Parent->OnLvnItemchangedList2(pNMHDR, pResult);
	}
	*pResult = 0;
}
