#include "pch.h"
#include "CSecurityGuard.h"
#include "CDlgFileCleaner.h"
#include "afxdialogex.h"

IMPLEMENT_DYNAMIC(CDlgFileCleaner, CDialogEx)

CDlgFileCleaner::CDlgFileCleaner(CWnd* pParent)
	: CDialogEx(IDD_DIALOG_CLEANER, pParent)
{
}

CDlgFileCleaner::~CDlgFileCleaner()
{
}

void CDlgFileCleaner::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_SUFFIX, m_cEditSuffix);
	DDX_Control(pDX, IDC_EDIT_DELETE1, m_cEditWillDelete);
	DDX_Control(pDX, IDC_EDIT_DELETE2, m_cEditIsDelete);
	DDX_Control(pDX, IDC_MFCSHELLTREE1, m_cShellTree);
	DDX_Control(pDX, IDC_EDIT_PATH, m_cEditFilePath);
	DDX_Control(pDX, IDC_PROGRESS1, m_cProgressFind);
	DDX_Control(pDX, IDC_PROGRESS2, m_cProgressDelete);
}


BEGIN_MESSAGE_MAP(CDlgFileCleaner, CDialogEx)
	ON_WM_SIZE()
	ON_NOTIFY(TVN_SELCHANGED, IDC_MFCSHELLTREE1, &CDlgFileCleaner::OnTvnSelchangedMfcshelltree1)
	ON_BN_CLICKED(IDC_BUTTON1, &CDlgFileCleaner::OnBnClickedButtonEnumFile)
	ON_BN_CLICKED(IDC_BUTTON2, &CDlgFileCleaner::OnBnClickedButtonDeleteFile)
END_MESSAGE_MAP()

BOOL CDlgFileCleaner::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	m_cEditWillDelete.SetLimitText(UINT_MAX);
	m_cEditIsDelete.SetLimitText(UINT_MAX);
	m_cEditSuffix.SetWindowText(_T(".obj\\.tlog\\.lastbuildstate\\.idb\\.pdb\\.pch\\.res\\.ilk\\.exe\\.sdf\\.ipch\\"));
	m_cEditFilePath.SetWindowText(_T("D:\\source"));
	return TRUE;  
}

void CDlgFileCleaner::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);
}


void CDlgFileCleaner::OnTvnSelchangedMfcshelltree1(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	if (m_cShellTree.GetItemPath(m_strFileFullPath, pNMTreeView->itemNew.hItem))
	{
		m_cEditFilePath.SetWindowText(m_strFileFullPath);
	}
	*pResult = 0;
}


void CDlgFileCleaner::OnBnClickedButtonEnumFile()
{
	CString strSuffix;
	m_dwFileDeleteLength = 0;
	int iPos = 0;
	m_cEditSuffix.GetWindowText(strSuffix);
	m_cEditFilePath.GetWindowText(m_strFileFullPath);
	CString strTmp = strSuffix.Tokenize(_T("\\"), iPos);
	while (strTmp.Trim() != _T(""))
	{
		m_vSuffix.push_back(strTmp);
		strTmp = strSuffix.Tokenize(_T("\\"), iPos);
	}
	m_cProgressFind.SetRange(0, GetFileCount(m_strFileFullPath));
	GetDeleteFileName(m_strFileFullPath);
	int a = m_vFileName.size();
	int nLength = m_cEditWillDelete.GetWindowTextLength();
	m_cEditWillDelete.SetSel(nLength, nLength);
	CString FileInfo;
	TCHAR m_FileSize[MAX_PATH] = { 0 };
	if (m_dwFileDeleteLength > 0x40000000)
		_stprintf(m_FileSize, TEXT("%.2lfGB"), m_dwFileDeleteLength / 1024.0 / 1024.0 / 1024.0);
	else if (m_dwFileDeleteLength > 0x100000)
		_stprintf(m_FileSize, TEXT("%.2lfMB (%dbytes)"), m_dwFileDeleteLength / 1024.0 / 1024.0, m_dwFileDeleteLength);
	else
		_stprintf(m_FileSize, TEXT("%.2lfKB (%dbytes)"), m_dwFileDeleteLength / 1024.0, m_dwFileDeleteLength);
	FileInfo.Format(_T("可删除大小:%s，可删除个数：%d个"), m_FileSize, m_vFileName.size());
	m_cEditWillDelete.ReplaceSel(FileInfo);
}

void CDlgFileCleaner::OnBnClickedButtonDeleteFile()
{
	m_cProgressDelete.SetRange(0, m_vFileName.size());
	for (size_t i = 0; i < m_vFileName.size(); i++)
	{
		CString strPath = m_vFileName[i];
		int nLength = m_cEditIsDelete.GetWindowTextLength();
		m_cEditIsDelete.SetSel(nLength, nLength);
		strPath += _T("\r\n");
		m_cEditIsDelete.ReplaceSel(strPath); 
		m_cProgressDelete.OffsetPos(1);
	}
}

void CDlgFileCleaner::GetDeleteFileName(CString FileFullPath)
{
	CFileFind FileFind;
	if (FileFullPath.Right(1) != _T("\\"))
		FileFullPath += "\\";
	FileFullPath += _T("*.*");
	BOOL res = FileFind.FindFile(FileFullPath);

	while (res)
	{
		res = FileFind.FindNextFile();
		CString strPath;
		CString strTitle;
		strPath = FileFind.GetFilePath();
		strTitle = FileFind.GetFileName();
		if (FileFind.IsDots())
		{
			continue;
		}
		else if (FileFind.IsDirectory())
		{
			GetDeleteFileName(strPath);
		}
		else
		{
			m_cProgressFind.OffsetPos(1);
			bool isok = true;
			for (size_t i = 0; i < m_vSuffix.size() && isok; i++)
			{
				if (-1 != strTitle.Find(m_vSuffix[i]))
				{
					m_dwFileDeleteLength += FileFind.GetLength();
					m_vFileName.push_back(strPath);
					int nLength = m_cEditWillDelete.GetWindowTextLength();
					m_cEditWillDelete.SetSel(nLength, nLength);
					strPath += _T("\r\n");
					m_cEditWillDelete.ReplaceSel(strPath);
					isok = false;
				}
			}
		}
	}
}

DWORD CDlgFileCleaner::GetFileCount(CString FileFullPath)
{
	CFileFind FileFind;
	int FileCount = 0;
	if (FileFullPath.Right(1) != _T("\\"))
		FileFullPath += "\\";
	FileFullPath += _T("*.*");
	BOOL res = FileFind.FindFile(FileFullPath);

	while (res)
	{
		res = FileFind.FindNextFile();
		if (FileFind.IsDots())
		{
			continue;
		}
		else if (FileFind.IsDirectory())
		{
			FileCount += GetFileCount(FileFind.GetFilePath());
		}
		else
		{
			FileCount++;
		}
	}
	return FileCount;
}
