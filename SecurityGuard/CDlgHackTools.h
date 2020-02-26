#pragma once

class CDlgHackTools : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgHackTools)

public:
	CDlgHackTools(CWnd* pParent = nullptr);  
	virtual ~CDlgHackTools();

#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_HACKTOOLS };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);   

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedInject();
<<<<<<< HEAD
=======
	afx_msg void OnBnClickedButtonDisasm();
	afx_msg void OnBnClickedButtonBind();
	afx_msg void OnBnClickedButtonSeparate();
	afx_msg void OnBnClickedButtonDriverLoad();
>>>>>>> v1.0
};
