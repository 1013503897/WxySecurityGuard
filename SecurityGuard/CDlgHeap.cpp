#include "pch.h"
#include "CSecurityGuard.h"
#include "CDlgHeap.h"
#include "afxdialogex.h"

IMPLEMENT_DYNAMIC(CDlgHeap, CDialogEx)

CDlgHeap::CDlgHeap(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_HEAP, pParent)
{

}

CDlgHeap::~CDlgHeap()
{
}

void CDlgHeap::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_cListHeapInfo);
}


BEGIN_MESSAGE_MAP(CDlgHeap, CDialogEx)
	ON_WM_SIZE()
END_MESSAGE_MAP()



BOOL CDlgHeap::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	m_cListHeapInfo.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_SUBITEMIMAGES);
	m_cListHeapInfo.InsertColumn(0, _T("堆ID"), LVCFMT_LEFT, 100);
	m_cListHeapInfo.InsertColumn(1, _T("堆大小"), LVCFMT_LEFT, 200);
	return TRUE;  	  
}


void CDlgHeap::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);
	if (m_cListHeapInfo.m_hWnd)
	{
		CRect rc;
		this->GetClientRect(rc);
		m_cListHeapInfo.MoveWindow(rc);
	}
}

VOID CDlgHeap::EnumHeap(DWORD ProcessID)
{
	PHEAP_HELPER pHeapHelper  = new HEAP_HELPER{ this, ProcessID };
	_beginthread(EnumHeapHelp, NULL, (PVOID)pHeapHelper);
}

VOID CDlgHeap::ShowHeapInfo()
{
	m_cListHeapInfo.DeleteAllItems();
	for (size_t i = 0; i < m_vHeapInfo.size(); i++)
	{
		m_cListHeapInfo.InsertItem(i, _T(""));
		CString HeapID;
		CString HeapSize;
		HeapID.Format(_T("%d"), m_vHeapInfo[i]->HeapID);
		HeapSize.Format(_T("%d B"), m_vHeapInfo[i]->BlockSize);
		m_cListHeapInfo.SetItemText(i, 0, HeapID);
		m_cListHeapInfo.SetItemText(i, 1, HeapSize);
	}
}

VOID EnumHeapHelp(PVOID Param)
{
	CDlgHeap* m_Dlg = ((PHEAP_HELPER)Param)->m_CDlgHandle;
	DWORD ProcessID = ((PHEAP_HELPER)Param)->m_ProcessID;
	WxyEnumHeap(ProcessID, m_Dlg->m_vHeapInfo);
	m_Dlg->ShowHeapInfo();
	delete (PHEAP_HELPER)Param;
}