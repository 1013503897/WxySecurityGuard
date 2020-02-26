#pragma once
#include "Helper.h"


class CDlgFileRight : public CFormView
{
	DECLARE_DYNCREATE(CDlgFileRight)

protected:
	CDlgFileRight();         
	virtual ~CDlgFileRight();

public:
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_FILEVIEW_RIGHT };
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
	afx_msg void OnSize(UINT nType, int cx, int cy);
	CListCtrl m_cListFileRightInfo;
	vector<PFILE_INFO> m_FileInfoVec;
	CImageList m_cImageList;
	virtual void OnInitialUpdate();
	void EnumFileInfo(TCHAR* FileFullPath);
};


