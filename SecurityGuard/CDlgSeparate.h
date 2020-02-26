#pragma once

typedef struct _USERDATA_
{
	TCHAR FileName1[MAX_PATH];
	TCHAR FileName2[MAX_PATH];
	DWORD FileSize1;
	DWORD FileSize2;
} USERDATA, * PUSERDATA;

// CDlgSeparate 对话框

class CDlgSeparate : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgSeparate)

public:
	CDlgSeparate(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CDlgSeparate();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_SEPARATE };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonSeparate();
	afx_msg void OnBnClickedOk();
	CEdit m_cEditFilePath;
	afx_msg void OnDropFiles(HDROP hDropInfo); 
	BOOL GetHaveBundledFileInfo(USERDATA& UserData);
};
