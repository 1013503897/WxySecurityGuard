#include "pch.h"
#include "CSecurityGuard.h"
#include "CDlgHackTools.h"
#include "afxdialogex.h"
#include "CDlgInject.h"
<<<<<<< HEAD

=======
#include "CDlgDisasm.h"
#include "CDlgFileBind.h"
#include "CDlgSeparate.h"
#include "CDlgDriverLoad.h"
>>>>>>> v1.0
IMPLEMENT_DYNAMIC(CDlgHackTools, CDialogEx)

CDlgHackTools::CDlgHackTools(CWnd* pParent)
	: CDialogEx(IDD_DIALOG_HACKTOOLS, pParent)
{

}

CDlgHackTools::~CDlgHackTools()
{
}

void CDlgHackTools::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlgHackTools, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &CDlgHackTools::OnBnClickedInject)
<<<<<<< HEAD
=======
	ON_BN_CLICKED(IDC_BUTTON2, &CDlgHackTools::OnBnClickedButtonDisasm)
	ON_BN_CLICKED(IDC_BUTTON3, &CDlgHackTools::OnBnClickedButtonBind)
	ON_BN_CLICKED(IDC_BUTTON6, &CDlgHackTools::OnBnClickedButtonSeparate)
	ON_BN_CLICKED(IDC_BUTTON5, &CDlgHackTools::OnBnClickedButtonDriverLoad)
>>>>>>> v1.0
END_MESSAGE_MAP()


void CDlgHackTools::OnBnClickedInject()
{
	CDlgInject dlg;
	dlg.DoModal();
}
<<<<<<< HEAD
=======


void CDlgHackTools::OnBnClickedButtonDisasm()
{
	CDlgDisasm dlg;
	dlg.DoModal();
}


void CDlgHackTools::OnBnClickedButtonBind()
{
	CDlgFileBind dlg;
	dlg.DoModal();
}


void CDlgHackTools::OnBnClickedButtonSeparate()
{
	CDlgSeparate dlg;
	dlg.DoModal();
}


void CDlgHackTools::OnBnClickedButtonDriverLoad()
{
	CDlgDriverLoad dlg;
	dlg.DoModal();
}
>>>>>>> v1.0
