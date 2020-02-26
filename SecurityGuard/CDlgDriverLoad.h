#pragma once


// CDlgDriverLoad 对话框

class CDlgDriverLoad : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgDriverLoad)

public:
	CDlgDriverLoad(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CDlgDriverLoad();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_DRIVERLOAD };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonInstall();
	afx_msg void OnBnClickedButtonStart();
	afx_msg void OnBnClickedButtonStop();
	afx_msg void OnBnClickedButtonUnistall();
	afx_msg void OnDropFiles(HDROP hDropInfo);
	TCHAR m_strFilePath[MAX_PATH];
	TCHAR m_strDriverName[MAX_PATH];
	SC_HANDLE m_hServiceDDK;
	SC_HANDLE sh; 
	void Msg(TCHAR* p);
	void Msg(CString msg);
	CListBox m_cListBoxMsg;
};
