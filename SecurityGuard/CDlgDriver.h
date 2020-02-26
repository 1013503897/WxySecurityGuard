#pragma once

typedef struct _DRIVER_INFO_
{
	WCHAR BaseDllName[100];
	PVOID DllBase;
	PVOID EntryPoint;
	ULONG SizeOfImage;
	WCHAR FullDllName[260];
}DRIVER_INFO, * PDRIVER_INFO;


class CDlgDriver : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgDriver)

public:
	CDlgDriver(CWnd* pParent = nullptr);  
	virtual ~CDlgDriver();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_DRIVER};
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);   

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	CListCtrl m_cListDriverInfo;
	afx_msg void OnSize(UINT nType, int cx, int cy);
};
