#pragma once
#include "helper.h"
#include <vector>
using namespace std;

// CDlgHandle 对话框

class CDlgHandle : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgHandle)

public:
	CDlgHandle(CWnd* pParent = nullptr);  
	virtual ~CDlgHandle();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_HANDLE };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	CListCtrl m_cListHandleInfo;
	vector<PHANDLE_INFO> m_vHandleInfo;
	VOID EnumHandleInfo(DWORD ProcessID);
	VOID ShowHandleInfo();
};

typedef struct _HANDLE_HELPER_
{
	CDlgHandle* m_CDlgHandle;
	DWORD m_ProcessID;
}HANDLE_HELPER, * PHANDLE_HELPER;

VOID EnumHandleHelp(PVOID Param);