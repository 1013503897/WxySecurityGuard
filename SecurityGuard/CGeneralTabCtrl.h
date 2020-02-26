#pragma once

class CGeneralTabCtrl : public CTabCtrl
{
	DECLARE_DYNAMIC(CGeneralTabCtrl)
	vector<CDialogEx*> m_Dia;
public:
	CGeneralTabCtrl();
	virtual ~CGeneralTabCtrl();
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnTcnSelchange(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSize(UINT nType, int cx, int cy);
};


