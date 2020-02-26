#pragma once


// ResourceInfoDlg 对话框

class ResourceInfoDlg : public CDialogEx
{
	DECLARE_DYNAMIC(ResourceInfoDlg)

public:
	ResourceInfoDlg(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~ResourceInfoDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG2 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CTreeCtrl m_cTreeCtrl;
	afx_msg void OnSize(UINT nType, int cx, int cy);
	CListCtrl m_cListCtrl;
	afx_msg void OnTvnSelchangedTree1(NMHDR* pNMHDR, LRESULT* pResult);
	virtual BOOL OnInitDialog();
};
