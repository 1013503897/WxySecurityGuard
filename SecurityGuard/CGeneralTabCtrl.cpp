#include "pch.h"
#include "CSecurityGuard.h"
#include "CGeneralTabCtrl.h"


IMPLEMENT_DYNAMIC(CGeneralTabCtrl, CTabCtrl)


CGeneralTabCtrl::CGeneralTabCtrl()
{

}

CGeneralTabCtrl::~CGeneralTabCtrl()
{
}


BEGIN_MESSAGE_MAP(CGeneralTabCtrl, CTabCtrl)
	ON_NOTIFY_REFLECT(TCN_SELCHANGE, &CGeneralTabCtrl::OnTcnSelchange)
	ON_WM_SIZE()
END_MESSAGE_MAP()


void CGeneralTabCtrl::OnTcnSelchange(NMHDR* pNMHDR, LRESULT* pResult)
{
	*pResult = 0;
	for (auto it : m_Dia)
	{
		it->ShowWindow(SW_HIDE);
	}
	m_Dia[GetCurSel()]->ShowWindow(SW_SHOW);
}

void CGeneralTabCtrl::OnSize(UINT nType, int cx, int cy)
{
	CRect rc;
	GetClientRect(rc);
	rc.DeflateRect(2, 23, 2, 2);
	for (auto it : m_Dia)
	{
		it->MoveWindow(rc);
	}
}