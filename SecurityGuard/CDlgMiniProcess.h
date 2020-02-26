#pragma once

class CDlgMiniProcess : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgMiniProcess)

public:
	CDlgMiniProcess(CWnd* pParent = nullptr);   
	virtual ~CDlgMiniProcess();

#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_MINIPROCESS };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);   

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	CListCtrl m_cListProcess;
	afx_msg void OnBnClickedAllCheck();
	vector<PPROCESS_INFO> m_vProcessInfo; 
	vector<PPROCESS_INFO> m_vProcessInfo2;
	void CDlgMiniProcess::OnBnClickedButtonCancle();
	afx_msg void OnBnClickedButtonOK();
	BOOL IsIAT;
	BOOL IsEAT;
	BOOL IsInline;
	CImageList m_cImageList;
};
