#pragma once

class CDlgAPITrace : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgAPITrace)

public:
	CDlgAPITrace(CWnd* pParent = nullptr);   
	virtual ~CDlgAPITrace();

#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_APITRACE };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
	virtual BOOL OnInitDialog();
	CListBox m_cListBoxMsg; 
	afx_msg void OnDropFiles(HDROP hDropInfo);
	CEdit m_cEditFileFullPath;
	afx_msg void OnBnClickedButtonTrace();
};
