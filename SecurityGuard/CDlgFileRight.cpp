#include "pch.h"
#include "CSecurityGuard.h"
#include "CDlgFileRight.h"

IMPLEMENT_DYNCREATE(CDlgFileRight, CFormView)

CDlgFileRight::CDlgFileRight()
	: CFormView(IDD_FILEVIEW_RIGHT)
{

}

CDlgFileRight::~CDlgFileRight()
{
}

void CDlgFileRight::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_cListFileRightInfo);
}

BEGIN_MESSAGE_MAP(CDlgFileRight, CFormView)
	ON_WM_SIZE()
END_MESSAGE_MAP()
#ifdef _DEBUG
void CDlgFileRight::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CDlgFileRight::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif 

void CDlgFileRight::OnSize(UINT nType, int cx, int cy)
{
	CFormView::OnSize(nType, cx, cy);

	CRect rc;
	this->GetClientRect(rc);
	CSize size;
	size.cx = rc.right - rc.left;
	size.cy = rc.bottom - rc.top;
	SetScrollSizes(MM_HIMETRIC, size);
	if (m_cListFileRightInfo.m_hWnd)
	{
		m_cListFileRightInfo.MoveWindow(rc);
	}
}


void CDlgFileRight::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();

	m_cListFileRightInfo.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_SUBITEMIMAGES);
	m_cListFileRightInfo.InsertColumn(0, _T("名称"), LVCFMT_LEFT, 200);
	m_cListFileRightInfo.InsertColumn(1, _T("大小"), LVCFMT_LEFT, 200);
	m_cListFileRightInfo.InsertColumn(2, _T("MD5值"), LVCFMT_LEFT, 300);
	m_cListFileRightInfo.InsertColumn(3, _T("修改时间"), LVCFMT_LEFT, 200);
	m_cImageList.Create(16, 16, ILC_COLOR32 | ILC_MASK, 3, 3);
}

void CDlgFileRight::EnumFileInfo(TCHAR* FileFullPath)
{
	m_cListFileRightInfo.DeleteAllItems();
	int iImageCount = m_cImageList.GetImageCount();
	for (int i = 0; i < iImageCount; i++)
	{
		m_cImageList.Remove(0);
	}
	for (size_t i = 0; i < m_FileInfoVec.size(); i++)
	{
		delete m_FileInfoVec[i];
	}
	m_FileInfoVec.clear();
	WxyGetFileInfo(FileFullPath, m_FileInfoVec);
	
	for (size_t i = 0; i < m_FileInfoVec.size(); i++)
	{
		if (-1 == m_cImageList.Add(m_FileInfoVec[i]->FileIcon))
			m_cImageList.Add(AfxGetApp()->LoadIcon(IDI_ICON1));
	}
	m_cListFileRightInfo.SetImageList(&m_cImageList, LVSIL_SMALL);
	for (size_t i = 0; i < m_FileInfoVec.size(); i++)
	{
		TCHAR m_FileSize[MAX_PATH] = { 0 };
		if (m_FileInfoVec[i]->m_FileSize != 0)
		{
			if (m_FileInfoVec[i]->m_FileSize > 0x40000000)
				_stprintf(m_FileSize, TEXT("%.2lfGB"), m_FileInfoVec[i]->m_FileSize / 1024.0 / 1024.0 / 1024.0);
			else if (m_FileInfoVec[i]->m_FileSize > 0x100000)
				_stprintf(m_FileSize, TEXT("%.2lfMB (%dbytes)"), m_FileInfoVec[i]->m_FileSize / 1024.0 / 1024.0, m_FileInfoVec[i]->m_FileSize);
			else
				_stprintf(m_FileSize, TEXT("%.2lfKB (%dbytes)"), m_FileInfoVec[i]->m_FileSize / 1024.0, m_FileInfoVec[i]->m_FileSize);
		}
		m_cListFileRightInfo.InsertItem(i, _T(""), i);
		m_cListFileRightInfo.SetItemText(i, 0, m_FileInfoVec[i]->FileName);
		m_cListFileRightInfo.SetItemText(i, 1, m_FileSize);
		m_cListFileRightInfo.SetItemText(i, 2, m_FileInfoVec[i]->MD5);
		m_cListFileRightInfo.SetItemText(i, 3, m_FileInfoVec[i]->FileLastChangeTime);
	}
}