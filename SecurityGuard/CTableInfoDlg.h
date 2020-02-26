#pragma once
// TableInfoDlg 对话框

class TableInfoDlg : public CDialogEx
{
	DECLARE_DYNAMIC(TableInfoDlg)

public:
	TableInfoDlg(CWnd* pParent = nullptr);  
	virtual ~TableInfoDlg();

#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG1 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	void ClearListCtrl();
	void ClearDownListCtrl();
	CListCtrl m_UpList;
	CListCtrl m_DownList;
	afx_msg void OnLvnItemchangedList2(NMHDR* pNMHDR, LRESULT* pResult);
};
