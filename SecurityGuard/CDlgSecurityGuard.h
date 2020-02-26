#pragma once
#include "Helper.h"
#include "CDlgModule.h"
#include "CDlgProcess.h"
#include "CGeneralTabCtrl.h"
#include <vector>

using namespace std;

// CDlgSecurityGuard 对话框	`
class CDlgSecurityGuard : public CDialogEx
{
// 构造
public:
	CDlgSecurityGuard(CWnd* pParent = nullptr);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_MAIN };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	CGeneralTabCtrl m_cMainTable;
	CStatusBar m_cStatusBar;
	afx_msg void OnSize(UINT nType, int cx, int cy);
	BOOL InitStatusBar();
	_FILETIME idleTime, kernelTime, userTime;
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	BOOL m_bIsWindowHide = TRUE;
	afx_msg void OnShutDown();
	afx_msg void OnSleep();
	afx_msg void OnReset();
};
