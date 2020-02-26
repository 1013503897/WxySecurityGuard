
#include "pch.h"
#include "CSecurityGuard.h"
#include "CFormViewProcUp.h"
#include "CFormViewProcDown.h"
#include "CDlgProcess.h"
#include "CGeneralTabCtrl.h"
#include "CDlgSecurityGuard.h"
#define MY_SELCHANGE (WM_USER+1001)

IMPLEMENT_DYNCREATE(CFormViewProcUp, CFormView)

CFormViewProcUp::CFormViewProcUp()
	: CFormView(IDD_FORMVIEW_UP)
{

}

CFormViewProcUp::~CFormViewProcUp()
{
}

void CFormViewProcUp::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_cListProcess);
}

BEGIN_MESSAGE_MAP(CFormViewProcUp, CFormView)
	ON_WM_SIZE()
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST1, &CFormViewProcUp::OnLvnItemchangedList1)
	ON_NOTIFY(NM_RCLICK, IDC_LIST1, &CFormViewProcUp::OnNMRClickList1)
	ON_COMMAND(ID_32771, &CFormViewProcUp::OnPressTmntProc)
END_MESSAGE_MAP()


#ifdef _DEBUG
void CFormViewProcUp::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CFormViewProcUp::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif 

void CFormViewProcUp::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();

	m_cListProcess.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_SUBITEMIMAGES);
	m_cListProcess.InsertColumn(0, _T("进程名"), LVCFMT_LEFT, 200);
	m_cListProcess.InsertColumn(1, _T("进程ID"), LVCFMT_LEFT, 70);
	m_cListProcess.InsertColumn(2, _T("进程位数"), LVCFMT_LEFT, 70);
	m_cListProcess.InsertColumn(3, _T("线程数"), LVCFMT_LEFT, 70);
	m_cListProcess.InsertColumn(4, _T("描述"), LVCFMT_LEFT, 320);
	m_cListProcess.InsertColumn(5, _T("公司名"), LVCFMT_LEFT, 200);
	m_cListProcess.InsertColumn(6, _T("路径"), LVCFMT_LEFT, 1000);
	m_cImageList.Create(16, 16, ILC_COLOR32 | ILC_MASK, 3, 3);
	WxyEnumProcess(m_vProcessInfo);
	PrintProcessInfo();
}

void CFormViewProcUp::OnSize(UINT nType, int cx, int cy)
{
	CFormView::OnSize(nType, cx, cy);
	if (m_cListProcess.m_hWnd)
	{
		CRect rc;
		this->GetClientRect(rc);
		CSize size;
		size.cx = rc.right - rc.left;
		size.cy = rc.bottom - rc.top;
		SetScrollSizes(MM_HIMETRIC, size);
		m_cListProcess.MoveWindow(rc);
	}
}

VOID CFormViewProcUp::PrintProcessInfo()
{
	int iImageCount = m_cImageList.GetImageCount();
	for (int i = 0; i < iImageCount; i++)
	{
		m_cImageList.Remove(0);
	}
	for (size_t i = 0; i < m_vProcessInfo.size(); i++)
	{
		if (-1 == m_cImageList.Add(m_vProcessInfo[i]->ProcessIcon))
			m_cImageList.Add(AfxGetApp()->LoadIcon(IDI_ICON5));
	}
	m_cListProcess.SetImageList(&m_cImageList, LVSIL_SMALL);
	int index = 0;
	for (size_t i = 0; i < m_vProcessInfo.size(); i++)
	{
		bool IsEqual = false;
		for (size_t j = 0; j < i; j++)
		{
			if (_tcscmp(m_vProcessInfo[i]->ProcessName, m_vProcessInfo[j]->ProcessName) == 0)
			{
				//IsEqual = true;
				break;
			}
		}
		if (!IsEqual)
		{
			CString ProcessID;
			CString ThreadNum;
			ProcessID.Format(_T("%d"), m_vProcessInfo[i]->ProcessID);
			ThreadNum.Format(_T("%d"), m_vProcessInfo[i]->ThreadNum);
			m_cListProcess.InsertItem(index, _T(""), index);
			m_cListProcess.SetItemText(index, 0, m_vProcessInfo[i]->ProcessName);
			m_cListProcess.SetItemText(index, 1, ProcessID);
			m_cListProcess.SetItemText(index, 2, m_vProcessInfo[i]->ProcessBitInfo);
			m_cListProcess.SetItemText(index, 3, ThreadNum);
			m_cListProcess.SetItemText(index, 4, m_vProcessInfo[i]->FileDescription);
			m_cListProcess.SetItemText(index, 5, m_vProcessInfo[i]->CompanyName);
			m_cListProcess.SetItemText(index, 6, m_vProcessInfo[i]->FileFullPath);
			index++;
		}
	}
}


void CFormViewProcUp::OnLvnItemchangedList1(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR); 
	POSITION pos = m_cListProcess.GetFirstSelectedItemPosition();
	if (pos != 0)
	{
		CDlgSecurityGuard* MainWnd = (CDlgSecurityGuard*)AfxGetMainWnd();
		CDlgProcess* processdlg = (CDlgProcess*)MainWnd->m_cMainTable.m_Dia[0];
		CFormViewProcDown* pView = (CFormViewProcDown*)processdlg->m_cSplitter.GetPane(1, 0);
		pView->EnumInfo(m_vProcessInfo[(int)pos - 1]->ProcessID);
	}
	*pResult = 0;
}


void CFormViewProcUp::OnNMRClickList1(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);

	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	if (pNMListView->iItem != -1)
	{
		DWORD dwPos = GetMessagePos();
		CPoint point(LOWORD(dwPos), HIWORD(dwPos));
		CMenu menu;
		VERIFY(menu.LoadMenu(IDR_MENU_PROCESS));
		CMenu* popup = menu.GetSubMenu(0);
		ASSERT(popup != NULL);
		popup->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, point.x, point.y, this);
	}
	*pResult = 0;
}


void CFormViewProcUp::OnPressTmntProc()
{
	POSITION pos = m_cListProcess.GetFirstSelectedItemPosition();
	int nItem = m_cListProcess.GetNextSelectedItem(pos);
	if (nItem != 0)
	{
		HANDLE ProcessHandle = OpenProcess(PROCESS_TERMINATE, NULL, m_vProcessInfo[nItem]->ProcessID);
		if (ProcessHandle)
		{
			TerminateProcess(ProcessHandle, -1);
			m_cListProcess.DeleteItem(nItem);
			CloseHandle(ProcessHandle);
		}
	}
}
