#include "pch.h"
#include "Helper.h"
#include "CSecurityGuard.h"
#include "CDlgProcess.h"
#include "CDlgThread.h"
#include "CDlgModule.h"
#include "CDlgHeap.h"
#include "CDlgHandle.h"
#include "afxdialogex.h"
#include "CFormViewProcDown.h"
#include "CFormViewProcUp.h"

// CDlgProcess 对话框

IMPLEMENT_DYNAMIC(CDlgProcess, CDialogEx)

CDlgProcess::CDlgProcess(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_PROCESS, pParent)
{

}

CDlgProcess::~CDlgProcess()
{
}

BOOL CDlgProcess::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	CRect cRect;
	GetWindowRect(&cRect);
	ScreenToClient(&cRect);
	m_pMyFrame->MoveWindow(&cRect);
	m_pMyFrame->ShowWindow(SW_SHOW);
	
	//PrintProcessInfo(); 
	return TRUE;
}


void CDlgProcess::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlgProcess, CDialogEx)
	ON_WM_SIZE()
	ON_WM_CREATE()
END_MESSAGE_MAP()


// CDlgProcess 消息处理程序


void CDlgProcess::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);
	if (m_pMyFrame)
	{
		CRect rc;
		GetClientRect(rc);
		m_pMyFrame->MoveWindow(rc);
		m_cSplitter.SetRowInfo(0, rc.Height() * 0.6, 10);
		m_cSplitter.RecalcLayout();
	}
}

VOID RefreshProcessInfo(CDlgProcess* DlgProcess)
{
	while (true)
	{
		//Sleep(1000);
		//WxyEnumProcess(DlgProcess->m_Vector_ProcessInfo);
		//DlgProcess->PrintProcessInfo();
	}
}



int CDlgProcess::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialogEx::OnCreate(lpCreateStruct) == -1)
		return -1;

	CString strMyClass = AfxRegisterWndClass(CS_VREDRAW | CS_HREDRAW,
		::LoadCursor(NULL, IDC_ARROW), (HBRUSH) ::GetStockObject(WHITE_BRUSH),
		::LoadIcon(NULL, IDI_APPLICATION));

	m_pMyFrame = new CFrameWnd;
	m_pMyFrame->Create(strMyClass, _T(""), WS_CHILD, CRect(0, 0, 1, 1), this);
	m_pMyFrame->ShowWindow(SW_SHOW);

	m_cSplitter.CreateStatic(m_pMyFrame, 2, 1); 
	m_cSplitter.CreateView(0, 0, RUNTIME_CLASS(CFormViewProcUp), CSize(1, 1), NULL);
	m_cSplitter.CreateView(1, 0, RUNTIME_CLASS(CFormViewProcDown), CSize(1, 1), NULL);

	return 0;
}