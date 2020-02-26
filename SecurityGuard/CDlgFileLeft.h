#pragma once
#include "Helper.h"

#include "CDlgFileRight.h"

class CDlgFileLeft : public CFormView
{
	DECLARE_DYNCREATE(CDlgFileLeft)

protected:
	CDlgFileLeft();          
	virtual ~CDlgFileLeft();

public:
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_FILEVIEW_LEFT };
#endif
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    

	DECLARE_MESSAGE_MAP()
public:
	CTreeCtrl m_cFileTree;
	afx_msg void OnSize(UINT nType, int cx, int cy);
	virtual void OnInitialUpdate();
	void ShowFile(CString DirStr, HTREEITEM TreeRoot);
	CImageList m_cImageList;
	afx_msg void OnNMClickTree1(NMHDR* pNMHDR, LRESULT* pResult);
};
