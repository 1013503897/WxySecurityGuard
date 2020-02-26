#pragma once
#include <vector>
using namespace std;
class CDlgFileCleaner : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgFileCleaner)
public:
	CDlgFileCleaner(CWnd* pParent = nullptr);  
	virtual ~CDlgFileCleaner();

#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_CLEANER};
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);  
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	DWORD m_dwFileDeleteLength;
	CEdit m_cEditSuffix;
	CEdit m_cEditWillDelete;
	CEdit m_cEditIsDelete;
	CMFCShellTreeCtrl m_cShellTree;
	CEdit m_cEditFilePath;
	CString m_strFileFullPath;
	afx_msg void OnTvnSelchangedMfcshelltree1(NMHDR* pNMHDR, LRESULT* pResult);
	CProgressCtrl m_cProgressFind;
	CProgressCtrl m_cProgressDelete;
	vector<CString> m_vFileName;
	vector<CString> m_vSuffix;
	afx_msg void OnBnClickedButtonEnumFile();
	afx_msg void OnBnClickedButtonDeleteFile();
	void GetDeleteFileName(CString FileFullPath);
	DWORD GetFileCount(CString FileFullPath);
};
