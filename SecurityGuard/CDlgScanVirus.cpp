#include "pch.h"
#include "CSecurityGuard.h"
#include "CDlgScanVirus.h"
#include "afxdialogex.h"
#include "CDlgUploadSample.h"
#include <WinSock2.h>
#include <ws2tcpip.h>
#include "Helper.h"
<<<<<<< HEAD
=======

>>>>>>> v1.0
IMPLEMENT_DYNAMIC(CDlgScanVirus, CDialogEx)

#pragma comment(lib,"ws2_32.lib")

CDlgScanVirus::CDlgScanVirus(CWnd* pParent)
	: CDialogEx(IDD_DIALOG_SCAN, pParent)
<<<<<<< HEAD
{

=======
	, m_bCloud(FALSE)
	, m_bBySignature(FALSE)
{
>>>>>>> v1.0
}

CDlgScanVirus::~CDlgScanVirus()
{
}

void CDlgScanVirus::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1, m_cEditFilePath);
	DDX_Control(pDX, IDC_LIST1, m_cListCtrl);
<<<<<<< HEAD
	DDX_Control(pDX, IDC_MFCSHELLTREE1, m_cShellTree);
	DDX_Control(pDX, IDC_PROGRESS1, m_cProgressCtrl);
	DDX_Control(pDX, IDC_EDIT2, m_cEditScanningPath);
}

BEGIN_MESSAGE_MAP(CDlgScanVirus, CDialogEx)
	ON_NOTIFY(TVN_SELCHANGED, IDC_MFCSHELLTREE1, &CDlgScanVirus::OnTvnSelchangedMfcshelltree1)
=======
	DDX_Control(pDX, IDC_PROGRESS1, m_cProgressCtrl);
	DDX_Control(pDX, IDC_EDIT2, m_cEditScanningPath);
	DDX_Check(pDX, IDC_CHECK1, m_bCloud);
	DDX_Check(pDX, IDC_CHECK3, m_bBySignature);
	DDX_Control(pDX, IDC_CHECK2, m_bByMD5);
}

BEGIN_MESSAGE_MAP(CDlgScanVirus, CDialogEx)
>>>>>>> v1.0
	ON_BN_CLICKED(IDC_BUTTON1, &CDlgScanVirus::OnBnClickedButtonScan)
	ON_BN_CLICKED(IDC_BUTTON3, &CDlgScanVirus::OnBnClickedButtonUpLoad)
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_BUTTON2, &CDlgScanVirus::OnBnClickedButtonStopScan)
	ON_BN_CLICKED(IDC_BUTTON4, &CDlgScanVirus::OnBnClickedButtonDelete)
<<<<<<< HEAD
=======
	ON_BN_CLICKED(IDC_BUTTON5, &CDlgScanVirus::OnBnClickedButtonSelect)
	ON_BN_CLICKED(IDC_CHECK1, &CDlgScanVirus::OnBnClickedCheckNet)
>>>>>>> v1.0
END_MESSAGE_MAP()

BOOL CDlgScanVirus::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	m_cListCtrl.InsertColumn(0, _T("文件路径"), LVCFMT_LEFT, 350);
	m_cListCtrl.InsertColumn(1, _T("md5"), LVCFMT_LEFT, 300);
	m_cListCtrl.InsertColumn(2, _T("样本描述"), LVCFMT_LEFT, 300);
	m_cImageList.Create(16, 16, ILC_COLOR32, 3, 3);
	m_cListCtrl.SetImageList(&m_cImageList, TVSIL_NORMAL);
<<<<<<< HEAD
	m_cShellTree.SetFlags(SHCONTF_NONFOLDERS| SHCONTF_FOLDERS, true);
=======
>>>>>>> v1.0

	WSADATA WsaData = { 0 };
	WSAStartup(0x0202, &WsaData);
	client = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
<<<<<<< HEAD
	sockaddr_in server_addr = { AF_INET };		
	server_addr.sin_port = htons(0x1515);		
	inet_pton(AF_INET, "127.0.0.1", &server_addr.sin_addr);
	connect(client, (sockaddr*)&server_addr, sizeof(server_addr));
=======

>>>>>>> v1.0
	m_hEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
	return TRUE;  
}

