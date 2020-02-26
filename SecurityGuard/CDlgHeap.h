#pragma once

// CDlgHeap 对话框
#include "Helper.h"
#include <vector>
using namespace std;

VOID EnumHeapHelp(PVOID Param);

class CDlgHeap : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgHeap)

public:
	CDlgHeap(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CDlgHeap();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_HEAP };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	CListCtrl m_cListHeapInfo;
	vector<PHEAP_INFO> m_vHeapInfo;
	VOID EnumHeap(DWORD ProcessID);
	VOID ShowHeapInfo();
	afx_msg void OnSize(UINT nType, int cx, int cy);
};

typedef struct _HEAP_HELPER_
{
	CDlgHeap* m_CDlgHandle;
	DWORD m_ProcessID;
}HEAP_HELPER, * PHEAP_HELPER;