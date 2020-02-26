#include "pch.h"
#include "CSecurityGuard.h"
#include "CDlgHandle.h"
#include "afxdialogex.h"


IMPLEMENT_DYNAMIC(CDlgHandle, CDialogEx)

CDlgHandle::CDlgHandle(CWnd* pParent)
	: CDialogEx(IDD_DIALOG_HANDLE, pParent)
{

}

CDlgHandle::~CDlgHandle()
{
}

void CDlgHandle::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_cListHandleInfo);
}


BEGIN_MESSAGE_MAP(CDlgHandle, CDialogEx)
	ON_WM_SIZE()
END_MESSAGE_MAP()

BOOL CDlgHandle::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	m_cListHandleInfo.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_SUBITEMIMAGES);
	m_cListHandleInfo.InsertColumn(0, _T("句柄值"), LVCFMT_LEFT, 80);
	m_cListHandleInfo.InsertColumn(1, _T("对象"), LVCFMT_LEFT, 160);
	m_cListHandleInfo.InsertColumn(2, _T("句柄类型名"), LVCFMT_LEFT, 160);
	m_cListHandleInfo.InsertColumn(3, _T("句柄类型索引"), LVCFMT_LEFT, 150);
	m_cListHandleInfo.InsertColumn(4, _T("句柄名称"), LVCFMT_LEFT, 800);

	return TRUE;  
}


void CDlgHandle::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	CRect rc;
	this->GetClientRect(rc);
	if (m_cListHandleInfo.m_hWnd)
	{
		m_cListHandleInfo.MoveWindow(rc);
	}
}

VOID CDlgHandle::EnumHandleInfo(DWORD ProcessID)
{
	PHANDLE_HELPER pHandleHelper = new HANDLE_HELPER{ this,ProcessID };
	_beginthread(EnumHandleHelp, NULL, pHandleHelper);
}

VOID CDlgHandle::ShowHandleInfo()
{
	m_cListHandleInfo.DeleteAllItems();
	for (size_t i = 0; i < m_vHandleInfo.size(); i++)
	{
		m_cListHandleInfo.InsertItem(i, _T(""));
		CString HandleValue;
		CString HandleObject;
		CString ObjectTypeIndex;
		HandleValue.Format(_T("%d"), m_vHandleInfo[i]->HandleValue);
		HandleObject.Format(_T("%p"), m_vHandleInfo[i]->Object);
		ObjectTypeIndex.Format(_T("%d"), m_vHandleInfo[i]->ObjectTypeIndex);
		m_cListHandleInfo.SetItemText(i, 0, HandleValue);
		m_cListHandleInfo.SetItemText(i, 1, HandleObject);
		m_cListHandleInfo.SetItemText(i, 2, m_vHandleInfo[i]->ObjectTypeName);
		m_cListHandleInfo.SetItemText(i, 3, ObjectTypeIndex);
		m_cListHandleInfo.SetItemText(i, 4, m_vHandleInfo[i]->HandleName);
	}
}

VOID EnumHandleHelp(PVOID Param)
{
	CDlgHandle* m_Dlg = ((PHANDLE_HELPER)Param)->m_CDlgHandle;
	DWORD ProcessID = ((PHANDLE_HELPER)Param)->m_ProcessID;
	WxyEnumHandle(ProcessID, m_Dlg->m_vHandleInfo);
	m_Dlg->ShowHandleInfo();
	delete (PHANDLE_HELPER)Param;
}
