#pragma once
#include "Helper.h"
#include <vector>
using namespace std;


// CFormViewProcUp 窗体视图

class CFormViewProcUp : public CFormView
{
	DECLARE_DYNCREATE(CFormViewProcUp)

protected:
	CFormViewProcUp();           // 动态创建所使用的受保护的构造函数
	virtual ~CFormViewProcUp();

public:
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_FORMVIEW_UP };
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
	CListCtrl m_cListProcess;
	CImageList m_cImageList;
	vector <PPROCESS_INFO> m_vProcessInfo;
	virtual void OnInitialUpdate();
	afx_msg void OnSize(UINT nType, int cx, int cy); 
	VOID PrintProcessInfo();
	afx_msg void OnLvnItemchangedList1(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnNMRClickList1(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnPressTmntProc();
};


