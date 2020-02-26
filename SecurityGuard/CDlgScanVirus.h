#pragma once

class CDlgScanVirus : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgScanVirus)

public:
	CDlgScanVirus(CWnd* pParent = nullptr);  
	virtual ~CDlgScanVirus();

#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_SCAN };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    
<<<<<<< HEAD

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
=======
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	BOOL m_bCloud;
>>>>>>> v1.0
	CEdit m_cEditFilePath;
	CImageList m_cImageList;
	CString m_strFileFullPath;
	CListCtrl m_cListCtrl;
<<<<<<< HEAD
	CMFCShellTreeCtrl m_cShellTree;
=======
>>>>>>> v1.0
	CProgressCtrl m_cProgressCtrl;
	HANDLE m_hEvent;
	CEdit m_cEditScanningPath;
	SOCKET client;
	int index = 0;
	void ScanFilePath(CString FileFullPath);
	DWORD GetFileCount(CString FileFullPath);
	afx_msg void OnBnClickedButtonUpLoad();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnBnClickedButtonStopScan();
<<<<<<< HEAD
	afx_msg void OnTvnSelchangedMfcshelltree1(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnBnClickedButtonScan();
	afx_msg void OnBnClickedButtonDelete();
=======
	afx_msg void OnBnClickedButtonScan();
	afx_msg void OnBnClickedButtonDelete();
	afx_msg void OnBnClickedButtonSelect();
	BOOL m_bBySignature;
	CButton m_bByMD5;
	afx_msg void OnBnClickedCheckNet();
>>>>>>> v1.0
};