<<<<<<< HEAD
void CDlgScanVirus::OnTvnSelchangedMfcshelltree1(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	if (m_cShellTree.GetItemPath(m_strFileFullPath, pNMTreeView->itemNew.hItem))
	{
		m_cEditFilePath.SetWindowText(m_strFileFullPath);
	}
	else
	{
		CString ParentPath;
		m_cShellTree.GetItemPath(ParentPath, m_cShellTree.GetParentItem(pNMTreeView->itemNew.hItem));
		CString FileName = m_cShellTree.GetItemText(pNMTreeView->itemNew.hItem);
		m_strFileFullPath = ParentPath + _T('\\') + FileName;
		m_cEditFilePath.SetWindowText(m_strFileFullPath);
	}
	*pResult = 0;
}

=======
>>>>>>> v1.0
DWORD CDlgScanVirus::GetFileCount(CString FileFullPath)
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
			m_cEditScanningPath.SetWindowText(FileFind.GetFilePath());
			UpdateWindow();
			FileCount++;
		}
	}
	return FileCount;
}

VOID ScanThreadProc(PVOID Param)
{
	CDlgScanVirus* m_Dlg = (CDlgScanVirus*)Param;
	m_Dlg->ScanFilePath(m_Dlg->m_strFileFullPath);
}

void CDlgScanVirus::OnBnClickedButtonScan()
{
	m_cEditFilePath.GetWindowText(m_strFileFullPath);
	m_cProgressCtrl.SetRange(0, GetFileCount(m_strFileFullPath));
	m_cProgressCtrl.SetPos(0);
	m_cListCtrl.DeleteAllItems(); 
	_beginthread(ScanThreadProc, NULL, this);
}

