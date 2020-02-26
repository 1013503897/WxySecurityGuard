#include "pch.h"
#include "CSecurityGuard.h"
#include "CDlgMiniProcess.h"
#include "afxdialogex.h"
#include "CDlgHookDetect.h"

IMPLEMENT_DYNAMIC(CDlgMiniProcess, CDialogEx)

CDlgMiniProcess::CDlgMiniProcess(CWnd* pParent)
	: CDialogEx(IDD_DIALOG_MINIPROCESS, pParent)
{

}

CDlgMiniProcess::~CDlgMiniProcess()
{
}

void CDlgMiniProcess::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_MINI_PROCESS, m_cListProcess);
}


BEGIN_MESSAGE_MAP(CDlgMiniProcess, CDialogEx)
	ON_BN_CLICKED(IDC_CHECK_ALLCHECK, &CDlgMiniProcess::OnBnClickedAllCheck)
	ON_BN_CLICKED(IDC_BUTTON2, &CDlgMiniProcess::OnBnClickedButtonCancle)
	ON_BN_CLICKED(IDC_BUTTON1, &CDlgMiniProcess::OnBnClickedButtonOK)
END_MESSAGE_MAP()



BOOL CDlgMiniProcess::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	m_cImageList.Create(16, 16, ILC_COLOR32 | ILC_MASK, 3, 3);
	m_cListProcess.SetExtendedStyle(LVS_EX_CHECKBOXES);
	m_cListProcess.InsertColumn(0, _T("进程名"), LVCFMT_LEFT, 150);
	m_cListProcess.InsertColumn(1, _T("进程ID"), LVCFMT_LEFT, 80);
	m_cListProcess.InsertColumn(2, _T("进程位数"), LVCFMT_LEFT, 80);
	m_cListProcess.InsertColumn(3, _T("路径"), LVCFMT_LEFT, 200);
	WxyEnumProcess(m_vProcessInfo);
	for (size_t i = 0; i < m_vProcessInfo.size(); i++)
	{
		if (-1 == m_cImageList.Add(m_vProcessInfo[i]->ProcessIcon))
			m_cImageList.Add(AfxGetApp()->LoadIcon(IDI_ICON5));
	}
	m_cListProcess.SetImageList(&m_cImageList, LVSIL_SMALL);
	for (int i = 0; i < m_vProcessInfo.size(); i++)
	{
		m_cListProcess.InsertItem(i, m_vProcessInfo[i]->ProcessName,i);
		CString ProcessID;
		ProcessID.Format(_T("%d"), m_vProcessInfo[i]->ProcessID);
		m_cListProcess.SetItemText(i, 1, ProcessID);
		m_cListProcess.SetItemText(i, 2, m_vProcessInfo[i]->ProcessBitInfo);
		m_cListProcess.SetItemText(i, 3, m_vProcessInfo[i]->FileFullPath);
	}
	return TRUE;  
}


void CDlgMiniProcess::OnBnClickedAllCheck()
{
	BOOL Status = ((CButton*)GetDlgItem(IDC_CHECK_ALLCHECK))->GetCheck();
	for (int i = 0; i < m_cListProcess.GetItemCount(); i++)
	{
		m_cListProcess.SetCheck(i, Status);
	}
}


void CDlgMiniProcess::OnBnClickedButtonCancle()
{
	CDialogEx::OnOK();
}


void CDlgMiniProcess::OnBnClickedButtonOK()
{
	IsIAT = ((CButton*)GetDlgItem(IDC_CHECK_IAT))->GetCheck();
	IsEAT = ((CButton*)GetDlgItem(IDC_CHECK_EAT))->GetCheck();
	IsInline = ((CButton*)GetDlgItem(IDC_CHECK_INLINE))->GetCheck();
	for (int i = 0; i < m_cListProcess.GetItemCount(); i++)
	{
		if (m_cListProcess.GetCheck(i))
		{
			if(!_tcsstr(m_vProcessInfo[i]->ProcessName,_T("svchost"))&& !_tcsstr(m_vProcessInfo[i]->ProcessName, _T("QQ")))
				m_vProcessInfo2.push_back(m_vProcessInfo[i]);
		}
	}
	CDialogEx::OnOK();
}
