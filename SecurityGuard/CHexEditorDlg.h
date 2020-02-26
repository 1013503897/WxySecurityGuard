#pragma once

#include "HexEdit.h"
class CHexEditorDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CHexEditorDlg)

public:
	CHexEditorDlg(CWnd* pParent = nullptr);   
	virtual ~CHexEditorDlg();

#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG4 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);   

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	CHexEdit m_HexEditor;
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	CEdit m_edit1;
};
