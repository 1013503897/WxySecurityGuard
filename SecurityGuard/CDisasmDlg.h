#pragma once
class CDisasmDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CDisasmDlg)
public:
	CDisasmDlg(CWnd* pParent = nullptr);   
	virtual ~CDisasmDlg();

#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_DISASM };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX); 

	DECLARE_MESSAGE_MAP()
public:
	CListCtrl m_cListCtrl;
	afx_msg void OnBnClickedButtonEnumFile();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	virtual BOOL OnInitDialog();
	CString m_edit_size;
	CString m_edit_offset;
	BOOL m_check;
	CComboBox m_combobox;
	CString m_edit_base;
	void InitEditAddress(int OEP, int OEP_VA);
};