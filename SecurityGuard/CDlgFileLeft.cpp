#include "pch.h"
#include "CSecurityGuard.h"
#include "CDlgFileLeft.h"
#include "CDlgSecurityGuard.h"
#include "CDlgFile.h"

int index = -1;
typedef struct _FILE_LEFT_INFO_
{
	CString strPath;
	CString strTitle;
	HICON hIcon;
}FILE_LEFT_INFO, * PFILE_LEFT_INFO;
IMPLEMENT_DYNCREATE(CDlgFileLeft, CFormView)

CDlgFileLeft::CDlgFileLeft()
	: CFormView(IDD_FILEVIEW_LEFT)
{

}

CDlgFileLeft::~CDlgFileLeft()
{
}

void CDlgFileLeft::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TREE1, m_cFileTree);
}

BEGIN_MESSAGE_MAP(CDlgFileLeft, CFormView)
	ON_WM_SIZE()
	ON_NOTIFY(NM_CLICK, IDC_TREE1, &CDlgFileLeft::OnNMClickTree1)
END_MESSAGE_MAP()

#ifdef _DEBUG
void CDlgFileLeft::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CDlgFileLeft::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif
void CDlgFileLeft::OnSize(UINT nType, int cx, int cy)
{
	CFormView::OnSize(nType, cx, cy);
	CRect rc;
	this->GetClientRect(rc);
	CSize size;
	size.cx = rc.right - rc.left;
	size.cy = rc.bottom - rc.top;
	SetScrollSizes(MM_HIMETRIC, size);
	this->GetClientRect(rc);
	if (m_cFileTree.m_hWnd)
		m_cFileTree.MoveWindow(rc);
}

void CDlgFileLeft::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();
	CFileFind FileFind;

	m_cImageList.Create(16, 16, ILC_COLOR32 | ILC_MASK, 3, 3);
	if (m_cFileTree.m_hWnd)
	{
		TCHAR* FileFullPath = new TCHAR[MAX_PATH];
		memset(FileFullPath, 0, MAX_PATH * sizeof(TCHAR));
		GetLogicalDriveStrings(MAX_PATH, FileFullPath);
		m_cFileTree.SetImageList(&m_cImageList, TVSIL_NORMAL);
		for (TCHAR* s = FileFullPath; *s; s += _tcslen(s) + 1)
		{
			HTREEITEM tree_Temp = m_cFileTree.InsertItem(s, TVI_ROOT, TVI_SORT);
			SHFILEINFO FileInfo = { 0 };
			::SHGetFileInfo(s, 0, &FileInfo, sizeof(SHFILEINFO), SHGFI_ICON);
			m_cImageList.Add(FileInfo.hIcon);
			m_cFileTree.SetItemImage(tree_Temp, index, index++);
			m_cFileTree.SetItemData(tree_Temp, (DWORD_PTR)s);
		}
	}
}

void CDlgFileLeft::ShowFile(CString DirStr, HTREEITEM TreeRoot)
{
	CFileFind FileFind;
	HTREEITEM tree_Temp;

	if (DirStr.Right(1) != _T("\\"))
		DirStr += "\\";
	DirStr += _T("*.*");
	BOOL res = FileFind.FindFile(DirStr);

	vector<FILE_LEFT_INFO> FileLeftInfoVec;
	while (res)
	{
		res = FileFind.FindNextFile();
		CString strPath;
		CString strTitle;
		strPath = FileFind.GetFilePath();
		strTitle = FileFind.GetFileName();
		if (strTitle == _T("..") || strTitle == _T("."))
		{
			continue;
		}
		SHFILEINFO FileInfo = { 0 };
		DWORD_PTR dwRet = ::SHGetFileInfo(strPath, 0, &FileInfo, sizeof(SHFILEINFO), SHGFI_ICON);
		HICON hIcon = FileInfo.hIcon;
		if (dwRet)
		{
			HICON hIcon = FileInfo.hIcon;
		}
		else
		{
			HICON hIcon = NULL;
		}
		FileLeftInfoVec.push_back({ strPath,strTitle,hIcon });
	}
	for (auto it : FileLeftInfoVec)
	{
		m_cImageList.Add(it.hIcon);
	}

	for (size_t i = 0; i < FileLeftInfoVec.size(); i++)
	{
		tree_Temp = TreeRoot;
		tree_Temp = m_cFileTree.InsertItem(FileLeftInfoVec[i].strTitle, index, index++, TreeRoot);
		TCHAR* FileFullPath = new TCHAR[MAX_PATH];
		_tcscpy(FileFullPath, FileLeftInfoVec[i].strPath);
		CString text = m_cFileTree.GetItemText(tree_Temp);
		m_cFileTree.SetItemData(tree_Temp, (DWORD_PTR)FileFullPath);
	}
	FileFind.Close();
}

void CDlgFileLeft::OnNMClickTree1(NMHDR* pNMHDR, LRESULT* pResult)
{
	CPoint oPoint;
	UINT uFlags;
	GetCursorPos(&oPoint); 
	::ScreenToClient(m_cFileTree.m_hWnd, &oPoint);
	HTREEITEM item = m_cFileTree.HitTest(oPoint, &uFlags);
	if (item)
	{
		if (!m_cFileTree.GetChildItem(item))
		{
			TCHAR* path = (TCHAR*)(m_cFileTree.GetItemData(item));
			ShowFile(path, item);
		}
		CDlgSecurityGuard* MainWnd = (CDlgSecurityGuard*)AfxGetMainWnd();
		CDlgFileRight* pView = (CDlgFileRight*)((CDlgFile*)MainWnd->m_cMainTable.m_Dia[2])->m_cSplitter.GetPane(0, 1);
		pView->EnumFileInfo((TCHAR*)m_cFileTree.GetItemData(item));
		m_cFileTree.Expand(item, TVE_TOGGLE);
	}
	*pResult = 0;
}