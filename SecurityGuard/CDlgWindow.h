#pragma once
#include <vector>
#include "Helper.h"
using namespace std;

<<<<<<< HEAD
// CDlgWindow 对话框

=======
>>>>>>> v1.0
class CDlgWindow : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgWindow)

public:
	CDlgWindow(CWnd* pParent = nullptr);   
	virtual ~CDlgWindow();

<<<<<<< HEAD
// 对话框数据
=======
>>>>>>> v1.0
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_WINDOW };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	vector<PWND_INFO> m_WndInfoVec;
	CListCtrl m_WndInfo;
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnNMRClickList1(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnCloseWindow();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
};
