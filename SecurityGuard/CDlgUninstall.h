#pragma once


class CDlgUninstall : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgUninstall)

public:
	CDlgUninstall(CWnd* pParent = nullptr);   
	virtual ~CDlgUninstall();

#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_UNINSTALL };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	CListCtrl m_cListCtrl;
	vector<SOFT_INFO> m_vSoftInfo;		
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnNMDblclkListUninstall(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
};
