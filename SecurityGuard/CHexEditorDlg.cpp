#include "pch.h"
#include "CSecurityGuard.h"
#include "CHexEditorDlg.h"

IMPLEMENT_DYNAMIC(CHexEditorDlg, CDialogEx)

CHexEditorDlg::CHexEditorDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_PE_HEXEDITOR, pParent)
{

}

CHexEditorDlg::~CHexEditorDlg()
{
}

void CHexEditorDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CUSTOM2, m_HexEditor);
	DDX_Control(pDX, IDC_EDIT9, m_edit1);
}


BEGIN_MESSAGE_MAP(CHexEditorDlg, CDialogEx)
	ON_WM_SIZE()
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()

BOOL CHexEditorDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	CString str;
	str = _T("偏移量              ");
	str += _T("0      1      2      3      4      5      6      7      8      9      A      B      C      D      E      F"); 
	str += _T("      Ascii    ");
	m_edit1.SetWindowText(str);
	return TRUE; 
}


void CHexEditorDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);
	if (m_HexEditor)
	{
		CRect rect;
		GetClientRect(rect);
		rect.bottom -= 2;
		rect.top += 20;
		rect.right = 620;
		m_HexEditor.MoveWindow(rect);
	}	
	if (m_edit1)
	{
		CRect rect;
		GetClientRect(rect);
		rect.bottom = 20;
		rect.top;
		rect.right = 620;
		m_edit1.MoveWindow(rect);
	}
}


HBRUSH CHexEditorDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	if (pWnd->GetDlgCtrlID() == IDC_EDIT9)
	{
		pDC->SetBkColor(RGB(255, 0, 0));
		pDC->SetTextColor(RGB(255, 0, 0));
	}
	return hbr;
}
