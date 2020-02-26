#pragma once
using namespace std;
class CDlgServer : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgServer)

public:
	CDlgServer(CWnd* pParent = nullptr);  
	virtual ~CDlgServer();

#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_SERVER };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    

	DECLARE_MESSAGE_MAP()
public:
	CListCtrl m_cListCtrl;
	afx_msg void OnSize(UINT nType, int cx, int cy);
	virtual BOOL OnInitDialog();
	vector <PSERVER_INFO> m_ServerInfoVec;
	afx_msg void OnListRClick(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnStratServer();
	afx_msg void OnCloseServer();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
};