void CDlgScanVirus::ScanFilePath(CString FileFullPath)
{
	CFileFind FileFind;
	if (FileFullPath.Right(1) != _T("\\"))
		FileFullPath += "\\";
	FileFullPath += _T("*.*");
	BOOL res = FileFind.FindFile(FileFullPath);
	while (res)
	{
		if (WAIT_OBJECT_0 == WaitForSingleObject(m_hEvent, 0))
		{
			_endthread();
		}
		else
		{
			res = FileFind.FindNextFile();
			CString strPath = FileFind.GetFilePath();
			if (FileFind.IsDots())
			{
				continue;
			}
			else if (FileFind.IsDirectory())
			{
				ScanFilePath(strPath);
			}
			else
			{
				m_cEditScanningPath.SetWindowText(FileFind.GetFilePath());
				UpdateWindow();
				char* tmpBuf;
				HANDLE pFile;
				DWORD dwBytesRead, dwBytesToRead;
				DWORD dwFileSize;
				pFile = CreateFile(strPath, GENERIC_READ,
					FILE_SHARE_READ,
					NULL,
					OPEN_EXISTING,
					FILE_ATTRIBUTE_NORMAL,
					NULL);
				if (pFile == INVALID_HANDLE_VALUE)
				{
					CloseHandle(pFile);
					pFile = CreateFile(strPath, GENERIC_READ,
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
				USES_CONVERSION;
				char* ch_md5 = T2A(md5);
				char tamp_string[50]{ 0 };
				evp_en_cipher((unsigned char*)ch_md5, (unsigned char*)tamp_string, 32);
				send(client, tamp_string, 40, 0);
				char FileDescription[100] = { 0 };
				recv(client, FileDescription, 100, 0);
				if (strlen(FileDescription) > 0)
				{
					m_cListCtrl.InsertItem(m_cListCtrl.GetItemCount(), strPath);
					m_cListCtrl.SetItemText(m_cListCtrl.GetItemCount() - 1, 1, md5);
					CString des = A2T(FileDescription);
					m_cListCtrl.SetItemText(m_cListCtrl.GetItemCount() - 1, 2, des);
				}
				free(FileAddress);
				m_cProgressCtrl.OffsetPos(1);
				UpdateWindow();
			}
		}
	}
}

void CDlgScanVirus::OnBnClickedButtonUpLoad()
{
<<<<<<< HEAD
	CDlgUpload* UpLoad = new CDlgUpload;
=======
	CDlgUpload* UpLoad = new CDlgUpload; 
	UpdateData(TRUE);
	UpLoad->m_bCloud = m_bCloud;
>>>>>>> v1.0
	UpLoad->DoModal();
}

void CDlgScanVirus::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);
	CRect rc;
<<<<<<< HEAD
	GetClientRect(rc);
	if (m_cShellTree)
	{
		rc.right *= 0.3;
		rc.bottom -= 30;
		m_cShellTree.MoveWindow(rc);
=======
	/*
>>>>>>> v1.0
		GetClientRect(rc);
		rc.right *= 0.3;
		rc.top = rc.bottom - 25;
		m_cEditFilePath.MoveWindow(rc);
		GetClientRect(rc);
		rc.left = rc.right * 0.3+5;
		rc.bottom = 25;
		m_cEditScanningPath.MoveWindow(rc); GetClientRect(rc);
		rc.left = rc.right * 0.3+5;
		rc.top = 30;
		rc.bottom -= 125;
		m_cListCtrl.MoveWindow(rc); 
		GetClientRect(rc);
		rc.left = rc.right * 0.3+5;
		rc.bottom -= 100;
		rc.top = rc.bottom - 25;
		m_cProgressCtrl.MoveWindow(rc);
		GetClientRect(rc);
		rc.left = rc.right * 0.65+2;
		rc.bottom -= 50;
		rc.top = rc.bottom - 50;
		GetDlgItem(IDC_BUTTON1)->MoveWindow(rc); 
		GetClientRect(rc); 
		rc.bottom -= 50;
		rc.top = rc.bottom - 50;
		rc.left = rc.right * 0.3+5;
		rc.right *= 0.65;
		rc.right -= 2;
		GetDlgItem(IDC_BUTTON2)->MoveWindow(rc); 
		GetClientRect(rc);
		rc.left = rc.right * 0.65+2;
		rc.top = rc.bottom - 50;
		GetDlgItem(IDC_BUTTON3)->MoveWindow(rc);
		GetClientRect(rc); 
		rc.left = rc.right * 0.3+5;
		rc.right *= 0.65; 
		rc.right -= 2;
		rc.top = rc.bottom - 50;
<<<<<<< HEAD
		GetDlgItem(IDC_BUTTON4)->MoveWindow(rc);
	}
=======
		GetDlgItem(IDC_BUTTON4)->MoveWindow(rc);*/
>>>>>>> v1.0
}

void CDlgScanVirus::OnBnClickedButtonStopScan()
{
	SetEvent(m_hEvent);
}

void CDlgScanVirus::OnBnClickedButtonDelete()
{
	for (int i = 0; i < m_cListCtrl.GetItemCount(); i++)
	{
		CString FileFullPath = m_cListCtrl.GetItemText(i, 0);
		MessageBox(FileFullPath);
		// DeleteFile(FileFullPath);
	}
}
<<<<<<< HEAD
=======


void CDlgScanVirus::OnBnClickedButtonSelect()
{

	BROWSEINFO  bi;
	bi.hwndOwner = NULL;
	bi.pidlRoot = NULL;
	bi.pszDisplayName = NULL;
	bi.lpszTitle = NULL;
	bi.ulFlags = 0;
	bi.lpfn = NULL;
	bi.iImage = 0;
	LPCITEMIDLIST pidl = SHBrowseForFolder(&bi);
	if (!pidl)
		return;
	TCHAR  szDisplayName[MAX_PATH];
	SHGetPathFromIDList(pidl, szDisplayName);
	m_cEditFilePath.SetWindowText(szDisplayName);
}


void CDlgScanVirus::OnBnClickedCheckNet()
{
	if (!m_bCloud)
	{
		sockaddr_in server_addr = { AF_INET };
		server_addr.sin_port = htons(0x1515);
		inet_pton(AF_INET, "127.0.0.1", &server_addr.sin_addr);
		if (connect(client, (sockaddr*)&server_addr, sizeof(server_addr)) == SOCKET_ERROR)
		{
			m_bCloud = FALSE;
			UpdateData(FALSE);
			MessageBox(_T("服务器连接失败"));
		}
	}
	else
	{
		closesocket(client);
	}
}
>>>>>>> v1.0
