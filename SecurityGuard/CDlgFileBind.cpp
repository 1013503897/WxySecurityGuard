#include "pch.h"
#include "CDlgFileBind.h"
#include "afxdialogex.h"
typedef struct _USERDATA_
{
	TCHAR FileName1[MAX_PATH];
	TCHAR FileName2[MAX_PATH];
	DWORD FileSize1;
	DWORD FileSize2;
} USERDATA, * PUSERDATA;


IMPLEMENT_DYNAMIC(CDlgFileBind, CDialogEx)

CDlgFileBind::CDlgFileBind(CWnd* pParent )
	: CDialogEx(IDD_DIALOG_BIND, pParent)
{

}

CDlgFileBind::~CDlgFileBind()
{
}

void CDlgFileBind::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1, m_cEditFileFullPath1);
	DDX_Control(pDX, IDC_EDIT2, m_cEditFileFullPath2);
}

BEGIN_MESSAGE_MAP(CDlgFileBind, CDialogEx)
	ON_BN_CLICKED(IDOK, &CDlgFileBind::OnBnClickedBind)
	ON_BN_CLICKED(IDC_BUTTON1, &CDlgFileBind::OnBnClickedButtonSelectFile1)
	ON_BN_CLICKED(IDC_BUTTON7, &CDlgFileBind::OnBnClickedButtonSelectFile2)
END_MESSAGE_MAP()


void CDlgFileBind::OnBnClickedBind()
{
	CString FileFullPath1, FileFullPath2;
	m_cEditFileFullPath1.GetWindowText(FileFullPath1);
	m_cEditFileFullPath2.GetWindowText(FileFullPath2);
	if (FileFullPath1.IsEmpty() || FileFullPath2.IsEmpty())
	{
		MessageBox(_T("文件路径为空！"));
		return;
	}
	CFileDialog FileDlg(TRUE, _T(".bind"), _T("BindFile"));
	FileDlg.m_ofn.lpstrTitle = _T("Save File");
	if (IDOK == FileDlg.DoModal())
	{
		char* BufferData;
		USERDATA UserData;
		HANDLE hFile1 = INVALID_HANDLE_VALUE;
		HANDLE hFile2 = INVALID_HANDLE_VALUE;
		HANDLE hBindFile = INVALID_HANDLE_VALUE;
		_tcscpy(UserData.FileName1, _tcsrchr(FileFullPath1.GetBuffer() + 1, _T('\\')));
		_tcscpy(UserData.FileName2, _tcsrchr(FileFullPath2.GetBuffer() + 1, _T('\\')));
		hFile1 = CreateFile(FileFullPath1, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, NULL, NULL);
		hFile2 = CreateFile(FileFullPath2, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, NULL, NULL);
		hBindFile = CreateFile(FileDlg.GetPathName(), GENERIC_WRITE, FILE_SHARE_WRITE, NULL, CREATE_ALWAYS, NULL, NULL);

		if (hFile1 == INVALID_HANDLE_VALUE || hFile2 == INVALID_HANDLE_VALUE || hBindFile == INVALID_HANDLE_VALUE)
		{
			goto Exit;
		}

		OVERLAPPED OverLapped;
		ZeroMemory(&OverLapped, sizeof(OVERLAPPED));
		UserData.FileSize1 = GetFileSize(hFile1, NULL);
		UserData.FileSize2 = GetFileSize(hFile2, NULL);
		DWORD dwBindFileSize = UserData.FileSize1 + UserData.FileSize2 + sizeof(USERDATA);
		BufferData = new char[dwBindFileSize] {};
		char* pBuffer = BufferData;
		if (!ReadFile(hFile1, pBuffer, UserData.FileSize1, NULL, NULL))
		{
			goto Exit;
		}
		pBuffer += UserData.FileSize1;
		if (!ReadFile(hFile2, pBuffer, UserData.FileSize2, NULL, NULL))
		{
			goto Exit;
		}
		pBuffer += UserData.FileSize2;
		memcpy(pBuffer, &UserData, sizeof(USERDATA));

		if (!WriteFile(hBindFile, BufferData, dwBindFileSize, NULL, &OverLapped))
		{
			goto Exit;
		}
		delete[] BufferData;
		MessageBox(_T("文件捆绑成功"));
	Exit:
		if (hFile1 != INVALID_HANDLE_VALUE)CloseHandle(hFile1);
		if (hBindFile != INVALID_HANDLE_VALUE)CloseHandle(hBindFile);
		if (hFile2 != INVALID_HANDLE_VALUE)CloseHandle(hFile2);
		CDialogEx::OnOK();
	}
}

void CDlgFileBind::OnBnClickedButtonSelectFile1()
{
	CFileDialog openFileDlg(
		TRUE, NULL, NULL,
		OFN_HIDEREADONLY | OFN_READONLY, NULL, NULL);
	openFileDlg.DoModal();
	m_cEditFileFullPath1.SetWindowText(openFileDlg.GetPathName());
}


void CDlgFileBind::OnBnClickedButtonSelectFile2()
{
	CFileDialog openFileDlg(
		TRUE, NULL, NULL,
		OFN_HIDEREADONLY | OFN_READONLY, NULL, NULL);
	openFileDlg.DoModal(); 
	TCHAR FileFullPath[MAX_PATH]{};
	m_cEditFileFullPath2.SetWindowText(openFileDlg.GetPathName());
}
