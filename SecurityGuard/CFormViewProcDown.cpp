#include "pch.h"
#include "CSecurityGuard.h"
#include "CFormViewProcDown.h"
#include "Helper.h"
#include "CDlgThread.h"
#include "CDlgModule.h"
#include "CDlgHeap.h"
#include "CDlgHandle.h"
#include "CDlgMemory.h"

IMPLEMENT_DYNCREATE(CFormViewProcDown, CFormView)

CFormViewProcDown::CFormViewProcDown()
	: CFormView(IDD_FORMVIEW_DOWN)
{
}

CFormViewProcDown::~CFormViewProcDown()
{
}

void CFormViewProcDown::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TAB1, m_ProcessDetail);
}

BEGIN_MESSAGE_MAP(CFormViewProcDown, CFormView)
	ON_WM_SIZE()
END_MESSAGE_MAP()

#ifdef _DEBUG
void CFormViewProcDown::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CFormViewProcDown::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif 

void CFormViewProcDown::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();

	m_ProcessDetail.InsertItem(0, _T("线程信息"));
	m_ProcessDetail.InsertItem(1, _T("模块信息"));
	m_ProcessDetail.InsertItem(2, _T("堆信息"));
	m_ProcessDetail.InsertItem(3, _T("内存信息"));
	m_ProcessDetail.InsertItem(4, _T("句柄信息"));
	m_ProcessDetail.m_Dia.push_back(new CDlgThread);
	m_ProcessDetail.m_Dia.push_back(new CDlgModule);
	m_ProcessDetail.m_Dia.push_back(new CDlgHeap);
	m_ProcessDetail.m_Dia.push_back(new CDlgMemory);
	m_ProcessDetail.m_Dia.push_back(new CDlgHandle);
	m_ProcessDetail.m_Dia[0]->Create(IDD_DIALOG_THREAD, &m_ProcessDetail);
	m_ProcessDetail.m_Dia[1]->Create(IDD_DIALOG_MODULE, &m_ProcessDetail);
	m_ProcessDetail.m_Dia[2]->Create(IDD_DIALOG_HEAP, &m_ProcessDetail);
	m_ProcessDetail.m_Dia[3]->Create(IDD_DIALOG_MEMORY, &m_ProcessDetail);
	m_ProcessDetail.m_Dia[4]->Create(IDD_DIALOG_HANDLE, &m_ProcessDetail);
}

void CFormViewProcDown::OnSize(UINT nType, int cx, int cy)
{
	CFormView::OnSize(nType, cx, cy);
	if (m_ProcessDetail.m_hWnd)
	{
		CRect rc;
		this->GetClientRect(rc);
		CSize size;
		size.cx = rc.right - rc.left;
		size.cy = rc.bottom - rc.top; 
		SetScrollSizes(MM_HIMETRIC, size);
		m_ProcessDetail.MoveWindow(rc);
	}
}


VOID CFormViewProcDown::EnumInfo(DWORD ProcessID)
{
	((CDlgThread*)m_ProcessDetail.m_Dia[0])->EnumThread(ProcessID);
	((CDlgModule*)m_ProcessDetail.m_Dia[1])->EnumModule(ProcessID);
	((CDlgHeap*)m_ProcessDetail.m_Dia[2])->EnumHeap(ProcessID);
	((CDlgMemory*)m_ProcessDetail.m_Dia[3])->GetMemoryInfo(ProcessID);
	((CDlgHandle*)m_ProcessDetail.m_Dia[4])->EnumHandleInfo(ProcessID);
}

