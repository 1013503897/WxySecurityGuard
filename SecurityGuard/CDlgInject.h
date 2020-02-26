#pragma once

class CDlgInject : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgInject)

public:
	CDlgInject(CWnd* pParent = nullptr);   
	virtual ~CDlgInject();

#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_INJECT };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);   
	BOOL ZwCreateThreadExInjectDll(DWORD dwProcessId, char* pszDllFileName);
	BOOL APCInject(DWORD dwProcessId, char* pszDllName);
	BOOL GetAllTidByPid(DWORD dwProcessId, DWORD** ppThreadId, DWORD* pdwThreadIdLength);
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnDropFiles(HDROP hDropInfo);
	afx_msg void OnBnClickedButtonStart();
	CString m_strFilePath;
	int m_nInjectType;
	CString m_PidAndName;
	CString m_TipMsg;
	CEdit m_objEdit;
	afx_msg void OnBnClickedRadio1();
	void SetGlobalHook();
	virtual BOOL OnInitDialog();
};
