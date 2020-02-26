#include <WinSock2.h>
#include "pch.h"
#include "CSecurityGuard.h"
#include "CDlgUploadSample.h"
#include "CDlgScanVirus.h"
#pragma comment(lib,"ws2_32.lib")
#include <ws2tcpip.h> 
IMPLEMENT_DYNAMIC(CDlgUpload, CDialogEx)

CDlgUpload::CDlgUpload(CWnd* pParent)
	: CDialogEx(IDD_DIALOG_UPLOAD, pParent)
{

}

CDlgUpload::~CDlgUpload()
{
}

void CDlgUpload::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_cListCtrl);
	DDX_Control(pDX, IDC_EDIT1, m_cEditFileFullPath);
	DDX_Control(pDX, IDC_EDIT2, m_cEditFileMD5);
	DDX_Control(pDX, IDC_EDIT3, m_cEditFileDescription);
}

BEGIN_MESSAGE_MAP(CDlgUpload, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON2, &CDlgUpload::OnBnClickedButtonAddFile)
	ON_BN_CLICKED(IDC_BUTTON1, &CDlgUpload::OnBnClickedButtonAddDir)
	ON_COMMAND(ID_32781, &CDlgUpload::OnDeleteItem)
	ON_COMMAND(ID_32782, &CDlgUpload::OnDownload)
	ON_NOTIFY(NM_RCLICK, IDC_LIST1, &CDlgUpload::OnNMRClickList)
END_MESSAGE_MAP()


