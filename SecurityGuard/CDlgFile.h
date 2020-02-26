#pragma once
#include "Helper.h"

class CDlgFile : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgFile)

public:
	CDlgFile(CWnd* pParent = nullptr);  
	virtual ~CDlgFile();
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_FILE };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);   

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	CFrameWnd* m_pMyFrame;
	CSplitterWnd m_cSplitter;
	afx_msg void OnSize(UINT nType, int cx, int cy);
};
