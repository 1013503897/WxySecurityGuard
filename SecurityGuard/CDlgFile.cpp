#include "pch.h"
#include "CSecurityGuard.h"
#include "CDlgFile.h"
#include "afxdialogex.h"
#include "CDlgFileLeft.h"
#include "CDlgFileRight.h"

IMPLEMENT_DYNAMIC(CDlgFile, CDialogEx)

CDlgFile::CDlgFile(CWnd* pParent)
	: CDialogEx(IDD_DIALOG_FILE, pParent)
{

}

CDlgFile::~CDlgFile()
{
}

void CDlgFile::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlgFile, CDialogEx)
	ON_WM_CREATE()
	ON_WM_SIZE()
END_MESSAGE_MAP()

BOOL CDlgFile::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	return TRUE;    
}


int CDlgFile::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialogEx::OnCreate(lpCreateStruct) == -1)
		return -1;

	CString strMyClass = AfxRegisterWndClass(CS_VREDRAW | CS_HREDRAW,
		::LoadCursor(NULL, IDC_ARROW), (HBRUSH) ::GetStockObject(WHITE_BRUSH),
		::LoadIcon(NULL, IDI_APPLICATION));

	m_pMyFrame = new CFrameWnd;
	m_pMyFrame->Create(strMyClass, _T(""), WS_CHILD, CRect(0, 0, 1, 1), this);
	m_pMyFrame->ShowWindow(SW_SHOW);
	m_cSplitter.CreateStatic(m_pMyFrame, 1, 2); 
	m_cSplitter.CreateView(0, 0, RUNTIME_CLASS(CDlgFileLeft), CSize(1, 1), NULL);
	m_cSplitter.CreateView(0, 1, RUNTIME_CLASS(CDlgFileRight), CSize(1, 1), NULL);

	return 0;
}


void CDlgFile::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);
	if (m_pMyFrame)
	{
		CRect rc;
		this->GetClientRect(rc);
		m_pMyFrame->MoveWindow(rc);
		m_cSplitter.SetColumnInfo(0, rc.Width() * 0.3, 10);
		m_cSplitter.RecalcLayout();
	}
}
