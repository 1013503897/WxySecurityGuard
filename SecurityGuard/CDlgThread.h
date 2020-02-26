#pragma once


// CDlgThread 对话框
#include "Helper.h"
using namespace std;
class CDlgThread : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgThread)

public:
	CDlgThread(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CDlgThread();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_THREAD };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CListCtrl m_cListThread;
	vector <PTHREAD_INFO> m_vThreadInfo;
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnNMRClickListThread(NMHDR* pNMHDR, LRESULT* pResult); 
	afx_msg void OnTerminateThread();
	afx_msg void OnResumeThread();
	afx_msg void OnSuspendThread();
	virtual BOOL OnInitDialog();
	VOID EnumThread(DWORD ProcessID);
	VOID ShowThreadInfo();
};
