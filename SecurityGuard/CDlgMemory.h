#pragma once

class CDlgMemory : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgMemory)

public:
	CDlgMemory(CWnd* pParent = nullptr);   
	virtual ~CDlgMemory();

#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_MEMORY };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);   

	DECLARE_MESSAGE_MAP()
public:
	CListCtrl m_cListMemoryInfo;
	virtual BOOL OnInitDialog();
	VOID GetMemoryInfo(DWORD ProcessID);
	afx_msg void OnSize(UINT nType, int cx, int cy);
};
