// CDlgSeparate.cpp: 实现文件
//

#include "pch.h"
#include "CDlgSeparate.h"
#include "afxdialogex.h"

// CDlgSeparate 对话框

IMPLEMENT_DYNAMIC(CDlgSeparate, CDialogEx)

CDlgSeparate::CDlgSeparate(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_SEPARATE, pParent)
{

}

CDlgSeparate::~CDlgSeparate()
{
}

void CDlgSeparate::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1, m_cEditFilePath);
}


BEGIN_MESSAGE_MAP(CDlgSeparate, CDialogEx)
	ON_BN_CLICKED(IDOK, &CDlgSeparate::OnBnClickedButtonSeparate)
	ON_BN_CLICKED(IDC_BUTTON1, &CDlgSeparate::OnBnClickedOk)
	ON_WM_DROPFILES()
END_MESSAGE_MAP()

BOOL CDlgSeparate::GetHaveBundledFileInfo(USERDATA& UserData)
{
	DWORD FileSize = 0;
	HANDLE ClientFileHandle;

	CString FileFullPath;
	m_cEditFilePath.GetWindowText(FileFullPath);
	ClientFileHandle = CreateFile(FileFullPath, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, NULL, NULL);
	if (ClientFileHandle == INVALID_HANDLE_VALUE)
	{
		return FALSE;
	}
	DWORD BindInfoSize = sizeof(USERDATA);

	FileSize = GetFileSize(ClientFileHandle, NULL);
	DWORD Offset = FileSize - BindInfoSize;

	if (Offset <= 0)
	{
		CloseHandle(ClientFileHandle);
		return FALSE;
	}
	SetFilePointer(ClientFileHandle, Offset, NULL, FILE_BEGIN);
	ReadFile(ClientFileHandle, &UserData, sizeof(USERDATA), NULL, NULL);

	CloseHandle(ClientFileHandle);
	return TRUE;
}

void CDlgSeparate::OnBnClickedButtonSeparate()
{
	char* BufferData;
	USERDATA UserData;
	CString FileFullPath;

	m_cEditFilePath.GetWindowText(FileFullPath);
	if (FileFullPath.IsEmpty())
	{
		MessageBox(_T("文件路径为空！"));
		return;
	}
	HANDLE hBindFile = INVALID_HANDLE_VALUE;
	HANDLE hFile1 = INVALID_HANDLE_VALUE;
	HANDLE hFile2 = INVALID_HANDLE_VALUE;
	if (!GetHaveBundledFileInfo(UserData))
	{
		return;
	}

	hBindFile = CreateFile(FileFullPath, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, NULL, NULL);
	hFile1 = CreateFile(UserData.FileName1, GENERIC_WRITE, FILE_SHARE_WRITE, NULL, CREATE_ALWAYS, NULL, NULL);
	hFile2 = CreateFile(UserData.FileName2, GENERIC_WRITE, FILE_SHARE_WRITE, NULL, CREATE_ALWAYS, NULL, NULL);

	if (hFile1 == INVALID_HANDLE_VALUE || hFile2 == \
		INVALID_HANDLE_VALUE || hBindFile == INVALID_HANDLE_VALUE)
	{
		goto Exit;
	}
	DWORD FileSize = GetFileSize(hBindFile, NULL);
	BufferData = new char[FileSize]{};
	if (!ReadFile(hBindFile, BufferData, FileSize, NULL, NULL))
	{
		goto Exit;
	}
	if (!WriteFile(hFile1, BufferData, UserData.FileSize1, NULL, NULL))
	{
		goto Exit;
	}
	BufferData += UserData.FileSize1;
	if (!WriteFile(hFile2, BufferData, UserData.FileSize2, NULL, NULL))
	{
		goto Exit;
	}
	MessageBox(_T("文件分离成功！"));
Exit:
	if (hFile1 != INVALID_HANDLE_VALUE)
	{
		CloseHandle(hFile1);
	}
	if (hFile2 != INVALID_HANDLE_VALUE)
	{
		CloseHandle(hFile2);
	}
	if (hBindFile != INVALID_HANDLE_VALUE)
	{
		CloseHandle(hBindFile);
	}
}


void CDlgSeparate::OnBnClickedOk()
{
	CFileDialog openFileDlg(
		TRUE, NULL, NULL,
		OFN_HIDEREADONLY | OFN_READONLY, NULL, NULL);
	openFileDlg.DoModal();
	TCHAR FileFullPath[MAX_PATH]{};
	m_cEditFilePath.SetWindowText(openFileDlg.GetPathName());
	CDialogEx::OnOK();
}


void CDlgSeparate::OnDropFiles(HDROP hDropInfo)
{
	TCHAR FileFullPath[MAX_PATH];
	DragQueryFile(hDropInfo, 0, FileFullPath, MAX_PATH);
	m_cEditFilePath.SetWindowText(FileFullPath);
	CDialogEx::OnDropFiles(hDropInfo);
}
