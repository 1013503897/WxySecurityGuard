#pragma once
#include "Helper.h"

// CDlgProcess 对话框

class CDlgProcess : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgProcess)

public:
	CDlgProcess(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CDlgProcess();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_PROCESS };
#endif

protected:
	virtual BOOL OnInitDialog();
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);	
	CFrameWnd* m_pMyFrame;   
	CSplitterWnd m_cSplitter;    
};

VOID RefreshProcessInfo(CDlgProcess*);