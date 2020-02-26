#pragma once
#include "Helper.h"

// CDlgMsConfig 对话框

class CDlgMsConfig : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgMsConfig)

public:
	CDlgMsConfig(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CDlgMsConfig();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_MSCONFIG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CListCtrl m_cListCtrl;
	afx_msg void OnSize(UINT nType, int cx, int cy);
	virtual BOOL OnInitDialog();
	vector <PMSCONFIG_INFO> m_vMSConfigInfo;
};
