#pragma once
#include "CGeneralTabCtrl.h"
class CFormViewProcDown : public CFormView
{
	DECLARE_DYNCREATE(CFormViewProcDown)

protected:
	CFormViewProcDown();          
	virtual ~CFormViewProcDown();

public:
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_FORMVIEW_DOWN };
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
	CGeneralTabCtrl m_ProcessDetail;
	virtual void OnInitialUpdate();
	afx_msg void OnSize(UINT nType, int cx, int cy); 
	VOID EnumInfo(DWORD ProcessID);
};


