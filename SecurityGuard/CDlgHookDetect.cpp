#include "pch.h"
#include "CSecurityGuard.h"
#include "CDlgHookDetect.h"
#include "afxdialogex.h"
#include "CDlgMiniProcess.h"

IMPLEMENT_DYNAMIC(CDlgHookDetect, CDialogEx)

CDlgHookDetect::CDlgHookDetect(CWnd* pParent )
	: CDialogEx(IDD_DIALOG_HOOKDETECT, pParent)
{

}

CDlgHookDetect::~CDlgHookDetect()
{
}

void CDlgHookDetect::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_HOOKINFO, m_cListHookDetect);
	DDX_Control(pDX, IDC_EDIT_DETECTING, m_cEditDetecting);
	DDX_Control(pDX, IDC_PROGRESS_DETECTING, m_cProgressDetecting);
}


BEGIN_MESSAGE_MAP(CDlgHookDetect, CDialogEx)
	ON_WM_SIZE()
<<<<<<< HEAD
	ON_BN_CLICKED(IDC_BUTTON1, &CDlgHookDetect::OnBnClickedButton1)
=======
	ON_BN_CLICKED(IDC_BUTTON1, &CDlgHookDetect::OnBnClickedButtonDisasm)
>>>>>>> v1.0
END_MESSAGE_MAP()



void CDlgHookDetect::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	CRect rc;
	GetClientRect(rc);
	rc.left += 150;
	if (m_cListHookDetect.m_hWnd)
	{
		rc.bottom -= 60;
		m_cListHookDetect.MoveWindow(rc);
		rc.top = rc.bottom + 5;
		rc.bottom = rc.top + 20;
		m_cEditDetecting.MoveWindow(rc); 
		rc.top = rc.bottom + 5;
		rc.bottom = rc.top + 20;
		m_cProgressDetecting.MoveWindow(rc);
	}
}


BOOL CDlgHookDetect::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	m_cListHookDetect.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_SUBITEMIMAGES);
	m_cListHookDetect.InsertColumn(0, _T("进程名"), LVCFMT_LEFT, 150);
	m_cListHookDetect.InsertColumn(1, _T("进程ID"), LVCFMT_LEFT, 80);
	m_cListHookDetect.InsertColumn(2, _T("类型"), LVCFMT_LEFT, 80);
	m_cListHookDetect.InsertColumn(3, _T("源地址"), LVCFMT_LEFT, 120);
	m_cListHookDetect.InsertColumn(4, _T("源位置"), LVCFMT_LEFT, 200);
	m_cListHookDetect.InsertColumn(5, _T("目标地址"), LVCFMT_LEFT, 120);
	m_cListHookDetect.InsertColumn(6, _T("目标位置"), LVCFMT_LEFT, 200);
	m_cListHookDetect.InsertColumn(7, _T("目标模块"), LVCFMT_LEFT, 400);
	return TRUE; 
}


<<<<<<< HEAD
void CDlgHookDetect::OnBnClickedButton1()
=======
void CDlgHookDetect::OnBnClickedButtonDisasm()
>>>>>>> v1.0
{
	CDlgMiniProcess dlg;
	if (dlg.DoModal() == IDOK)
	{
		for (auto it : m_vHookInfoVec)
		{
			delete it;
		}
		m_vHookInfoVec.clear();
		m_cProgressDetecting.SetRange(0, dlg.m_vProcessInfo2.size());
		for (int i = 0; i < dlg.m_vProcessInfo2.size(); i++)
		{
			m_cEditDetecting.SetWindowText(dlg.m_vProcessInfo2[i]->ProcessName);
			m_cProgressDetecting.OffsetPos(1);
			UpdateWindow();
			if (!_tcscmp(dlg.m_vProcessInfo2[i]->ProcessBitInfo, _T("x32")))
			{
				if(dlg.IsIAT)
					WxyCheckIAT(dlg.m_vProcessInfo2[i]->ProcessID, m_vHookInfoVec);
				if(dlg.IsEAT)
					WxyCheckEAT(dlg.m_vProcessInfo2[i]->ProcessID, m_vHookInfoVec);
				if (dlg.IsInline)
					WxyCheckInline(dlg.m_vProcessInfo2[i]->ProcessID, m_vHookInfoVec);
			}
		}
		m_cEditDetecting.SetWindowText(_T("扫描完毕"));
		m_cListHookDetect.DeleteAllItems();
		for (int i = 0; i < m_vHookInfoVec.size(); i++)
		{
			m_cListHookDetect.InsertItem(i, m_vHookInfoVec[i]->ProcessName);
			CString ProcessID;
			ProcessID.Format(_T("%d"), m_vHookInfoVec[i]->ProcessID);
			m_cListHookDetect.SetItemText(i, 1, ProcessID);
			m_cListHookDetect.SetItemText(i, 2, m_vHookInfoVec[i]->HookType);
			CString SrcAddress;
			SrcAddress.Format(_T("0x%08X"), m_vHookInfoVec[i]->SrcAddress);
			m_cListHookDetect.SetItemText(i, 3, SrcAddress);
			m_cListHookDetect.SetItemText(i, 4, m_vHookInfoVec[i]->SrcPlace);
			CString DstAddress;
			DstAddress.Format(_T("0x%08X"), m_vHookInfoVec[i]->DstAddress);
			m_cListHookDetect.SetItemText(i, 5, DstAddress);
			m_cListHookDetect.SetItemText(i, 6, m_vHookInfoVec[i]->DstPlace);
			m_cListHookDetect.SetItemText(i, 7, m_vHookInfoVec[i]->DstDll);
		}
	}
	}
	
