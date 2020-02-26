#pragma once

class CDlgFileBind : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgFileBind)

public:
	CDlgFileBind(CWnd* pParent = nullptr);  
	virtual ~CDlgFileBind();

#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_BIND };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);  

	DECLARE_MESSAGE_MAP()
public:
	CEdit m_cEditFileFullPath1;
	CEdit m_cEditFileFullPath2;
	afx_msg void OnBnClickedBind();
	afx_msg void OnBnClickedButtonSelectFile1();
	afx_msg void OnBnClickedButtonSelectFile2();
};
