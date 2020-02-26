#include "pch.h"
#include "CSecurityGuard.h"
#include "CDlgMemory.h"
#include "afxdialogex.h"

IMPLEMENT_DYNAMIC(CDlgMemory, CDialogEx)

CDlgMemory::CDlgMemory(CWnd* pParent)
	: CDialogEx(IDD_DIALOG_MEMORY, pParent)
{

}

CDlgMemory::~CDlgMemory()
{
}

void CDlgMemory::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_cListMemoryInfo);
}


BEGIN_MESSAGE_MAP(CDlgMemory, CDialogEx)
	ON_WM_SIZE()
END_MESSAGE_MAP()



BOOL CDlgMemory::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	m_cListMemoryInfo.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_SUBITEMIMAGES);
	m_cListMemoryInfo.InsertColumn(0, _T("属性"), LVCFMT_LEFT, 180);
	m_cListMemoryInfo.InsertColumn(1, _T("值"), LVCFMT_LEFT, 100);
	m_cListMemoryInfo.InsertColumn(2, _T("含义"), LVCFMT_LEFT, 200);

	return TRUE; 
}

VOID CDlgMemory::GetMemoryInfo(DWORD ProcessID)
{
	PPROCESS_MEMORY_COUNTERS pmc = new PROCESS_MEMORY_COUNTERS{};

	WxyEnumMemory(ProcessID, pmc);
	m_cListMemoryInfo.DeleteAllItems();
	int index = 0;
	m_cListMemoryInfo.InsertItem(index, _T("PageFaultCount"));
	CString PageFaultCount;
	PageFaultCount.Format(_T("%d"), pmc->PageFaultCount);
	m_cListMemoryInfo.SetItemText(index, 1, PageFaultCount);
	m_cListMemoryInfo.SetItemText(index++, 2, _T("缺页中断次数"));

	m_cListMemoryInfo.InsertItem(index, _T("PeakWorkingSetSize"));
	CString PeakWorkingSetSize;
	PeakWorkingSetSize.Format(_T("%d"), pmc->PeakWorkingSetSize);
	m_cListMemoryInfo.SetItemText(index, 1, PeakWorkingSetSize);
	m_cListMemoryInfo.SetItemText(index++, 2, _T("使用内存高峰"));

	m_cListMemoryInfo.InsertItem(index, _T("WorkingSetSize"));
	CString WorkingSetSize;
	WorkingSetSize.Format(_T("%d"), pmc->WorkingSetSize);
	m_cListMemoryInfo.SetItemText(index, 1, WorkingSetSize);
	m_cListMemoryInfo.SetItemText(index++, 2, _T("当前使用的内存"));

	m_cListMemoryInfo.InsertItem(index, _T("QuotaPeakPagedPoolUsage"));
	CString QuotaPeakPagedPoolUsage;
	QuotaPeakPagedPoolUsage.Format(_T("%d"), pmc->QuotaPeakPagedPoolUsage);
	m_cListMemoryInfo.SetItemText(index, 1, QuotaPeakPagedPoolUsage);
	m_cListMemoryInfo.SetItemText(index++, 2, _T("使用页面缓存池高峰"));

	m_cListMemoryInfo.InsertItem(index, _T("QuotaPagedPoolUsage"));
	CString QuotaPagedPoolUsage;
	QuotaPagedPoolUsage.Format(_T("%d"), pmc->QuotaPagedPoolUsage);
	m_cListMemoryInfo.SetItemText(index, 1, QuotaPagedPoolUsage);
	m_cListMemoryInfo.SetItemText(index++, 2, _T("使用页面缓存池"));

	m_cListMemoryInfo.InsertItem(index, _T("QuotaPeakNonPagedPoolUsage"));
	CString QuotaPeakNonPagedPoolUsage;
	QuotaPeakNonPagedPoolUsage.Format(_T("%d"), pmc->QuotaPeakNonPagedPoolUsage);
	m_cListMemoryInfo.SetItemText(index, 1, QuotaPeakNonPagedPoolUsage);
	m_cListMemoryInfo.SetItemText(index++, 2, _T("使用非分页缓存池高峰"));

	m_cListMemoryInfo.InsertItem(index, _T("QuotaNonPagedPoolUsage"));
	CString QuotaNonPagedPoolUsage;
	QuotaNonPagedPoolUsage.Format(_T("%d"), pmc->QuotaNonPagedPoolUsage);
	m_cListMemoryInfo.SetItemText(index, 1, QuotaNonPagedPoolUsage);
	m_cListMemoryInfo.SetItemText(index++, 2, _T("使用非分页缓存池"));

	m_cListMemoryInfo.InsertItem(index, _T("PagefileUsage"));
	CString PagefileUsage;
	PagefileUsage.Format(_T("%d"), pmc->PagefileUsage);
	m_cListMemoryInfo.SetItemText(index, 1, PagefileUsage);
	m_cListMemoryInfo.SetItemText(index++, 2, _T("使用分页文件"));

	m_cListMemoryInfo.InsertItem(index, _T("PeakPagefileUsage"));
	CString PeakPagefileUsage;
	PeakPagefileUsage.Format(_T("%d"), pmc->PeakPagefileUsage);
	m_cListMemoryInfo.SetItemText(index, 1, PeakPagefileUsage);
	m_cListMemoryInfo.SetItemText(index++, 2, _T("使用分页文件高峰"));
	delete pmc;
}


void CDlgMemory::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	if (m_cListMemoryInfo.m_hWnd)
	{
		CRect rc;
		this->GetClientRect(rc);
		m_cListMemoryInfo.MoveWindow(rc);
	}
}