void CDlgUpload::OnBnClickedButtonAddFile()
{
<<<<<<< HEAD
	CString FileFullPath;
	m_cEditFileFullPath.GetWindowText(FileFullPath);
	HANDLE file = CreateFile(FileFullPath, GENERIC_READ, FILE_SHARE_READ,
		NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	DWORD size = GetFileSize(file, NULL);
	int section_count = size % SECTION_SIZE == 0
		? size / SECTION_SIZE : size / SECTION_SIZE + 1;
	char tmpName[MAX_PATH];
	WideCharToMultiByte(CP_OEMCP, 0, m_strFileName.GetBuffer(), -1, tmpName, MAX_PATH, NULL, FALSE);
	SEND_FILE_INFO file_info;
	strcpy(file_info.name, tmpName);
	file_info.section_count = section_count;
	file_info.file_size = size;
	CString file_description, file_md5;
	m_cEditFileDescription.GetWindowText(file_description);
	m_cEditFileMD5.GetWindowText(file_md5); 
	USES_CONVERSION;
	strcpy(file_info.file_description, T2A(file_description)); 
	strcpy(file_info.file_md5, T2A(file_md5));
	send(client, (char*)&file_info, sizeof(file_info), 0);

	for (int i = 0; i < section_count; ++i)
	{
		FILE_SECTION file_section = { i };
		SetFilePointer(file, SECTION_SIZE * i, 0, FILE_BEGIN);
		ReadFile(file, file_section.data, SECTION_SIZE, &file_section.size, NULL);
		send(client, (char*)&file_section, sizeof(file_section), 0);
	}
	TCHAR t_md5[33]{ 0 };
	TCHAR t_des[100]{ 0 };
	m_cListCtrl.InsertItem(m_cListCtrl.GetItemCount(), file_md5);
	m_cListCtrl.SetItemText(m_cListCtrl.GetItemCount() - 1, 1, file_description);
	CloseHandle(file);
=======
	CString file_description, file_md5;
	m_cEditFileDescription.GetWindowText(file_description);
	m_cEditFileMD5.GetWindowText(file_md5);
	CString FileFullPath;
	m_cEditFileFullPath.GetWindowText(FileFullPath);
	if (_tcslen(file_md5) <= 20)
		return;
	USES_CONVERSION;
	if (m_bCloud)
	{
		HANDLE file = CreateFile(FileFullPath, GENERIC_READ, FILE_SHARE_READ,
			NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
		DWORD size = GetFileSize(file, NULL);
		int section_count = size % SECTION_SIZE == 0
			? size / SECTION_SIZE : size / SECTION_SIZE + 1;
		SEND_FILE_INFO file_info;
		strcpy(file_info.name, T2A(m_strFileName));
		file_info.section_count = section_count;
		file_info.file_size = size;
		strcpy(file_info.file_description, T2A(file_description));
		strcpy(file_info.file_md5, T2A(file_md5));
		send(client, (char*)&file_info, sizeof(file_info), 0);

		for (int i = 0; i < section_count; ++i)
		{
			FILE_SECTION file_section = { i };
			SetFilePointer(file, SECTION_SIZE * i, 0, FILE_BEGIN);
			ReadFile(file, file_section.data, SECTION_SIZE, &file_section.size, NULL);
			send(client, (char*)&file_section, sizeof(file_section), 0);
		}
		CloseHandle(file);
	}
	else
	{
		FILE* fp;
		int index = 0;
		fp = fopen("VirusInfo", "a");
		char line[1000];
		if (fp == NULL)
		{
			return;
		}
		fputc('\n', fp);
		fwrite(T2A(file_md5), sizeof(char), _tcslen(file_md5), fp);
		fputc(' ', fp);
		fwrite(T2A(FileFullPath), sizeof(char), _tcslen(FileFullPath), fp);
		fputc(' ', fp);
		fwrite(T2A(file_description), sizeof(char), _tcslen(file_description), fp);
		fclose(fp);
	}
	m_cListCtrl.InsertItem(m_cListCtrl.GetItemCount(), file_md5);
	m_cListCtrl.SetItemText(m_cListCtrl.GetItemCount() - 1, 1, file_description);
>>>>>>> v1.0
}

void CDlgUpload::OnBnClickedButtonAddDir()
{
	CFileDialog dlgFile(TRUE);
	OPENFILENAME& ofn = dlgFile.GetOFN();
	dlgFile.DoModal(); 
	CString FileFullPath = dlgFile.GetPathName();
	m_strFileName = dlgFile.GetFileName();
	m_cEditFileFullPath.SetWindowText(FileFullPath); 
	char* tmpBuf;
	HANDLE pFile;
	DWORD dwBytesRead, dwBytesToRead;
	DWORD dwFileSize;
	pFile = CreateFile(FileFullPath, GENERIC_READ,
		FILE_SHARE_READ,
		NULL,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL,
		NULL);
	if (pFile == INVALID_HANDLE_VALUE)
	{
		CloseHandle(pFile);
		pFile = CreateFile(FileFullPath, GENERIC_READ,
			FILE_SHARE_READ,
			NULL,
			OPEN_EXISTING,
			FILE_ATTRIBUTE_NORMAL,
			NULL);
		if (pFile == INVALID_HANDLE_VALUE)
		{
			CloseHandle(pFile);
			return;
		}
	}
	dwFileSize = GetFileSize(pFile, NULL);
	char* FileAddress = (char*)malloc(dwFileSize);
	dwBytesToRead = dwFileSize;
	dwBytesRead = 0;
	tmpBuf = FileAddress;
	do {
		ReadFile(pFile, tmpBuf, dwBytesToRead, &dwBytesRead, NULL);
		if (dwBytesRead == 0)
			break;
		dwBytesToRead -= dwBytesRead;
		tmpBuf += dwBytesRead;
	} while (dwBytesToRead > 0);
	CString md5;
	md5_encrypt(FileAddress, dwFileSize, md5);
	m_cEditFileMD5.SetWindowText(md5);
}

BOOL CDlgUpload::OnInitDialog()
{
<<<<<<< HEAD
	CDialogEx::OnInitDialog(); 
	WSADATA WsaData = { 0 };
	WSAStartup(0x0202, &WsaData);
	client = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	sockaddr_in server_addr = { AF_INET };
	server_addr.sin_port = htons(0x1515);
	inet_pton(AF_INET, "127.0.0.1", &server_addr.sin_addr);
	connect(client, (sockaddr*)&server_addr, sizeof(server_addr));
	m_cListCtrl.InsertColumn(0, _T("md5"), LVCFMT_LEFT, 350);
	m_cListCtrl.InsertColumn(1, _T("文件描述"), LVCFMT_LEFT, 300);
	send(client, "selectall", sizeof("selectall"), 0);
	int count = 0;
	recv(client, (char*)&count, 4, 0);
	for (int i = 0; i < count; i++)
	{
		char md5[33]{ 0 };
		char des[100]{ 0 };
		recv(client, md5, 33, 0);
		recv(client, des, 100, 0);
		TCHAR t_md5[33]{ 0 };
		TCHAR t_des[100]{ 0 };
		MultiByteToWideChar(CP_ACP, 0, md5, -1, t_md5, 100);
		MultiByteToWideChar(CP_ACP, 0, des, -1, t_des, 100);
		m_cListCtrl.InsertItem(i, t_md5);
		m_cListCtrl.SetItemText(i, 1, t_des);
	}
=======
	CDialogEx::OnInitDialog();
	m_cListCtrl.InsertColumn(0, _T("md5"), LVCFMT_LEFT, 350);
	m_cListCtrl.InsertColumn(1, _T("文件描述"), LVCFMT_LEFT, 300);
	if (m_bCloud)
	{
		WSADATA WsaData = { 0 };
		WSAStartup(0x0202, &WsaData);
		client = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
		sockaddr_in server_addr = { AF_INET };
		server_addr.sin_port = htons(0x1515);
		inet_pton(AF_INET, "127.0.0.1", &server_addr.sin_addr);
		connect(client, (sockaddr*)&server_addr, sizeof(server_addr));
		send(client, "selectall", sizeof("selectall"), 0);
		int count = 0;
		recv(client, (char*)&count, 4, 0);
		for (int i = 0; i < count; i++)
		{
			char md5[33]{ 0 };
			char des[100]{ 0 };
			recv(client, md5, 33, 0);
			recv(client, des, 100, 0);
			USES_CONVERSION;
			m_cListCtrl.InsertItem(i, A2T(md5));
			m_cListCtrl.SetItemText(i, 1, A2T(des));
		}
	}
	else
	{
		FILE* fp;
		int index = 0;
		fp = fopen("VirusInfo", "r");
		char line[1000];
		if (fp == NULL)
		{
			return FALSE;
		}
		while (!feof(fp))
		{
			fgets(line, 1000, fp);
			char md5[33]{ 0 };
			char filepath[MAX_PATH]{ 0 };
			char des[100]{ 0 };
			sscanf(line, "%[^ ] %[^ ] %s", md5, filepath, des); 
			USES_CONVERSION;
			m_cListCtrl.InsertItem(index, A2T(md5));
			m_cListCtrl.SetItemText(index++, 1, A2T(des));
		}
		fclose(fp);
	}
	
>>>>>>> v1.0
	return TRUE;  
}

void CDlgUpload::OnDeleteItem()
{
	POSITION pos = m_cListCtrl.GetFirstSelectedItemPosition();
<<<<<<< HEAD
	int nItem = m_cListCtrl.GetNextSelectedItem(pos); 
	char del[50] = { "DeleteFile" };
	CString md5 = m_cListCtrl.GetItemText(nItem, 0);
	USES_CONVERSION;
	strcat(del, T2A(md5));
	send(client, del, sizeof(del), 0);
=======
	int nItem = m_cListCtrl.GetNextSelectedItem(pos);
	CString md5 = m_cListCtrl.GetItemText(nItem, 0);
	USES_CONVERSION;
	if (m_bCloud)
	{
		char del[50] = { "DeleteFile" };
		strcat(del, T2A(md5));
		send(client, del, sizeof(del), 0);
	}
	else
	{
		FILE* fin, * fout;
		int counter = 1;
		fin = fopen("VirusInfo", "r");
		fout = fopen("t.tmp", "w");

		if (fin == NULL || fout == NULL) 
		{
			return;
		}
		char line[1000]; 
		char file_md5[33]{ 0 };
		char filepath[MAX_PATH]{ 0 };
		char des[100]{ 0 };
		while (fgets(line, 1000, fin))
		{
			sscanf(line, "%[^ ] %[^ ] %s", file_md5, filepath, des);
			if (strcmp(T2A(md5), file_md5))
			{
				fputs(line, fout);
			}
		}
		fclose(fin);
		fclose(fout);
		remove("VirusInfo");
		rename("t.tmp", "VirusInfo");
	}
>>>>>>> v1.0
	m_cListCtrl.DeleteItem(nItem);
}

bool SelectFilePath(CString& strFilePath)
{
	TCHAR szPathName[MAX_PATH] = { 0 };
	BROWSEINFO bInfo = { 0 };
<<<<<<< HEAD
	bInfo.hwndOwner = GetForegroundWindow(); // 父窗口;
=======
	bInfo.hwndOwner = GetForegroundWindow(); 
>>>>>>> v1.0
	bInfo.lpszTitle = _T("选择目录");
	bInfo.ulFlags = BIF_RETURNONLYFSDIRS | BIF_USENEWUI| BIF_UAHINT;

	LPITEMIDLIST lpDlist;
	lpDlist = SHBrowseForFolder(&bInfo);
<<<<<<< HEAD
	if (nullptr == lpDlist) // 单击了确定按钮;
=======
	if (nullptr == lpDlist) 
>>>>>>> v1.0
	{
		return false;
	}
	SHGetPathFromIDList(lpDlist, szPathName);
	strFilePath = szPathName;
	return true;
}

void CDlgUpload::OnDownload()
{
	CString FileFullPath;
	SelectFilePath(FileFullPath);
	POSITION pos = m_cListCtrl.GetFirstSelectedItemPosition();
	int nItem = m_cListCtrl.GetNextSelectedItem(pos);
	char down[50] = { "DownLoad" };
	CString md5 = m_cListCtrl.GetItemText(nItem, 0);
	USES_CONVERSION;
	strcat(down, T2A(md5));
	char tmpfullpath[MAX_PATH];
	strcpy(tmpfullpath, T2A(FileFullPath));
	send(client, down, sizeof(down), 0);

	SEND_FILE_INFO file_info = { 0 };
	recv(client, (char*)&file_info, sizeof(file_info), 0);
	strcat(tmpfullpath, "\\");
	strcat(tmpfullpath, file_info.name);
	HANDLE file = CreateFileA(tmpfullpath, GENERIC_WRITE, NULL,
		NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	for (int i = 0; i < file_info.section_count; ++i)
	{
		FILE_SECTION file_section = { 0 };
		recv(client, (char*)&file_section, sizeof(file_section), 0);
		SetFilePointer(file, SECTION_SIZE * file_section.index, 0, FILE_BEGIN);
		WriteFile(file, file_section.data, file_section.size, &file_section.size, NULL);
	}
	MessageBoxA(NULL, tmpfullpath, "下载成功", MB_OK);
	CloseHandle(file);
}

void CDlgUpload::OnNMRClickList(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	if (pNMListView->iItem != -1)
	{
		DWORD dwPos = GetMessagePos();
		CPoint point(LOWORD(dwPos), HIWORD(dwPos));
		CMenu menu;
		VERIFY(menu.LoadMenu(IDR_MENU3));
		CMenu* popup = menu.GetSubMenu(0);
		ASSERT(popup != NULL);
		popup->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, point.x, point.y, this);
	}
	*pResult = 0;
}
