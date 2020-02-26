#pragma once

class CDlgHookDetect : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgHookDetect)

public:
	CDlgHookDetect(CWnd* pParent = nullptr);  
	virtual ~CDlgHookDetect();

#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_HOOKDETECT };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
	CListCtrl m_cListHookDetect;
	vector<PHOOK_INFO> m_vHookInfoVec;
	virtual BOOL OnInitDialog();
<<<<<<< HEAD
	afx_msg void OnBnClickedButton1();
=======
	afx_msg void OnBnClickedButtonDisasm();
>>>>>>> v1.0
	CEdit m_cEditDetecting;
	CProgressCtrl m_cProgressDetecting;
};
