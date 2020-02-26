#pragma once


class CDlgDisasm : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgDisasm)

public:
	CDlgDisasm(CWnd* pParent = nullptr);   
	virtual ~CDlgDisasm();

#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_DISASM2 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    

	DECLARE_MESSAGE_MAP()
public:
	CListCtrl m_cListCode;
	afx_msg void OnBnClickedButtonDisasm();
	CEdit m_cEditHexData;
	virtual BOOL OnInitDialog();
};
char* CharToHex(char* AsciiChar);