#include "pch.h"
#include "CSecurityGuard.h"
#include "CPEInfoDlg.h"
#include "afxdialogex.h"
#include <vector>
#include <algorithm>
#include <strsafe.h>

#define BEA_ENGINE_STATIC
#define BEA_USE_STDCALL

#ifndef _WIN64
#include "BeaEngine_4.1/Win32/headers/BeaEngine.h"
#pragma comment (lib , "BeaEngine_4.1/Win32/Win32/Lib/BeaEngine.lib")
#else
#include "BeaEngine_4.1/Win64/headers/BeaEngine.h"
#pragma comment (lib , "BeaEngine_4.1/Win64/Win64/Lib/BeaEngine.lib")
#endif
#pragma comment(linker, "/NODEFAULTLIB:\"crt.lib\"")



#ifdef _DEBUG
#define new DEBUG_NEW
#endif

using namespace std;

#define  DOWN_LIST m_TableInfoDlg->m_DownList
#define  UP_LIST  m_TableInfoDlg->m_UpList
#define	 RESOURCE_TREE m_ResourceInfoDlg->m_cTreeCtrl;

typedef struct _RESOURCE_DATA
{
<<<<<<< HEAD
	DWORD Type;	// 1 PIMAGE_RESOURCE_DIRECTORY, 2 PIMAGE_RESOURCE_DIRECTORY_ENTRY, 3 IMAGE_RESOURCE_DATA_ENTRY
=======
	DWORD Type;	
>>>>>>> v1.0
	DWORD Address;
}RESOURCE_DATA, * PRESOURCE_DATA;

CPEInfoDlg::CPEInfoDlg(CWnd* pParent)
	: CDialogEx(IDD_PEINFO_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CPEInfoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TREE2, m_cTreeCtrl);
	DDX_Control(pDX, IDC_LIST2, m_cListCtrl);
	DDX_Control(pDX, IDC_EDIT9, m_edit_VA);
	DDX_Control(pDX, IDC_EDIT10, m_edit_RVA);
	DDX_Control(pDX, IDC_EDIT11, m_edit_FOA);
}

BEGIN_MESSAGE_MAP(CPEInfoDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_DROPFILES()
	ON_WM_SIZE()
	ON_WM_MOVE()
	ON_NOTIFY(TVN_SELCHANGED, IDC_TREE2, &CPEInfoDlg::OnTvnSelchangedTree3)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST2, &CPEInfoDlg::OnLvnItemchangedList2)
END_MESSAGE_MAP()

BOOL CPEInfoDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
<<<<<<< HEAD

	ShowWindow(SW_HIDE);
=======
>>>>>>> v1.0
	m_TableInfoDlg = new TableInfoDlg;
	m_TableInfoDlg->Create(IDD_DIALOG_PE_TABLEINFO, this);
	m_ResourceInfoDlg = new ResourceInfoDlg;
	m_ResourceInfoDlg->Create(IDD_DIALOG_RESOURCE, this);
	m_CDisasmDlg = new CDisasmDlg;
	m_CDisasmDlg->Create(IDD_DIALOG_DISASM, this);
	m_HexEditorDlg = new CHexEditorDlg;
	m_HexEditorDlg->Create(IDD_DIALOG_PE_HEXEDITOR, this);
	HICON hIcon[4];
	hIcon[0] = theApp.LoadIcon(IDI_ICON4);
	hIcon[1] = theApp.LoadIcon(IDI_ICON1);
	hIcon[2] = theApp.LoadIcon(IDI_ICON2);
	hIcon[3] = theApp.LoadIcon(IDI_ICON3);
	m_ImageList.Create(16, 16, ILC_COLOR32, 3, 3);
	m_cTreeCtrl.SetImageList(&m_ImageList, TVSIL_NORMAL);
	for (int i = 0; i < 4; i++)
		m_ImageList.Add(hIcon[i]);
	SetWindowText(_T("WXY-PE-INFO"));
	m_cListCtrl.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	HideConvertGroup();

	DWORD ZZY = m_cTreeCtrl.GetStyle();
	ZZY = m_cTreeCtrl.GetExStyle();
	DWORD WXY = m_HexEditorDlg->m_HexEditor.GetStyle();
	WXY = m_HexEditorDlg->m_HexEditor.GetExStyle();
	m_HexEditorDlg->m_HexEditor.ModifyStyle(0, WS_BORDER);
	return TRUE;  
}

HCURSOR CPEInfoDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CPEInfoDlg::OnDropFiles(HDROP hDropInfo)
{
	int DropCount = DragQueryFile(hDropInfo, -1, NULL, 0);

	ZeroMemory(FileName, MAX_PATH * sizeof(TCHAR));
	if (FileAddress)
	{
		free(FileAddress);
		FileAddress = NULL;
	}
	DragQueryFile(hDropInfo, DropCount - 1, FileFullPath, MAX_PATH);
	GetFileFullPath();
	TCHAR* pLast = FileFullPath + _tcslen(FileFullPath);
	for (; pLast >= FileFullPath; pLast--)
	{
		if (*pLast == _T('\\'))
		{
			memcpy(FileName, ++pLast, _tcslen(pLast) * sizeof(TCHAR));
			break;
		}
	}
	CString Title = _T("WXY-PE-INFO  [");
	Title += FileName;
	Title += _T("]");
	SetWindowText(_T(""));
	SetWindowText(Title);
	DragFinish(hDropInfo);
	ClearListCtrl();
	m_cTreeCtrl.DeleteAllItems();
	CString RootTitle = _T("文件:");
	RootTitle += FileName;
	HTREEITEM hRoot = m_cTreeCtrl.InsertItem(RootTitle, 0, 0, TVI_ROOT);
	m_cTreeCtrl.InsertItem(_T("Dos头部"), 2, 2, hRoot);
	HTREEITEM hNtRoot = m_cTreeCtrl.InsertItem(_T("Nt头部"), 2, 2, hRoot);
	m_cTreeCtrl.InsertItem(_T("文件头部"), 2, 2, hNtRoot);
	HTREEITEM hOpRoot = m_cTreeCtrl.InsertItem(_T("可选头部"), 2, 2, hNtRoot);
	m_cTreeCtrl.InsertItem(_T("数据目录"), 2, 2, hOpRoot);
	m_cTreeCtrl.InsertItem(_T("节头部"), 2, 2, hRoot);
	CDialogEx::OnDropFiles(hDropInfo);
	HANDLE pFile;
	
	char* tmpBuf;
	DWORD dwBytesRead, dwBytesToRead;
	pFile = CreateFile(FileFullPath, GENERIC_READ,
		FILE_SHARE_READ,
		NULL,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL,
		NULL);
	if (pFile == INVALID_HANDLE_VALUE)
	{
		CloseHandle(pFile);
		FileFullPath[0] = _T('D');
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

	m_FileSize = GetFileSize(pFile, NULL);

	FileAddress = (char*)malloc(m_FileSize);
	if (FileAddress == NULL)
	{
		CloseHandle(pFile);
	}
	ZeroMemory(FileAddress, m_FileSize);
	dwBytesToRead = m_FileSize;
	dwBytesRead = 0;
	tmpBuf = FileAddress;
	do {
		ReadFile(pFile, tmpBuf, dwBytesToRead, &dwBytesRead, NULL);
		if (dwBytesRead == 0)
			break;
		dwBytesToRead -= dwBytesRead;
		tmpBuf += dwBytesRead;

	} while (dwBytesToRead > 0);
	if (HaveExportTable())
		m_cTreeCtrl.InsertItem(_T("导出目录"), 3, 3, hRoot);
	if (HaveImportTable())
		m_cTreeCtrl.InsertItem(_T("导入目录"), 3, 3, hRoot);
	if (HaveRelocationTable())
		m_cTreeCtrl.InsertItem(_T("重定位目录"), 3, 3, hRoot);
	if (HaveResourceTable())
		m_cTreeCtrl.InsertItem(_T("资源目录"), 3, 3, hRoot);
	if (HaveTLSTable())
		m_cTreeCtrl.InsertItem(_T("TLS目录"), 3, 3, hRoot);
	if (HaveDelayTable())
		m_cTreeCtrl.InsertItem(_T("延迟导入目录"), 3, 3, hRoot);
	m_cTreeCtrl.InsertItem(_T("地址转换"), 1, 1, hRoot);
	m_cTreeCtrl.InsertItem(_T("依赖性分析"), 1, 1, hRoot);
	m_cTreeCtrl.InsertItem(_T("Hex编辑器"), 1, 1, hRoot);
	m_cTreeCtrl.InsertItem(_T("快速反汇编"), 1, 1, hRoot);
	CloseHandle(pFile);
	MyExpandTree(hRoot);

	PrintFileInfo();
}

void CPEInfoDlg::ClearListCtrl()
{
	m_cListCtrl.DeleteAllItems();
	int nColumnCount = m_cListCtrl.GetHeaderCtrl()->GetItemCount();
	for (int i = 0; i < nColumnCount; i++)
	{
		m_cListCtrl.DeleteColumn(0);
	}
}

void CPEInfoDlg::PrintFileInfo()
{
	m_TableInfoDlg->ShowWindow(SW_HIDE);
	m_ResourceInfoDlg->ShowWindow(SW_HIDE);
	m_cListCtrl.ShowWindow(SW_SHOW);HideConvertGroup();
	m_CDisasmDlg->ShowWindow(SW_HIDE);
	m_HexEditorDlg->ShowWindow(SW_HIDE);
	ClearListCtrl();
	m_cListCtrl.InsertColumn(0, _T("属性"), LVCFMT_LEFT, 120);
	m_cListCtrl.InsertColumn(1, _T("值"), LVCFMT_LEFT, 400);
	int index = 0;
	SYSTEMTIME System = { 0 };
	TCHAR CreateTime[MAX_PATH] = { 0 };
	TCHAR ChangeTime[MAX_PATH] = { 0 };
	TCHAR VisitTime[MAX_PATH] = { 0 };
	TCHAR m_FileSize[MAX_PATH] = { 0 };
	WIN32_FIND_DATA stFileData = { 0 };
	HANDLE hFile = FindFirstFile(FileFullPath, &stFileData);
	if (hFile == INVALID_HANDLE_VALUE)
		return;
	FileTimeToSystemTime(&stFileData.ftCreationTime, &System);
	_stprintf(CreateTime, TEXT("%d/%d/%d %d:%d:%d"), System.wYear,
		System.wMonth, System.wDay, System.wHour + 8, System.wMinute, System.wSecond);
	FileTimeToSystemTime(&stFileData.ftLastWriteTime, &System);
	_stprintf(ChangeTime, TEXT("%d/%d/%d %d:%d:%d"), System.wYear,
		System.wMonth, System.wDay, System.wHour + 8, System.wMinute, System.wSecond);
	FileTimeToSystemTime(&stFileData.ftLastAccessTime, &System);
	_stprintf(VisitTime, TEXT("%d/%d/%d %d:%d:%d"), System.wYear,
		System.wMonth, System.wDay, System.wHour + 8, System.wMinute, System.wSecond);

	if (stFileData.nFileSizeLow > 0x40000000)
		_stprintf(m_FileSize, TEXT("%.2lfGB"), stFileData.nFileSizeLow / 1024.0 / 1024.0 / 1024.0);
	else if (stFileData.nFileSizeLow > 0x100000)
		_stprintf(m_FileSize, TEXT("%.2lfMB (%dbytes)"), stFileData.nFileSizeLow / 1024.0 / 1024.0, stFileData.nFileSizeLow);
	else
		_stprintf(m_FileSize, TEXT("%.2lfKB (%dbytes)"), stFileData.nFileSizeLow / 1024.0, stFileData.nFileSizeLow);
	m_cListCtrl.InsertItem(index, _T("文件名"));
	m_cListCtrl.SetItemText(index++, 1, FileName);
	m_cListCtrl.InsertItem(index, _T("完整路径"));
	m_cListCtrl.SetItemText(index++, 1, FileFullPath);
<<<<<<< HEAD
	//m_ListCtrl.InsertItem(index++, _T("文件类型")); 
=======
>>>>>>> v1.0
	DWORD BufferLength = GetFileVersionInfoSize(FileFullPath, NULL);
	char* BufferData;
	BufferData = new char[BufferLength + 1];
	TCHAR FileDescription[0x50] = { _T("UNKOWN") };
	TCHAR CompanyName[0x50] = { _T("UNKOWN") };
	TCHAR FileVersion[0x50] = { _T("UNKOWN") };
	TCHAR LegalCopyright[0x50] = { _T("UNKOWN") };
	TCHAR ProductName[0x50] = { _T("UNKOWN") };
	BOOL IsOk = GetFileVersionInfo(FileFullPath, NULL, BufferLength, BufferData);
	if (IsOk)
	{
		LPVOID lpBuffer = NULL;
		UINT uLen = 0;
		struct LANGANDCODEPAGE {
			WORD wLanguage;
			WORD wCodePage;
		} *lpTranslate;
		VerQueryValue(BufferData,
			TEXT("\\VarFileInfo\\Translation"),
			(LPVOID*)&lpTranslate,
			&uLen);
		for (UINT i = 0; i < (uLen / sizeof(struct LANGANDCODEPAGE)); i++)
		{
			TCHAR SubBlock[0x50];
			StringCchPrintf(SubBlock, 0x50,
				_T("\\StringFileInfo\\%04x%04x\\FileDescription"),
				lpTranslate[i].wLanguage,
				lpTranslate[i].wCodePage);
			IsOk = VerQueryValue(BufferData, SubBlock, &lpBuffer, &uLen);
			if (IsOk)
			{
				memcpy(FileDescription, lpBuffer, 0x50 * sizeof(TCHAR));
			}
			TCHAR SubBlock1[0x50];
			StringCchPrintf(SubBlock1, 0x50,
				_T("\\StringFileInfo\\%04x%04x\\CompanyName"),
				lpTranslate[i].wLanguage,
				lpTranslate[i].wCodePage);
			IsOk = VerQueryValue(BufferData, SubBlock1, &lpBuffer, &uLen);
			if (IsOk)
			{
				memcpy(CompanyName, lpBuffer, 0x50 * sizeof(TCHAR));
			}
			StringCchPrintf(SubBlock1, 0x50,
				_T("\\StringFileInfo\\%04x%04x\\FileVersion"),
				lpTranslate[i].wLanguage,
				lpTranslate[i].wCodePage);
			IsOk = VerQueryValue(BufferData, SubBlock1, &lpBuffer, &uLen);
			if (IsOk)
			{
				memcpy(FileVersion, lpBuffer, 0x50 * sizeof(TCHAR));
			}
			StringCchPrintf(SubBlock1, 0x50,
				_T("\\StringFileInfo\\%04x%04x\\LegalCopyright"),
				lpTranslate[i].wLanguage,
				lpTranslate[i].wCodePage);
			IsOk = VerQueryValue(BufferData, SubBlock1, &lpBuffer, &uLen);
			if (IsOk)
			{
				memcpy(LegalCopyright, lpBuffer, 0x50 * sizeof(TCHAR));
			}
			StringCchPrintf(SubBlock1, 0x50,
				_T("\\StringFileInfo\\%04x%04x\\ProductName"),
				lpTranslate[i].wLanguage,
				lpTranslate[i].wCodePage);
			IsOk = VerQueryValue(BufferData, SubBlock1, &lpBuffer, &uLen);
			if (IsOk)
			{
				memcpy(ProductName, lpBuffer, 0x50 * sizeof(TCHAR));
			}
		}
		delete[]BufferData;
	}
	CString md4;
	CString md5;
	CString sha1;
	m_cListCtrl.InsertItem(index, _T("文件大小"));
	m_cListCtrl.SetItemText(index++, 1, m_FileSize);
<<<<<<< HEAD
	//m_ListCtrl.InsertItem(index++, _T("PE大小"));
=======
>>>>>>> v1.0
	m_cListCtrl.InsertItem(index, _T("创建时间"));
	m_cListCtrl.SetItemText(index++, 1, CreateTime);
	m_cListCtrl.InsertItem(index, _T("修改时间"));
	m_cListCtrl.SetItemText(index++, 1, ChangeTime);
	m_cListCtrl.InsertItem(index, _T("访问时间"));
	m_cListCtrl.SetItemText(index++, 1, VisitTime);
	md5_encrypt(FileAddress, this->m_FileSize, md5);
	sha1_encrypt(FileAddress, this->m_FileSize, sha1);
	md4_encrypt(FileAddress, this->m_FileSize, md4);
	m_cListCtrl.InsertItem(index, _T("MD4"));
	m_cListCtrl.SetItemText(index++, 1, md4);
	m_cListCtrl.InsertItem(index, _T("MD5"));
	m_cListCtrl.SetItemText(index++, 1, md5);
	m_cListCtrl.InsertItem(index, _T("SHA-1"));
	m_cListCtrl.SetItemText(index++, 1, sha1);
	m_cListCtrl.InsertItem(index, _T("FileDescription"));
	m_cListCtrl.SetItemText(index++, 1, (LPCTSTR)FileDescription);
	m_cListCtrl.InsertItem(index, _T("CompanyName"));
	m_cListCtrl.SetItemText(index++, 1, (LPCTSTR)CompanyName);
	m_cListCtrl.InsertItem(index, _T("FileVersion"));
	m_cListCtrl.SetItemText(index++, 1, (LPCTSTR)FileVersion);
	m_cListCtrl.InsertItem(index, _T("LegalCopyright"));
	m_cListCtrl.SetItemText(index++, 1, (LPCTSTR)LegalCopyright);
	m_cListCtrl.InsertItem(index, _T("ProductName"));
	m_cListCtrl.SetItemText(index++, 1, (LPCTSTR)ProductName);
}

void CPEInfoDlg::PrintDosHeader()
{
	m_TableInfoDlg->ShowWindow(SW_HIDE);
	m_cListCtrl.ShowWindow(SW_SHOW);HideConvertGroup();
	m_ResourceInfoDlg->ShowWindow(SW_HIDE);
	m_CDisasmDlg->ShowWindow(SW_HIDE);
	m_HexEditorDlg->ShowWindow(SW_HIDE);
	PIMAGE_DOS_HEADER pDosHeader = (PIMAGE_DOS_HEADER)FileAddress;
	ClearListCtrl();
	m_cListCtrl.InsertColumn(0, _T("成员"), LVCFMT_LEFT, 100);
	m_cListCtrl.InsertColumn(1, _T("偏移量"), LVCFMT_LEFT, 100);
	m_cListCtrl.InsertColumn(2, _T("大小"), LVCFMT_LEFT, 100);
	m_cListCtrl.InsertColumn(3, _T("值"), LVCFMT_LEFT, 100);
	int index = 0;

	m_cListCtrl.InsertItem(index, _T("e_magic"));
	m_cListCtrl.SetItemText(index, 1, Int2CString((DWORD) & (pDosHeader->e_magic) - (DWORD)pDosHeader, 2));
	m_cListCtrl.SetItemText(index, 2, GetVariableSize(sizeof(pDosHeader->e_magic)));
	m_cListCtrl.SetItemText(index++, 3, Int2CString(pDosHeader->e_magic, 4));
	m_cListCtrl.InsertItem(index, _T("e_cblp"));
	m_cListCtrl.SetItemText(index, 1, Int2CString((DWORD) & (pDosHeader->e_cblp) - (DWORD)pDosHeader, 2));
	m_cListCtrl.SetItemText(index, 2, GetVariableSize(sizeof(pDosHeader->e_cblp)));
	m_cListCtrl.SetItemText(index++, 3, Int2CString(pDosHeader->e_cblp, 4));
	m_cListCtrl.InsertItem(index, _T("e_cp"));
	m_cListCtrl.SetItemText(index, 1, Int2CString((DWORD) & (pDosHeader->e_cp) - (DWORD)pDosHeader, 2));
	m_cListCtrl.SetItemText(index, 2, GetVariableSize(sizeof(pDosHeader->e_cp)));
	m_cListCtrl.SetItemText(index, 3, Int2CString(pDosHeader->e_cp, 4));
	m_cListCtrl.InsertItem(index, _T("e_crlc"));
	m_cListCtrl.SetItemText(index, 1, Int2CString((DWORD) & (pDosHeader->e_crlc) - (DWORD)pDosHeader, 2));
	m_cListCtrl.SetItemText(index, 2, GetVariableSize(sizeof(pDosHeader->e_crlc)));
	m_cListCtrl.SetItemText(index++, 3, Int2CString(pDosHeader->e_crlc, 4));
	m_cListCtrl.InsertItem(index, _T("e_cparhdr"));
	m_cListCtrl.SetItemText(index, 1, Int2CString((DWORD) & (pDosHeader->e_cparhdr) - (DWORD)pDosHeader, 2));
	m_cListCtrl.SetItemText(index, 2, GetVariableSize(sizeof(pDosHeader->e_cparhdr)));
	m_cListCtrl.SetItemText(index++, 3, Int2CString(pDosHeader->e_cparhdr, 4));
	m_cListCtrl.InsertItem(index, _T("e_minalloc"));
	m_cListCtrl.SetItemText(index, 1, Int2CString((DWORD) & (pDosHeader->e_minalloc) - (DWORD)pDosHeader, 2));
	m_cListCtrl.SetItemText(index, 2, GetVariableSize(sizeof(pDosHeader->e_minalloc)));
	m_cListCtrl.SetItemText(index++, 3, Int2CString(pDosHeader->e_minalloc, 4));
	m_cListCtrl.InsertItem(index, _T("e_maxalloc"));
	m_cListCtrl.SetItemText(index, 1, Int2CString((DWORD) & (pDosHeader->e_maxalloc) - (DWORD)pDosHeader, 2));
	m_cListCtrl.SetItemText(index, 2, GetVariableSize(sizeof(pDosHeader->e_maxalloc)));
	m_cListCtrl.SetItemText(index++, 3, Int2CString(pDosHeader->e_maxalloc, 4));
	m_cListCtrl.InsertItem(index, _T("e_ss"));
	m_cListCtrl.SetItemText(index, 1, Int2CString((DWORD) & (pDosHeader->e_ss) - (DWORD)pDosHeader, 2));
	m_cListCtrl.SetItemText(index, 2, GetVariableSize(sizeof(pDosHeader->e_ss)));
	m_cListCtrl.SetItemText(index++, 3, Int2CString(pDosHeader->e_ss, 4));
	m_cListCtrl.InsertItem(index, _T("e_sp"));
	m_cListCtrl.SetItemText(index, 1, Int2CString((DWORD) & (pDosHeader->e_sp) - (DWORD)pDosHeader, 2));
	m_cListCtrl.SetItemText(index, 2, GetVariableSize(sizeof(pDosHeader->e_sp)));
	m_cListCtrl.SetItemText(index++, 3, Int2CString(pDosHeader->e_sp, 4));
	m_cListCtrl.InsertItem(index, _T("e_csum"));
	m_cListCtrl.SetItemText(index, 1, Int2CString((DWORD) & (pDosHeader->e_csum) - (DWORD)pDosHeader, 2));
	m_cListCtrl.SetItemText(index, 2, GetVariableSize(sizeof(pDosHeader->e_csum)));
	m_cListCtrl.SetItemText(index++, 3, Int2CString(pDosHeader->e_csum, 4));
	m_cListCtrl.InsertItem(index, _T("e_ip"));
	m_cListCtrl.SetItemText(index, 1, Int2CString((DWORD) & (pDosHeader->e_ip) - (DWORD)pDosHeader, 2));
	m_cListCtrl.SetItemText(index, 2, GetVariableSize(sizeof(pDosHeader->e_ip)));
	m_cListCtrl.SetItemText(index++, 3, Int2CString(pDosHeader->e_ip, 4));
	m_cListCtrl.InsertItem(index, _T("e_cs"));
	m_cListCtrl.SetItemText(index, 1, Int2CString((DWORD) & (pDosHeader->e_cs) - (DWORD)pDosHeader, 2));
	m_cListCtrl.SetItemText(index, 2, GetVariableSize(sizeof(pDosHeader->e_cs)));
	m_cListCtrl.SetItemText(index++, 3, Int2CString(pDosHeader->e_cs, 4));
	m_cListCtrl.InsertItem(index, _T("e_lfarlc"));
	m_cListCtrl.SetItemText(index, 1, Int2CString((DWORD) & (pDosHeader->e_lfarlc) - (DWORD)pDosHeader, 2));
	m_cListCtrl.SetItemText(index, 2, GetVariableSize(sizeof(pDosHeader->e_lfarlc)));
	m_cListCtrl.SetItemText(index++, 3, Int2CString(pDosHeader->e_lfarlc, 4));
	//m_ListCtrl.InsertItem(index, _T("e_res"));
	//m_ListCtrl.SetItemText(index, 1, Int2Cstring((DWORD) & (pDosHeader->e_res) - (DWORD)pDosHeader));
	//m_ListCtrl.SetItemText(index, 2, GetVariableSize(sizeof(pDosHeader->e_res)));
	// m_ListCtrl.SetItemText(index++, 3, Int2Cstring(pDosHeader->e_res));
	m_cListCtrl.InsertItem(index, _T("e_oemid"));
	m_cListCtrl.SetItemText(index, 1, Int2CString((DWORD) & (pDosHeader->e_oemid) - (DWORD)pDosHeader, 2));
	m_cListCtrl.SetItemText(index, 2, GetVariableSize(sizeof(pDosHeader->e_oemid)));
	m_cListCtrl.SetItemText(index++, 3, Int2CString(pDosHeader->e_oemid, 4));
	m_cListCtrl.InsertItem(index, _T("e_oeminfo"));
	m_cListCtrl.SetItemText(index, 1, Int2CString((DWORD) & (pDosHeader->e_oeminfo) - (DWORD)pDosHeader, 2));
	m_cListCtrl.SetItemText(index, 2, GetVariableSize(sizeof(pDosHeader->e_oeminfo)));
	m_cListCtrl.SetItemText(index++, 3, Int2CString(pDosHeader->e_oeminfo, 4));
<<<<<<< HEAD
	//m_ListCtrl.InsertItem(index, _T("e_res2"));
	//m_ListCtrl.SetItemText(index, 1, Int2Cstring((DWORD) & (pDosHeader->e_res2) - (DWORD)pDosHeader));
	//m_ListCtrl.SetItemText(index, 2, GetVariableSize(sizeof(pDosHeader->e_res2)));
	//m_ListCtrl.SetItemText(index++, 3, Int2Cstring(pDosHeader->e_sp));
=======
>>>>>>> v1.0
	m_cListCtrl.InsertItem(index, _T("e_lfanew"));
	m_cListCtrl.SetItemText(index, 1, Int2CString((DWORD) & (pDosHeader->e_lfanew) - (DWORD)pDosHeader, 2));
	m_cListCtrl.SetItemText(index, 2, GetVariableSize(sizeof(pDosHeader->e_lfanew)));
	m_cListCtrl.SetItemText(index++, 3, Int2CString(pDosHeader->e_lfanew, 4));
}

void CPEInfoDlg::PrintNtHeader()
{
	m_TableInfoDlg->ShowWindow(SW_HIDE);
	m_cListCtrl.ShowWindow(SW_SHOW);
	m_ResourceInfoDlg->ShowWindow(SW_HIDE);
	m_CDisasmDlg->ShowWindow(SW_HIDE);
	m_HexEditorDlg->ShowWindow(SW_HIDE);
	PIMAGE_NT_HEADERS pNtHeader = (PIMAGE_NT_HEADERS)(FileAddress + ((PIMAGE_DOS_HEADER)FileAddress)->e_lfanew);
	ClearListCtrl();
	m_cListCtrl.InsertColumn(0, _T("成员"), LVCFMT_LEFT, 100);
	m_cListCtrl.InsertColumn(1, _T("偏移量"), LVCFMT_LEFT, 100);
	m_cListCtrl.InsertColumn(2, _T("大小"), LVCFMT_LEFT, 100);
	m_cListCtrl.InsertColumn(3, _T("值"), LVCFMT_LEFT, 100);

	m_cListCtrl.InsertItem(0, _T("Signature"));
	m_cListCtrl.SetItemText(0, 1, Int2CString((DWORD) & (pNtHeader->Signature) - (DWORD)FileAddress));
	m_cListCtrl.SetItemText(0, 2, GetVariableSize(sizeof(pNtHeader->Signature)));
	m_cListCtrl.SetItemText(0, 3, Int2CString(pNtHeader->Signature));
	int index = 0;
}

CString CPEInfoDlg::GetVariableSize(int size)
{
	switch (size)
	{
		case sizeof(BYTE) :
			return _T("BYTE");
			case  sizeof(WORD) :
				return _T("WORD");
				case sizeof(DWORD) :
					return _T("DWORD");
				default:
					return _T("UNKOWN");
	}
}

void CPEInfoDlg::OnTvnSelchangedTree3(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	HTREEITEM hitem = pNMTreeView->itemNew.hItem;
	CString ItemText = m_cTreeCtrl.GetItemText(pNMTreeView->itemNew.hItem);

	if (ItemText == FileName)
	{
		PrintFileInfo();
	}
	else if (ItemText == _T("Dos头部"))
	{
		PrintDosHeader();
	}
	else if (ItemText == _T("Nt头部"))
	{
		PrintNtHeader();
	}
	else if (ItemText == _T("文件头部"))
	{
		PrintFileHeader();
	}
	else if (ItemText == _T("可选头部"))
	{
		PrintOptionalHeader();
	}
	else if (ItemText == _T("数据目录"))
	{
		PrintDataDirectory();
	}
	else if (ItemText == _T("节头部"))
	{
		PrintSectionHeader();
	}
	else if (ItemText == _T("导出目录"))
	{
		PrintExportTable();
	}
	else if (ItemText == _T("导入目录"))
	{
		PrintImportTable();
	}
	else if (ItemText == _T("重定位目录"))
	{
		PrintRelocationTable();
	}
	else if (ItemText == _T("资源目录"))
	{
		PrintResourceTable();
	}
	else if (ItemText == _T("Hex编辑器"))
	{
		PrintHexEditor();
	}
	else if (ItemText == _T("地址转换"))
	{
		PrintAddressConvert();
	}
	else if (ItemText == _T("快速反汇编"))
	{
		PrintDisasmDlg();
	}
	else if (ItemText == _T("TLS目录"))
	{
		PrintTLSTable();
	}
	else if (ItemText == _T("延迟导入目录"))
	{
		PrintDelayImportTable();
	}
	*pResult = 0;
}

void CPEInfoDlg::PrintFileHeader()
{
	m_TableInfoDlg->ShowWindow(SW_HIDE);
	m_cListCtrl.ShowWindow(SW_SHOW);
	m_ResourceInfoDlg->ShowWindow(SW_HIDE);
	m_CDisasmDlg->ShowWindow(SW_HIDE);
	m_HexEditorDlg->ShowWindow(SW_HIDE);
	PIMAGE_NT_HEADERS pNtHeader = (PIMAGE_NT_HEADERS)(FileAddress + ((PIMAGE_DOS_HEADER)FileAddress)->e_lfanew);
	PIMAGE_FILE_HEADER pFileHeader = (PIMAGE_FILE_HEADER)((DWORD)pNtHeader + sizeof(pNtHeader->Signature));
	ClearListCtrl();
	m_cListCtrl.InsertColumn(0, _T("成员"), LVCFMT_LEFT, 150);
	m_cListCtrl.InsertColumn(1, _T("偏移量"), LVCFMT_LEFT, 100);
	m_cListCtrl.InsertColumn(2, _T("大小"), LVCFMT_LEFT, 100);
	m_cListCtrl.InsertColumn(3, _T("值"), LVCFMT_LEFT, 100);

	int index = 0;
	m_cListCtrl.InsertItem(index, _T("Machine"));
	m_cListCtrl.SetItemText(index, 1, Int2CString((DWORD) & (pFileHeader->Machine) - (DWORD)FileAddress));
	m_cListCtrl.SetItemText(index, 2, GetVariableSize(sizeof(pFileHeader->Machine)));
	m_cListCtrl.SetItemText(index++, 3, Int2CString(pFileHeader->Machine, 8));
	m_cListCtrl.InsertItem(index, _T("NumberOfSections"));
	m_cListCtrl.SetItemText(index, 1, Int2CString((DWORD) & (pFileHeader->NumberOfSections) - (DWORD)FileAddress));
	m_cListCtrl.SetItemText(index, 2, GetVariableSize(sizeof(pFileHeader->NumberOfSections)));
	m_cListCtrl.SetItemText(index++, 3, Int2CString(pFileHeader->NumberOfSections, 8));
	m_cListCtrl.InsertItem(index, _T("TimeDateStamp"));
	m_cListCtrl.SetItemText(index, 1, Int2CString((DWORD) & (pFileHeader->TimeDateStamp) - (DWORD)FileAddress));
	m_cListCtrl.SetItemText(index, 2, GetVariableSize(sizeof(pFileHeader->TimeDateStamp)));
	m_cListCtrl.SetItemText(index++, 3, Int2CString(pFileHeader->TimeDateStamp, 8));
	m_cListCtrl.InsertItem(index, _T("PointerToSymbolTable"));
	m_cListCtrl.SetItemText(index, 1, Int2CString((DWORD) & (pFileHeader->PointerToSymbolTable) - (DWORD)FileAddress));
	m_cListCtrl.SetItemText(index, 2, GetVariableSize(sizeof(pFileHeader->PointerToSymbolTable)));
	m_cListCtrl.SetItemText(index++, 3, Int2CString(pFileHeader->PointerToSymbolTable, 8));
	m_cListCtrl.InsertItem(index, _T("NumberOfSymbols"));
	m_cListCtrl.SetItemText(index, 1, Int2CString((DWORD) & (pFileHeader->NumberOfSymbols) - (DWORD)FileAddress));
	m_cListCtrl.SetItemText(index, 2, GetVariableSize(sizeof(pFileHeader->NumberOfSymbols)));
	m_cListCtrl.SetItemText(index++, 3, Int2CString(pFileHeader->NumberOfSymbols, 8));
	m_cListCtrl.InsertItem(index, _T("SizeOfOptionalHeader"));
	m_cListCtrl.SetItemText(index, 1, Int2CString((DWORD) & (pFileHeader->SizeOfOptionalHeader) - (DWORD)FileAddress));
	m_cListCtrl.SetItemText(index, 2, GetVariableSize(sizeof(pFileHeader->SizeOfOptionalHeader)));
	m_cListCtrl.SetItemText(index++, 3, Int2CString(pFileHeader->SizeOfOptionalHeader, 8));
	m_cListCtrl.InsertItem(index, _T("Characteristics"));
	m_cListCtrl.SetItemText(index, 1, Int2CString((DWORD) & (pFileHeader->Characteristics) - (DWORD)FileAddress));
	m_cListCtrl.SetItemText(index, 2, GetVariableSize(sizeof(pFileHeader->Characteristics)));
	m_cListCtrl.SetItemText(index++, 3, Int2CString(pFileHeader->Characteristics, 8));
}

void CPEInfoDlg::PrintOptionalHeader()
{
	m_TableInfoDlg->ShowWindow(SW_HIDE);
	m_cListCtrl.ShowWindow(SW_SHOW);
	m_ResourceInfoDlg->ShowWindow(SW_HIDE);
	m_CDisasmDlg->ShowWindow(SW_HIDE);
	m_HexEditorDlg->ShowWindow(SW_HIDE);
	PIMAGE_NT_HEADERS pNtHeader = (PIMAGE_NT_HEADERS)(FileAddress + ((PIMAGE_DOS_HEADER)FileAddress)->e_lfanew);
	PIMAGE_OPTIONAL_HEADER pOptionalHeader = (PIMAGE_OPTIONAL_HEADER)((DWORD)pNtHeader + sizeof(pNtHeader->Signature) + sizeof(pNtHeader->FileHeader));
	ClearListCtrl();
	m_cListCtrl.InsertColumn(0, _T("成员"), LVCFMT_LEFT, 180);
	m_cListCtrl.InsertColumn(1, _T("偏移量"), LVCFMT_LEFT, 100);
	m_cListCtrl.InsertColumn(2, _T("大小"), LVCFMT_LEFT, 100);
	m_cListCtrl.InsertColumn(3, _T("值"), LVCFMT_LEFT, 100);

	int index = 0;
	m_cListCtrl.InsertItem(index, _T("Magic"));
	m_cListCtrl.SetItemText(index, 1, Int2CString((DWORD) & (pOptionalHeader->Magic) - (DWORD)FileAddress, 3));
	m_cListCtrl.SetItemText(index, 2, GetVariableSize(sizeof(pOptionalHeader->Magic)));
	m_cListCtrl.SetItemText(index++, 3, Int2CString(pOptionalHeader->Magic, 8));
	m_cListCtrl.InsertItem(index, _T("MajorLinkerVersion"));
	m_cListCtrl.SetItemText(index, 1, Int2CString((DWORD) & (pOptionalHeader->MajorLinkerVersion) - (DWORD)FileAddress, 3));
	m_cListCtrl.SetItemText(index, 2, GetVariableSize(sizeof(pOptionalHeader->MajorLinkerVersion)));
	m_cListCtrl.SetItemText(index++, 3, Int2CString(pOptionalHeader->MajorLinkerVersion, 8));
	m_cListCtrl.InsertItem(index, _T("MinorLinkerVersion"));
	m_cListCtrl.SetItemText(index, 1, Int2CString((DWORD) & (pOptionalHeader->MinorLinkerVersion) - (DWORD)FileAddress, 3));
	m_cListCtrl.SetItemText(index, 2, GetVariableSize(sizeof(pOptionalHeader->MinorLinkerVersion)));
	m_cListCtrl.SetItemText(index++, 3, Int2CString(pOptionalHeader->MinorLinkerVersion, 8));
	m_cListCtrl.InsertItem(index, _T("SizeOfCode"));
	m_cListCtrl.SetItemText(index, 1, Int2CString((DWORD) & (pOptionalHeader->SizeOfCode) - (DWORD)FileAddress, 3));
	m_cListCtrl.SetItemText(index, 2, GetVariableSize(sizeof(pOptionalHeader->SizeOfCode)));
	m_cListCtrl.SetItemText(index++, 3, Int2CString(pOptionalHeader->SizeOfCode, 8));
	m_cListCtrl.InsertItem(index, _T("SizeOfInitializedData"));
	m_cListCtrl.SetItemText(index, 1, Int2CString((DWORD) & (pOptionalHeader->SizeOfInitializedData) - (DWORD)FileAddress, 3));
	m_cListCtrl.SetItemText(index, 2, GetVariableSize(sizeof(pOptionalHeader->SizeOfInitializedData)));
	m_cListCtrl.SetItemText(index++, 3, Int2CString(pOptionalHeader->SizeOfInitializedData, 8));
	m_cListCtrl.InsertItem(index, _T("SizeOfUninitializedData"));
	m_cListCtrl.SetItemText(index, 1, Int2CString((DWORD) & (pOptionalHeader->SizeOfUninitializedData) - (DWORD)FileAddress, 3));
	m_cListCtrl.SetItemText(index, 2, GetVariableSize(sizeof(pOptionalHeader->SizeOfUninitializedData)));
	m_cListCtrl.SetItemText(index++, 3, Int2CString(pOptionalHeader->SizeOfUninitializedData, 8));
	m_cListCtrl.InsertItem(index, _T("AddressOfEntryPoint"));
	m_cListCtrl.SetItemText(index, 1, Int2CString((DWORD) & (pOptionalHeader->AddressOfEntryPoint) - (DWORD)FileAddress, 3));
	m_cListCtrl.SetItemText(index, 2, GetVariableSize(sizeof(pOptionalHeader->AddressOfEntryPoint)));
	m_cListCtrl.SetItemText(index++, 3, Int2CString(pOptionalHeader->AddressOfEntryPoint, 8));
	m_cListCtrl.InsertItem(index, _T("BaseOfCode"));
	m_cListCtrl.SetItemText(index, 1, Int2CString((DWORD) & (pOptionalHeader->BaseOfCode) - (DWORD)FileAddress, 3));
	m_cListCtrl.SetItemText(index, 2, GetVariableSize(sizeof(pOptionalHeader->BaseOfCode)));
	m_cListCtrl.SetItemText(index++, 3, Int2CString(pOptionalHeader->BaseOfCode, 8));
	m_cListCtrl.InsertItem(index, _T("BaseOfData"));
	m_cListCtrl.SetItemText(index, 1, Int2CString((DWORD) & (pOptionalHeader->BaseOfData) - (DWORD)FileAddress, 3));
	m_cListCtrl.SetItemText(index, 2, GetVariableSize(sizeof(pOptionalHeader->BaseOfData)));
	m_cListCtrl.SetItemText(index++, 3, Int2CString(pOptionalHeader->BaseOfData, 8));
	m_cListCtrl.InsertItem(index, _T("ImageBase"));
	m_cListCtrl.SetItemText(index, 1, Int2CString((DWORD) & (pOptionalHeader->ImageBase) - (DWORD)FileAddress, 3));
	m_cListCtrl.SetItemText(index, 2, GetVariableSize(sizeof(pOptionalHeader->ImageBase)));
	m_cListCtrl.SetItemText(index++, 3, Int2CString(pOptionalHeader->ImageBase, 8));
	m_cListCtrl.InsertItem(index, _T("SectionAlignment"));
	m_cListCtrl.SetItemText(index, 1, Int2CString((DWORD) & (pOptionalHeader->SectionAlignment) - (DWORD)FileAddress, 3));
	m_cListCtrl.SetItemText(index, 2, GetVariableSize(sizeof(pOptionalHeader->SectionAlignment)));
	m_cListCtrl.SetItemText(index++, 3, Int2CString(pOptionalHeader->SectionAlignment, 8));
	m_cListCtrl.InsertItem(index, _T("FileAlignment"));
	m_cListCtrl.SetItemText(index, 1, Int2CString((DWORD) & (pOptionalHeader->FileAlignment) - (DWORD)FileAddress, 3));
	m_cListCtrl.SetItemText(index, 2, GetVariableSize(sizeof(pOptionalHeader->FileAlignment)));
	m_cListCtrl.SetItemText(index++, 3, Int2CString(pOptionalHeader->FileAlignment, 8));
	m_cListCtrl.InsertItem(index, _T("MajorOperatingSystemVersion"));
	m_cListCtrl.SetItemText(index, 1, Int2CString((DWORD) & (pOptionalHeader->MajorOperatingSystemVersion) - (DWORD)FileAddress, 3));
	m_cListCtrl.SetItemText(index, 2, GetVariableSize(sizeof(pOptionalHeader->MajorOperatingSystemVersion)));
	m_cListCtrl.SetItemText(index++, 3, Int2CString(pOptionalHeader->MajorOperatingSystemVersion, 8));
	m_cListCtrl.InsertItem(index, _T("MinorOperatingSystemVersion"));
	m_cListCtrl.SetItemText(index, 1, Int2CString((DWORD) & (pOptionalHeader->MinorOperatingSystemVersion) - (DWORD)FileAddress, 3));
	m_cListCtrl.SetItemText(index, 2, GetVariableSize(sizeof(pOptionalHeader->MinorOperatingSystemVersion)));
	m_cListCtrl.SetItemText(index++, 3, Int2CString(pOptionalHeader->MinorOperatingSystemVersion, 8));
	m_cListCtrl.InsertItem(index, _T("Win32VersionValue"));
	m_cListCtrl.SetItemText(index, 1, Int2CString((DWORD) & (pOptionalHeader->Win32VersionValue) - (DWORD)FileAddress, 3));
	m_cListCtrl.SetItemText(index, 2, GetVariableSize(sizeof(pOptionalHeader->Win32VersionValue)));
	m_cListCtrl.SetItemText(index++, 3, Int2CString(pOptionalHeader->Win32VersionValue, 8));
	m_cListCtrl.InsertItem(index, _T("SizeOfImage"));
	m_cListCtrl.SetItemText(index, 1, Int2CString((DWORD) & (pOptionalHeader->SizeOfImage) - (DWORD)FileAddress, 3));
	m_cListCtrl.SetItemText(index, 2, GetVariableSize(sizeof(pOptionalHeader->SizeOfImage)));
	m_cListCtrl.SetItemText(index++, 3, Int2CString(pOptionalHeader->SizeOfImage, 8));
	m_cListCtrl.InsertItem(index, _T("SizeOfHeaders"));
	m_cListCtrl.SetItemText(index, 1, Int2CString((DWORD) & (pOptionalHeader->SizeOfHeaders) - (DWORD)FileAddress, 3));
	m_cListCtrl.SetItemText(index, 2, GetVariableSize(sizeof(pOptionalHeader->SizeOfHeaders)));
	m_cListCtrl.SetItemText(index++, 3, Int2CString(pOptionalHeader->SizeOfHeaders, 8));
	m_cListCtrl.InsertItem(index, _T("CheckSum"));
	m_cListCtrl.SetItemText(index, 1, Int2CString((DWORD) & (pOptionalHeader->CheckSum) - (DWORD)FileAddress, 3));
	m_cListCtrl.SetItemText(index, 2, GetVariableSize(sizeof(pOptionalHeader->CheckSum)));
	m_cListCtrl.SetItemText(index++, 3, Int2CString(pOptionalHeader->CheckSum, 8));
	m_cListCtrl.InsertItem(index, _T("Subsystem"));
	m_cListCtrl.SetItemText(index, 1, Int2CString((DWORD) & (pOptionalHeader->Subsystem) - (DWORD)FileAddress, 3));
	m_cListCtrl.SetItemText(index, 2, GetVariableSize(sizeof(pOptionalHeader->Subsystem)));
	m_cListCtrl.SetItemText(index++, 3, Int2CString(pOptionalHeader->Subsystem, 8));
	m_cListCtrl.InsertItem(index, _T("DllCharacteristics"));
	m_cListCtrl.SetItemText(index, 1, Int2CString((DWORD) & (pOptionalHeader->DllCharacteristics) - (DWORD)FileAddress, 3));
	m_cListCtrl.SetItemText(index, 2, GetVariableSize(sizeof(pOptionalHeader->DllCharacteristics)));
	m_cListCtrl.SetItemText(index++, 3, Int2CString(pOptionalHeader->DllCharacteristics, 8));
	m_cListCtrl.InsertItem(index, _T("SizeOfStackReserve"));
	m_cListCtrl.SetItemText(index, 1, Int2CString((DWORD) & (pOptionalHeader->SizeOfStackReserve) - (DWORD)FileAddress, 3));
	m_cListCtrl.SetItemText(index, 2, GetVariableSize(sizeof(pOptionalHeader->SizeOfStackReserve)));
	m_cListCtrl.SetItemText(index++, 3, Int2CString(pOptionalHeader->SizeOfStackReserve, 8));
	m_cListCtrl.InsertItem(index, _T("SizeOfStackCommit"));
	m_cListCtrl.SetItemText(index, 1, Int2CString((DWORD) & (pOptionalHeader->SizeOfStackCommit) - (DWORD)FileAddress, 3));
	m_cListCtrl.SetItemText(index, 2, GetVariableSize(sizeof(pOptionalHeader->SizeOfStackCommit)));
	m_cListCtrl.SetItemText(index++, 3, Int2CString(pOptionalHeader->SizeOfStackCommit, 8));
	m_cListCtrl.InsertItem(index, _T("SizeOfHeapReserve"));
	m_cListCtrl.SetItemText(index, 1, Int2CString((DWORD) & (pOptionalHeader->SizeOfHeapReserve) - (DWORD)FileAddress, 3));
	m_cListCtrl.SetItemText(index, 2, GetVariableSize(sizeof(pOptionalHeader->SizeOfHeapReserve)));
	m_cListCtrl.SetItemText(index++, 3, Int2CString(pOptionalHeader->SizeOfHeapReserve, 8));
	m_cListCtrl.InsertItem(index, _T("SizeOfHeapCommit"));
	m_cListCtrl.SetItemText(index, 1, Int2CString((DWORD) & (pOptionalHeader->SizeOfHeapCommit) - (DWORD)FileAddress, 3));
	m_cListCtrl.SetItemText(index, 2, GetVariableSize(sizeof(pOptionalHeader->SizeOfHeapCommit)));
	m_cListCtrl.SetItemText(index++, 3, Int2CString(pOptionalHeader->SizeOfHeapCommit, 8));
	m_cListCtrl.InsertItem(index, _T("LoaderFlags"));
	m_cListCtrl.SetItemText(index, 1, Int2CString((DWORD) & (pOptionalHeader->LoaderFlags) - (DWORD)FileAddress, 3));
	m_cListCtrl.SetItemText(index, 2, GetVariableSize(sizeof(pOptionalHeader->LoaderFlags)));
	m_cListCtrl.SetItemText(index++, 3, Int2CString(pOptionalHeader->LoaderFlags, 8));
	m_cListCtrl.InsertItem(index, _T("NumberOfRvaAndSizes"));
	m_cListCtrl.SetItemText(index, 1, Int2CString((DWORD) & (pOptionalHeader->NumberOfRvaAndSizes) - (DWORD)FileAddress, 3));
	m_cListCtrl.SetItemText(index, 2, GetVariableSize(sizeof(pOptionalHeader->NumberOfRvaAndSizes)));
	m_cListCtrl.SetItemText(index++, 3, Int2CString(pOptionalHeader->NumberOfRvaAndSizes, 8));

}

void CPEInfoDlg::PrintSectionHeader()
{
	m_TableInfoDlg->ShowWindow(SW_HIDE);
	m_cListCtrl.ShowWindow(SW_SHOW);
	m_ResourceInfoDlg->ShowWindow(SW_HIDE);
	m_CDisasmDlg->ShowWindow(SW_HIDE);
	m_HexEditorDlg->ShowWindow(SW_HIDE);
	PIMAGE_NT_HEADERS pImageNtHeaders = \
		(PIMAGE_NT_HEADERS)((DWORD)FileAddress + ((PIMAGE_DOS_HEADER)FileAddress)->e_lfanew);
	PIMAGE_FILE_HEADER pImageFileHeader = \
		(PIMAGE_FILE_HEADER)((DWORD)pImageNtHeaders + +sizeof(pImageNtHeaders->Signature));
	PIMAGE_OPTIONAL_HEADER pImageOptionalHeader = \
		(PIMAGE_OPTIONAL_HEADER)((DWORD)pImageFileHeader + sizeof(IMAGE_FILE_HEADER));
	PIMAGE_SECTION_HEADER pImageSectionHeader = \
		(PIMAGE_SECTION_HEADER)((DWORD)pImageOptionalHeader + pImageFileHeader->SizeOfOptionalHeader);
	ClearListCtrl();
	m_cListCtrl.InsertColumn(0, _T("名称"), LVCFMT_LEFT, 100);
	m_cListCtrl.InsertColumn(1, _T("虚拟大小"), LVCFMT_LEFT, 100);
	m_cListCtrl.InsertColumn(2, _T("虚拟地址"), LVCFMT_LEFT, 100);
	m_cListCtrl.InsertColumn(3, _T("Raw大小"), LVCFMT_LEFT, 100);
	m_cListCtrl.InsertColumn(4, _T("Raw地址"), LVCFMT_LEFT, 100);
	for (int i = 0; i < pImageFileHeader->NumberOfSections; i++)
	{
		TCHAR name[9] = { 0 };
#ifdef UNICODE
		MultiByteToWideChar(CP_ACP, 0, (LPCCH)pImageSectionHeader[i].Name, -1, name, 100);
#else
		strcpy(name, pImageSectionHeader[i].Name);
#endif
		m_cListCtrl.InsertItem(i, name);
		m_cListCtrl.SetItemText(i, 1, Int2CString(pImageSectionHeader[i].Misc.VirtualSize, 8));
		m_cListCtrl.SetItemText(i, 2, Int2CString(pImageSectionHeader[i].VirtualAddress, 8));
		m_cListCtrl.SetItemText(i, 3, Int2CString(pImageSectionHeader[i].SizeOfRawData, 8));
		m_cListCtrl.SetItemText(i, 4, Int2CString(pImageSectionHeader[i].PointerToRawData, 8));
	}
}

void CPEInfoDlg::PrintDataDirectory()
{
	m_TableInfoDlg->ShowWindow(SW_HIDE);
	m_cListCtrl.ShowWindow(SW_SHOW);
	HideConvertGroup();
	m_ResourceInfoDlg->ShowWindow(SW_HIDE);
	m_CDisasmDlg->ShowWindow(SW_HIDE);
	m_HexEditorDlg->ShowWindow(SW_HIDE);
	PIMAGE_DOS_HEADER pImageDosHeader = (PIMAGE_DOS_HEADER)FileAddress;
	PIMAGE_FILE_HEADER pImageFileHeader = (PIMAGE_FILE_HEADER)((DWORD)pImageDosHeader + pImageDosHeader->e_lfanew + sizeof(DWORD));
	PIMAGE_OPTIONAL_HEADER pImageOptionalHeader = (PIMAGE_OPTIONAL_HEADER)((DWORD)pImageFileHeader + sizeof(IMAGE_FILE_HEADER));

	TCHAR DirectoryName[][50] = {
		_T("Export Directory RVA"),
		_T("Export Directory Size"),
		_T("Import Directory RVA"),
		_T("Import Directory Size"),
		_T("Resource Directory RVA"),
		_T("Resource Directory Size"),
		_T("Exception Directory RVA"),
		_T("Exception Directory Size"),
		_T("Security Directory RVA"),
		_T("Security Directory Size"),
		_T("Relocation Directory RVA"),
		_T("Relocation Directory Size"),
		_T("Debug Directory RVA"),
		_T("Debug Directory Size"),
		_T("Architecture Directory RVA"),
		_T("Architecture Directory Size"),
		_T("Reserved RVA"),
		_T("Reserved Size"),
		_T("TLS Directory RVA"),
		_T("TLS Directory Size"),
		_T("Configuration Directory RVA"),
		_T("Configuration Directory Size"),
		_T("Bound Import Directory RVA"),
		_T("Bound Import Directory Size"),
		_T("Import Address Table Directory RVA"),
		_T("Import Address Table Directory Size"),
		_T("Delay Import Directory RVA"),
		_T("Delay Import Directory Size"),
		_T(".NET MetaData Directory RVA"),
		_T(".NET MetaData Directory Size")
	};
	ClearListCtrl();
	m_cListCtrl.InsertColumn(0, _T("成员"), LVCFMT_LEFT, 220);
	m_cListCtrl.InsertColumn(1, _T("偏移量"), LVCFMT_LEFT, 100);
	m_cListCtrl.InsertColumn(2, _T("大小"), LVCFMT_LEFT, 100);
	m_cListCtrl.InsertColumn(3, _T("值"), LVCFMT_LEFT, 100);
	m_cListCtrl.InsertColumn(4, _T("段"), LVCFMT_LEFT, 100);
	for (int i = 0; i < 15; i++)
	{
		DWORD ExportDirectory_RVA = pImageOptionalHeader->DataDirectory[i].VirtualAddress;
		int index = 2 * i;
		m_cListCtrl.InsertItem(index, DirectoryName[index]);
		m_cListCtrl.SetItemText(index, 1, Int2CString((DWORD)&pImageOptionalHeader->DataDirectory[i].VirtualAddress - (DWORD)FileAddress, 8));
		m_cListCtrl.SetItemText(index, 2, GetVariableSize(sizeof(pImageOptionalHeader->DataDirectory[i].VirtualAddress, 2)));
		m_cListCtrl.SetItemText(index, 3, Int2CString(pImageOptionalHeader->DataDirectory[i].VirtualAddress, 8));
		TCHAR SectionName[9] = { 0 };
		GetSectionName(pImageOptionalHeader->DataDirectory[i].VirtualAddress, SectionName);
		m_cListCtrl.SetItemText(index, 4, SectionName);
		index++;
		m_cListCtrl.InsertItem(index, DirectoryName[index]);
		m_cListCtrl.SetItemText(index, 1, Int2CString((DWORD)&pImageOptionalHeader->DataDirectory[i].Size - (DWORD)FileAddress, 8));
		m_cListCtrl.SetItemText(index, 2, GetVariableSize(sizeof(pImageOptionalHeader->DataDirectory[i].Size, 2)));
		m_cListCtrl.SetItemText(index, 3, Int2CString(pImageOptionalHeader->DataDirectory[i].Size, 8));
		m_cListCtrl.SetItemText(index, 4, _T(""));
	}
	DWORD ExportDirectory_RVA = pImageOptionalHeader->DataDirectory[0].VirtualAddress;
	PIMAGE_EXPORT_DIRECTORY pImageExportDirectory = (PIMAGE_EXPORT_DIRECTORY)(RVA_TO_FOA(ExportDirectory_RVA) + (DWORD)FileAddress);

}

void CPEInfoDlg::PrintTLSTable()
{
	m_TableInfoDlg->ShowWindow(SW_HIDE);
	m_cListCtrl.ShowWindow(SW_SHOW);
	HideConvertGroup();
	m_ResourceInfoDlg->ShowWindow(SW_HIDE);
	m_CDisasmDlg->ShowWindow(SW_HIDE);
	m_HexEditorDlg->ShowWindow(SW_HIDE);
	PIMAGE_DOS_HEADER pImageDosHeader = (PIMAGE_DOS_HEADER)FileAddress;
	PIMAGE_FILE_HEADER pImageFileHeader = (PIMAGE_FILE_HEADER)((DWORD)pImageDosHeader + pImageDosHeader->e_lfanew + sizeof(DWORD));
	PIMAGE_OPTIONAL_HEADER pImageOptionalHeader = (PIMAGE_OPTIONAL_HEADER)((DWORD)pImageFileHeader + sizeof(IMAGE_FILE_HEADER));

	ClearListCtrl();
	m_cListCtrl.InsertColumn(0, _T("成员"), LVCFMT_LEFT, 220);
	m_cListCtrl.InsertColumn(1, _T("偏移量"), LVCFMT_LEFT, 100);
	m_cListCtrl.InsertColumn(2, _T("大小"), LVCFMT_LEFT, 100);
	m_cListCtrl.InsertColumn(3, _T("值"), LVCFMT_LEFT, 100);

	int index = 0;
	PIMAGE_TLS_DIRECTORY pImageTlsDirectory = (PIMAGE_TLS_DIRECTORY)
		(RVA_TO_FOA(pImageOptionalHeader->DataDirectory[9].VirtualAddress) + (DWORD)FileAddress);
	m_cListCtrl.InsertItem(index,_T("StartAddressOfRawData"));
	m_cListCtrl.SetItemText(index, 1, Int2CString((DWORD)&pImageTlsDirectory->StartAddressOfRawData-(DWORD)FileAddress, 8));
	m_cListCtrl.SetItemText(index, 2, GetVariableSize(sizeof(pImageTlsDirectory->StartAddressOfRawData)));
	m_cListCtrl.SetItemText(index++, 3, Int2CString(pImageTlsDirectory->StartAddressOfRawData, 8));
	m_cListCtrl.InsertItem(index, _T("EndAddressOfRawData"));
	m_cListCtrl.SetItemText(index, 1, Int2CString((DWORD)&pImageTlsDirectory->EndAddressOfRawData - (DWORD)FileAddress, 8));
	m_cListCtrl.SetItemText(index, 2, GetVariableSize(sizeof(pImageTlsDirectory->EndAddressOfRawData)));
	m_cListCtrl.SetItemText(index++, 3, Int2CString(pImageTlsDirectory->EndAddressOfRawData, 8));
	m_cListCtrl.InsertItem(index, _T("AddressOfIndex"));
	m_cListCtrl.SetItemText(index, 1, Int2CString((DWORD)&pImageTlsDirectory->AddressOfIndex - (DWORD)FileAddress, 8));
	m_cListCtrl.SetItemText(index, 2, GetVariableSize(sizeof(pImageTlsDirectory->AddressOfIndex)));
	m_cListCtrl.SetItemText(index++, 3, Int2CString(pImageTlsDirectory->AddressOfIndex, 8));
	m_cListCtrl.InsertItem(index, _T("AddressOfCallBacks"));
	m_cListCtrl.SetItemText(index, 1, Int2CString((DWORD)&pImageTlsDirectory->AddressOfCallBacks - (DWORD)FileAddress, 8));
	m_cListCtrl.SetItemText(index, 2, GetVariableSize(sizeof(pImageTlsDirectory->AddressOfCallBacks)));
	m_cListCtrl.SetItemText(index++, 3, Int2CString(pImageTlsDirectory->AddressOfCallBacks, 8));
	m_cListCtrl.InsertItem(index, _T("SizeOfZeroFill"));
	m_cListCtrl.SetItemText(index, 1, Int2CString((DWORD)&pImageTlsDirectory->SizeOfZeroFill - (DWORD)FileAddress, 8));
	m_cListCtrl.SetItemText(index, 2, GetVariableSize(sizeof(pImageTlsDirectory->SizeOfZeroFill)));
	m_cListCtrl.SetItemText(index++, 3, Int2CString(pImageTlsDirectory->SizeOfZeroFill, 8));
	m_cListCtrl.InsertItem(index, _T("Characteristics"));
	m_cListCtrl.SetItemText(index, 1, Int2CString((DWORD)&pImageTlsDirectory->Characteristics - (DWORD)FileAddress, 8));
	m_cListCtrl.SetItemText(index, 2, GetVariableSize(sizeof(pImageTlsDirectory->Characteristics)));
	m_cListCtrl.SetItemText(index++, 3, Int2CString(pImageTlsDirectory->Characteristics, 8));
}

DWORD CPEInfoDlg::FOA_TO_RVA(DWORD FOA)
{
	DWORD RVA;
	PIMAGE_DOS_HEADER pDosHeader = (PIMAGE_DOS_HEADER)(FileAddress);
	PIMAGE_FILE_HEADER pFileHeader = (PIMAGE_FILE_HEADER)((DWORD)pDosHeader + pDosHeader->e_lfanew + 4);
	PIMAGE_OPTIONAL_HEADER32 pOptionalHeader = (PIMAGE_OPTIONAL_HEADER32)((DWORD)pFileHeader + sizeof(IMAGE_FILE_HEADER));
	PIMAGE_SECTION_HEADER pSectionGroup = (PIMAGE_SECTION_HEADER)((DWORD)pOptionalHeader + pFileHeader->SizeOfOptionalHeader);

	if (FOA < pOptionalHeader->SizeOfHeaders || pOptionalHeader->SectionAlignment == pOptionalHeader->FileAlignment)
	{
		RVA = FOA;
		return RVA;
	}

	for (int i = 0; i < pFileHeader->NumberOfSections; i++)
	{
		if (FOA >= pSectionGroup[i].PointerToRawData && FOA < pSectionGroup[i].PointerToRawData + pSectionGroup[i].SizeOfRawData)
		{
			RVA = pSectionGroup[i].VirtualAddress + FOA - pSectionGroup[i].PointerToRawData;
			return RVA;
		}
	}
	return NULL;
}

DWORD CPEInfoDlg::RVA_TO_FOA(DWORD RVA)
{
	DWORD FOA;
	PIMAGE_DOS_HEADER pImageDosHeader = (PIMAGE_DOS_HEADER)FileAddress;
	PIMAGE_FILE_HEADER pImageFileHeader = (PIMAGE_FILE_HEADER)((DWORD)FileAddress + 4 + pImageDosHeader->e_lfanew);
	PIMAGE_OPTIONAL_HEADER pImageOptionalHeader = (PIMAGE_OPTIONAL_HEADER)((DWORD)pImageFileHeader + sizeof(IMAGE_FILE_HEADER));
	PIMAGE_SECTION_HEADER pSectionGroup = (PIMAGE_SECTION_HEADER)((DWORD)pImageOptionalHeader + pImageFileHeader->SizeOfOptionalHeader);

	if (RVA < pImageOptionalHeader->SizeOfHeaders || pImageOptionalHeader->FileAlignment == pImageOptionalHeader->SectionAlignment)
	{
		FOA = RVA;
		return FOA;
	}

	for (int i = 0; i < pImageFileHeader->NumberOfSections; i++)
	{
		if (RVA >= pSectionGroup[i].VirtualAddress && RVA < pSectionGroup[i].VirtualAddress + pSectionGroup[i].Misc.VirtualSize)
		{
			FOA = pSectionGroup[i].PointerToRawData + RVA - pSectionGroup[i].VirtualAddress;
			return FOA;
		}
	}
	return NULL;
}

void CPEInfoDlg::GetSectionName(DWORD RVA, TCHAR* Name)
{
	PIMAGE_DOS_HEADER pImageDosHeader = (PIMAGE_DOS_HEADER)FileAddress;
	PIMAGE_FILE_HEADER pImageFileHeader = (PIMAGE_FILE_HEADER)((DWORD)FileAddress + 4 + pImageDosHeader->e_lfanew);
	PIMAGE_OPTIONAL_HEADER pImageOptionalHeader = (PIMAGE_OPTIONAL_HEADER)((DWORD)pImageFileHeader + sizeof(IMAGE_FILE_HEADER));
	PIMAGE_SECTION_HEADER pSectionGroup = (PIMAGE_SECTION_HEADER)((DWORD)pImageOptionalHeader + pImageFileHeader->SizeOfOptionalHeader);
	for (int i = 0; i < pImageFileHeader->NumberOfSections; i++)
	{
		if (RVA >= pSectionGroup[i].VirtualAddress && RVA < pSectionGroup[i].VirtualAddress + pSectionGroup[i].Misc.VirtualSize)
		{
#ifdef UNICODE
			MultiByteToWideChar(CP_ACP, 0, (LPCCH)pSectionGroup[i].Name, -1, Name, 100);
#else
			strcpy(Name, pSectionGroup[i].Name);
#endif
			break;
		}
	}
}

BOOL CPEInfoDlg::HaveExportTable()
{
	PIMAGE_DOS_HEADER pImageDosHeader = (PIMAGE_DOS_HEADER)FileAddress;
	PIMAGE_FILE_HEADER pImageFileHeader = (PIMAGE_FILE_HEADER)((DWORD)pImageDosHeader + pImageDosHeader->e_lfanew + sizeof(DWORD));
	PIMAGE_OPTIONAL_HEADER pImageOptionalHeader = (PIMAGE_OPTIONAL_HEADER)((DWORD)pImageFileHeader + sizeof(IMAGE_FILE_HEADER));

	return pImageOptionalHeader->DataDirectory[0].VirtualAddress && pImageOptionalHeader->DataDirectory[0].Size;
}

BOOL CPEInfoDlg::HaveImportTable()
{
	PIMAGE_DOS_HEADER pImageDosHeader = (PIMAGE_DOS_HEADER)FileAddress;
	PIMAGE_FILE_HEADER pImageFileHeader = (PIMAGE_FILE_HEADER)((DWORD)pImageDosHeader + pImageDosHeader->e_lfanew + sizeof(DWORD));
	PIMAGE_OPTIONAL_HEADER pImageOptionalHeader = (PIMAGE_OPTIONAL_HEADER)((DWORD)pImageFileHeader + sizeof(IMAGE_FILE_HEADER));

	return pImageOptionalHeader->DataDirectory[1].VirtualAddress && pImageOptionalHeader->DataDirectory[1].Size;
}

BOOL CPEInfoDlg::HaveRelocationTable()
{
	PIMAGE_DOS_HEADER pImageDosHeader = (PIMAGE_DOS_HEADER)FileAddress;
	PIMAGE_FILE_HEADER pImageFileHeader = (PIMAGE_FILE_HEADER)((DWORD)pImageDosHeader + pImageDosHeader->e_lfanew + sizeof(DWORD));
	PIMAGE_OPTIONAL_HEADER pImageOptionalHeader = (PIMAGE_OPTIONAL_HEADER)((DWORD)pImageFileHeader + sizeof(IMAGE_FILE_HEADER));

	return pImageOptionalHeader->DataDirectory[5].VirtualAddress && pImageOptionalHeader->DataDirectory[5].Size;
}

BOOL CPEInfoDlg::HaveResourceTable()
{
	PIMAGE_DOS_HEADER pImageDosHeader = (PIMAGE_DOS_HEADER)FileAddress;
	PIMAGE_FILE_HEADER pImageFileHeader = (PIMAGE_FILE_HEADER)((DWORD)pImageDosHeader + pImageDosHeader->e_lfanew + sizeof(DWORD));
	PIMAGE_OPTIONAL_HEADER pImageOptionalHeader = (PIMAGE_OPTIONAL_HEADER)((DWORD)pImageFileHeader + sizeof(IMAGE_FILE_HEADER));

	return pImageOptionalHeader->DataDirectory[2].VirtualAddress && pImageOptionalHeader->DataDirectory[2].Size;
}

BOOL CPEInfoDlg::HaveTLSTable()
{
	PIMAGE_DOS_HEADER pImageDosHeader = (PIMAGE_DOS_HEADER)FileAddress;
	PIMAGE_FILE_HEADER pImageFileHeader = (PIMAGE_FILE_HEADER)((DWORD)pImageDosHeader + pImageDosHeader->e_lfanew + sizeof(DWORD));
	PIMAGE_OPTIONAL_HEADER pImageOptionalHeader = (PIMAGE_OPTIONAL_HEADER)((DWORD)pImageFileHeader + sizeof(IMAGE_FILE_HEADER));

	return pImageOptionalHeader->DataDirectory[9].VirtualAddress && pImageOptionalHeader->DataDirectory[2].Size;
}

BOOL CPEInfoDlg::HaveDelayTable()
{
	PIMAGE_DOS_HEADER pImageDosHeader = (PIMAGE_DOS_HEADER)FileAddress;
	PIMAGE_FILE_HEADER pImageFileHeader = (PIMAGE_FILE_HEADER)((DWORD)pImageDosHeader + pImageDosHeader->e_lfanew + sizeof(DWORD));
	PIMAGE_OPTIONAL_HEADER pImageOptionalHeader = (PIMAGE_OPTIONAL_HEADER)((DWORD)pImageFileHeader + sizeof(IMAGE_FILE_HEADER));

	return pImageOptionalHeader->DataDirectory[13].VirtualAddress && pImageOptionalHeader->DataDirectory[2].Size;
}

void CPEInfoDlg::GetFileFullPath()
{
	if (_tcscmp(&FileFullPath[_tcslen(FileFullPath) - 4], _T(".lnk")))
	{
		return;
	}
	::CoInitialize(NULL);
	IShellLink* psl = NULL;
	HRESULT hr = CoCreateInstance(CLSID_ShellLink, NULL, CLSCTX_INPROC_SERVER, IID_IShellLink, (LPVOID*)&psl);
	if (SUCCEEDED(hr))
	{
		IPersistFile* ppf;
		hr = psl->QueryInterface(IID_IPersistFile, (LPVOID*)&ppf);
		if (SUCCEEDED(hr))
		{
			hr = ppf->Load(FileFullPath, STGM_READ);    //加载文件

			if (SUCCEEDED(hr))
			{
				WIN32_FIND_DATA wfd;
				psl->GetPath(FileFullPath, MAX_PATH, (WIN32_FIND_DATA*)&wfd, SLGP_SHORTPATH);
				return;
			}
			ppf->Release();
		}
		psl->Release();
	}
	::CoUninitialize();
}

void CPEInfoDlg::PrintImportTable()
{
	m_TableInfoDlg->ShowWindow(SW_SHOW);
	m_cListCtrl.ShowWindow(SW_HIDE);HideConvertGroup();
	m_ResourceInfoDlg->ShowWindow(SW_HIDE);
	m_CDisasmDlg->ShowWindow(SW_HIDE);
	m_TableInfoDlg->ClearListCtrl();
	m_HexEditorDlg->ShowWindow(SW_HIDE);

	PIMAGE_DOS_HEADER pImageDosHeader = (PIMAGE_DOS_HEADER)FileAddress;
	PIMAGE_FILE_HEADER pImageFileHeader = (PIMAGE_FILE_HEADER)((DWORD)pImageDosHeader + pImageDosHeader->e_lfanew + sizeof(DWORD));
	PIMAGE_OPTIONAL_HEADER pImageOptionalHeader = (PIMAGE_OPTIONAL_HEADER)((DWORD)pImageFileHeader + sizeof(IMAGE_FILE_HEADER));
	PIMAGE_IMPORT_DESCRIPTOR ImportDescriptor = (PIMAGE_IMPORT_DESCRIPTOR)((DWORD)FileAddress + RVA_TO_FOA(pImageOptionalHeader->DataDirectory[1].VirtualAddress));
	UP_LIST.InsertColumn(0, _T("模块名"), LVCFMT_LEFT, 140);
	UP_LIST.InsertColumn(1, _T("导入"), LVCFMT_LEFT, 80);
	UP_LIST.InsertColumn(2, _T("时间日期戳"), LVCFMT_LEFT, 120);
	UP_LIST.InsertColumn(3, _T("转发链"), LVCFMT_LEFT, 120);
	UP_LIST.InsertColumn(4, _T("OFTs"), LVCFMT_LEFT, 100);
	UP_LIST.InsertColumn(5, _T("名称RVA"), LVCFMT_LEFT, 110);
	UP_LIST.InsertColumn(6, _T("FTs(IAT)"), LVCFMT_LEFT, 110);
	int index = 0;
	while (ImportDescriptor->Name)
	{
		DWORD DllName = RVA_TO_FOA(ImportDescriptor->Name) + (DWORD)FileAddress;
		TCHAR tDllName[MAX_PATH] = { 0 };
#ifdef UNICODE
		MultiByteToWideChar(CP_ACP, 0, (LPCCH)DllName, -1, tDllName, 100);
#else
		ExportDirectoryInfoVec[i].FunctionName = FunctionName;
#endif

		PIMAGE_THUNK_DATA pINT = (PIMAGE_THUNK_DATA)((DWORD)FileAddress + RVA_TO_FOA((DWORD)ImportDescriptor->OriginalFirstThunk));
		int Count = 0;
		while (pINT->u1.AddressOfData != 0)
		{
			Count++;
			pINT++;
		}
		DWORD TimeDateStamp = RVA_TO_FOA(ImportDescriptor->TimeDateStamp) + (DWORD)FileAddress;
		UP_LIST.InsertItem(index, tDllName);
		UP_LIST.SetItemData(index, (DWORD_PTR)ImportDescriptor);
		UP_LIST.SetItemText(index, 1, Int2CString(Count, 3));
		UP_LIST.SetItemText(index, 2, Int2CString(ImportDescriptor->TimeDateStamp, 8));
		UP_LIST.SetItemText(index, 3, Int2CString(ImportDescriptor->ForwarderChain, 8));
		UP_LIST.SetItemText(index, 4, Int2CString(ImportDescriptor->OriginalFirstThunk));
		UP_LIST.SetItemText(index, 5, Int2CString(ImportDescriptor->Name));
		UP_LIST.SetItemText(index++, 6, Int2CString(ImportDescriptor->FirstThunk));
		ImportDescriptor++;
	}
	DOWN_LIST.InsertColumn(0, _T("OFTs"), LVCFMT_LEFT, 110);
	DOWN_LIST.InsertColumn(1, _T("FTs"), LVCFMT_LEFT, 110);
	DOWN_LIST.InsertColumn(2, _T("Hint"), LVCFMT_LEFT, 110);
	DOWN_LIST.InsertColumn(3, _T("名称"), LVCFMT_LEFT, 200);
}

void CPEInfoDlg::PrintResourceTable()
{
	m_TableInfoDlg->ShowWindow(SW_HIDE);
	m_cListCtrl.ShowWindow(SW_HIDE);HideConvertGroup();
	m_ResourceInfoDlg->ShowWindow(SW_SHOW);
	m_CDisasmDlg->ShowWindow(SW_HIDE);
	m_HexEditorDlg->ShowWindow(SW_HIDE);
	m_ResourceInfoDlg->m_cTreeCtrl.DeleteAllItems();
	PIMAGE_DOS_HEADER pImageDosHeader = (PIMAGE_DOS_HEADER)FileAddress;
	PIMAGE_FILE_HEADER pImageFileHeader = (PIMAGE_FILE_HEADER)((DWORD)pImageDosHeader + pImageDosHeader->e_lfanew + sizeof(DWORD));
	PIMAGE_OPTIONAL_HEADER pImageOptionalHeader = (PIMAGE_OPTIONAL_HEADER)((DWORD)pImageFileHeader + sizeof(IMAGE_FILE_HEADER));
	PIMAGE_RESOURCE_DIRECTORY pResRoot = (PIMAGE_RESOURCE_DIRECTORY)((DWORD)FileAddress +
		RVA_TO_FOA(pImageOptionalHeader->DataDirectory[2].VirtualAddress));

	HTREEITEM hRoot = m_ResourceInfoDlg->m_cTreeCtrl.InsertItem(_T("资源目录"), 0, 0, TVI_ROOT);
	PRESOURCE_DATA ResourceData = new RESOURCE_DATA;
	ResourceData->Type = 1;
	ResourceData->Address = (DWORD)pResRoot;
	m_ResourceInfoDlg->m_cTreeCtrl.SetItemData(hRoot, (DWORD_PTR)ResourceData);
	PIMAGE_RESOURCE_DIRECTORY_ENTRY pDirEntry1 =
		(PIMAGE_RESOURCE_DIRECTORY_ENTRY)(pResRoot + 1);
	DWORD dwCountOfEntry1 = pResRoot->NumberOfIdEntries + pResRoot->NumberOfNamedEntries;
	for (DWORD i = 0; i < dwCountOfEntry1; ++i) {
		static const TCHAR* type[] =
		{
			_T("") , // 0
			_T("光标") , // 1
			_T("位图") , // 2 RT_BITMAP
			_T("图标") , // 3 RT_ICON
			_T("菜单") , // 4 RT_MENU
			_T("对话框") , // 5 RT_DIALOG
			_T("字符串表") , // 6 RT_STRING
			_T("字体目录") , // 7 RT_FONTDIR
			_T("字体") , // 8 RT_FONT
			_T("加速键表") , // 9 RT_ACCELERATOR
			_T("RCData") , // 10 RT_RCDATA
			_T("消息表") , // 11 RT_MESSAGETABLE
			_T("光标组") , // 12
			_T("") , // 13
			_T("图标组") , // 14
			_T("") , // 15
			_T("版本信息") , // 16
			_T("对话框目录") ,	// 17 #define RT_DLGINCLUDE   MAKEINTRESOURCE(17)
			_T("") , // 18 #define RT_PLUGPLAY     MAKEINTRESOURCE(19)
			_T("") , // 19 #define RT_VXD          MAKEINTRESOURCE(20)
			_T("") , // 20 #define RT_ANICURSOR    MAKEINTRESOURCE(21)
			_T("") , // 21 #define RT_ANIICON      MAKEINTRESOURCE(22)
			_T("") , // 22
			_T("网页文件") ,// 23 #define RT_HTML         MAKEINTRESOURCE(23)
			_T("配置文件") // 24 RT_MANIFEST
		};
		CString tmpStr;
		tmpStr.Format(_T("资源目录项 %d，ID：%d，AKA：%s"), i, pDirEntry1[i].Id, type[pDirEntry1[i].Id]);
		HTREEITEM tmpItem = m_ResourceInfoDlg->m_cTreeCtrl.InsertItem(tmpStr, hRoot);
		PRESOURCE_DATA ResourceData1 = new RESOURCE_DATA;
		ResourceData1->Type = 2;
		ResourceData1->Address = (DWORD)&pDirEntry1[i];
		m_ResourceInfoDlg->m_cTreeCtrl.SetItemData(tmpItem, (DWORD_PTR)ResourceData1);
		PIMAGE_RESOURCE_DIRECTORY pDir2 =
			(PIMAGE_RESOURCE_DIRECTORY)
			(pDirEntry1[i].OffsetToDirectory + (char*)pResRoot);
		PIMAGE_RESOURCE_DIRECTORY_ENTRY pDirEntry2 =
			(PIMAGE_RESOURCE_DIRECTORY_ENTRY)(pDir2 + 1);
		tmpItem = m_ResourceInfoDlg->m_cTreeCtrl.InsertItem(_T("资源目录"), tmpItem);
		PRESOURCE_DATA ResourceData2 = new RESOURCE_DATA;
		ResourceData2->Type = 1;
		ResourceData2->Address = (DWORD)pDir2;
		m_ResourceInfoDlg->m_cTreeCtrl.SetItemData(tmpItem, (DWORD_PTR)ResourceData2);
		DWORD dwCountOfEntry2 = pDir2->NumberOfIdEntries + pDir2->NumberOfNamedEntries;
		for (DWORD i = 0; i < dwCountOfEntry2; ++i) {
			CString tmpStr2;
			tmpStr2.Format(_T("资源目录项 %d，ID：%d"), i, pDirEntry2[i].Id);
			HTREEITEM tmpItem2 = m_ResourceInfoDlg->m_cTreeCtrl.InsertItem(tmpStr2, tmpItem);
			PRESOURCE_DATA ResourceData3 = new RESOURCE_DATA;
			ResourceData3->Type = 2;
			ResourceData3->Address = (DWORD)&pDirEntry2[i];
			m_ResourceInfoDlg->m_cTreeCtrl.SetItemData(tmpItem2, (DWORD_PTR)ResourceData3);
			PIMAGE_RESOURCE_DIRECTORY pDir3 =
				(PIMAGE_RESOURCE_DIRECTORY)(pDirEntry2[i].OffsetToDirectory + (char*)pResRoot);
			tmpItem2 = m_ResourceInfoDlg->m_cTreeCtrl.InsertItem(_T("资源目录"), tmpItem2);
			PRESOURCE_DATA ResourceData4 = new RESOURCE_DATA;
			ResourceData4->Type = 1;
			ResourceData4->Address = (DWORD)pDir3;
			m_ResourceInfoDlg->m_cTreeCtrl.SetItemData(tmpItem2, (DWORD_PTR)ResourceData4);
			PIMAGE_RESOURCE_DIRECTORY_ENTRY pDirEntry3 =
				(PIMAGE_RESOURCE_DIRECTORY_ENTRY)(pDir3 + 1);
			PIMAGE_RESOURCE_DATA_ENTRY pDataEntry =
				(PIMAGE_RESOURCE_DATA_ENTRY)(pDirEntry3->OffsetToData + (char*)pResRoot);
			tmpStr2.Format(_T("资源目录项 %d，ID：%d"), i, pDirEntry3[i].Id);
			HTREEITEM tmpItem3 = m_ResourceInfoDlg->m_cTreeCtrl.InsertItem(tmpStr2, tmpItem2);
			PRESOURCE_DATA ResourceData5 = new RESOURCE_DATA;
			ResourceData5->Type = 2;
			ResourceData5->Address = (DWORD)&pDirEntry3[i];
			m_ResourceInfoDlg->m_cTreeCtrl.SetItemData(tmpItem3, (DWORD_PTR)ResourceData5);
			tmpItem3 = m_ResourceInfoDlg->m_cTreeCtrl.InsertItem(_T("资源数据项"), tmpItem3);
			PRESOURCE_DATA ResourceData6 = new RESOURCE_DATA;
			ResourceData6->Type = 3;
			ResourceData6->Address = (DWORD)pDataEntry;
			m_ResourceInfoDlg->m_cTreeCtrl.SetItemData(tmpItem3, (DWORD_PTR)ResourceData6);
		}
	}
	m_ResourceInfoDlg->m_cTreeCtrl.Expand(hRoot, TVE_EXPAND);
}

void CPEInfoDlg::PrintResourceItem(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	HTREEITEM hitem = pNMTreeView->itemNew.hItem;
	PRESOURCE_DATA ResourceData = (PRESOURCE_DATA)m_cTreeCtrl.GetItemData(pNMTreeView->itemNew.hItem);
	if (ResourceData == NULL)
	{
		return;
	}
	m_ResourceInfoDlg->m_cListCtrl.DeleteAllItems();
	int index = 0;
	PIMAGE_RESOURCE_DIRECTORY ResourceDirectory = (PIMAGE_RESOURCE_DIRECTORY)ResourceData->Address;
	PIMAGE_RESOURCE_DIRECTORY_ENTRY ResourceDirectoryEntry = (PIMAGE_RESOURCE_DIRECTORY_ENTRY)ResourceData->Address;
	PIMAGE_RESOURCE_DATA_ENTRY ResourceDataEntry = (PIMAGE_RESOURCE_DATA_ENTRY)ResourceData->Address;
	switch (ResourceData->Type)
	{
	case 1:
		m_ResourceInfoDlg->m_cListCtrl.InsertItem(index, _T("Characteristics"));
		m_ResourceInfoDlg->m_cListCtrl.SetItemText(index, 1, Int2CString((DWORD) & (ResourceDirectory->Characteristics) - (DWORD)FileAddress, 8));
		m_ResourceInfoDlg->m_cListCtrl.SetItemText(index, 2, GetVariableSize(sizeof(ResourceDirectory->Characteristics)));
		m_ResourceInfoDlg->m_cListCtrl.SetItemText(index++, 3, Int2CString(ResourceDirectory->Characteristics, 2 * sizeof(ResourceDirectory->Characteristics)));
		m_ResourceInfoDlg->m_cListCtrl.InsertItem(index, _T("TimeDateStamp"));
		m_ResourceInfoDlg->m_cListCtrl.SetItemText(index, 1, Int2CString((DWORD) & (ResourceDirectory->TimeDateStamp) - (DWORD)FileAddress, 8));
		m_ResourceInfoDlg->m_cListCtrl.SetItemText(index, 2, GetVariableSize(sizeof(ResourceDirectory->TimeDateStamp)));
		m_ResourceInfoDlg->m_cListCtrl.SetItemText(index++, 3, Int2CString(ResourceDirectory->TimeDateStamp, 2 * sizeof(ResourceDirectory->TimeDateStamp)));
		m_ResourceInfoDlg->m_cListCtrl.InsertItem(index, _T("MajorVersion"));
		m_ResourceInfoDlg->m_cListCtrl.SetItemText(index, 1, Int2CString((DWORD) & (ResourceDirectory->MajorVersion) - (DWORD)FileAddress, 8));
		m_ResourceInfoDlg->m_cListCtrl.SetItemText(index, 2, GetVariableSize(sizeof(ResourceDirectory->MajorVersion)));
		m_ResourceInfoDlg->m_cListCtrl.SetItemText(index++, 3, Int2CString(ResourceDirectory->MajorVersion, 2 * sizeof(ResourceDirectory->MajorVersion)));
		m_ResourceInfoDlg->m_cListCtrl.InsertItem(index, _T("MinorVersion"));
		m_ResourceInfoDlg->m_cListCtrl.SetItemText(index, 1, Int2CString((DWORD) & (ResourceDirectory->MinorVersion) - (DWORD)FileAddress, 8));
		m_ResourceInfoDlg->m_cListCtrl.SetItemText(index, 2, GetVariableSize(sizeof(ResourceDirectory->MinorVersion)));
		m_ResourceInfoDlg->m_cListCtrl.SetItemText(index++, 3, Int2CString(ResourceDirectory->MinorVersion, 2 * sizeof(ResourceDirectory->MinorVersion)));
		m_ResourceInfoDlg->m_cListCtrl.InsertItem(index, _T("NumberOfNamedEntries"));
		m_ResourceInfoDlg->m_cListCtrl.SetItemText(index, 1, Int2CString((DWORD) & (ResourceDirectory->NumberOfNamedEntries) - (DWORD)FileAddress, 8));
		m_ResourceInfoDlg->m_cListCtrl.SetItemText(index, 2, GetVariableSize(sizeof(ResourceDirectory->NumberOfNamedEntries)));
		m_ResourceInfoDlg->m_cListCtrl.SetItemText(index++, 3, Int2CString(ResourceDirectory->NumberOfNamedEntries, 2 * sizeof(ResourceDirectory->NumberOfNamedEntries)));
		m_ResourceInfoDlg->m_cListCtrl.InsertItem(index, _T("NumberOfIdEntries"));
		m_ResourceInfoDlg->m_cListCtrl.SetItemText(index, 1, Int2CString((DWORD) & (ResourceDirectory->NumberOfIdEntries) - (DWORD)FileAddress, 8));
		m_ResourceInfoDlg->m_cListCtrl.SetItemText(index, 2, GetVariableSize(sizeof(ResourceDirectory->NumberOfIdEntries)));
		m_ResourceInfoDlg->m_cListCtrl.SetItemText(index++, 3, Int2CString(ResourceDirectory->NumberOfIdEntries, 2 * sizeof(ResourceDirectory->NumberOfIdEntries)));
		break;
	case 2:
		m_ResourceInfoDlg->m_cListCtrl.InsertItem(index, _T("Name"));
		m_ResourceInfoDlg->m_cListCtrl.SetItemText(index, 1, Int2CString((DWORD) & (ResourceDirectoryEntry->Name) - (DWORD)FileAddress, 8));
		m_ResourceInfoDlg->m_cListCtrl.SetItemText(index, 2, GetVariableSize(sizeof(ResourceDirectoryEntry->Name)));
		m_ResourceInfoDlg->m_cListCtrl.SetItemText(index++, 3, Int2CString(ResourceDirectoryEntry->Name, 2 * sizeof(ResourceDirectoryEntry->Name)));
		m_ResourceInfoDlg->m_cListCtrl.InsertItem(index, _T("OffsetToData"));
		m_ResourceInfoDlg->m_cListCtrl.SetItemText(index, 1, Int2CString((DWORD) & (ResourceDirectoryEntry->OffsetToData) - (DWORD)FileAddress, 8));
		m_ResourceInfoDlg->m_cListCtrl.SetItemText(index, 2, GetVariableSize(sizeof(ResourceDirectoryEntry->OffsetToData)));
		m_ResourceInfoDlg->m_cListCtrl.SetItemText(index++, 3, Int2CString(ResourceDirectoryEntry->OffsetToData, 2 * sizeof(ResourceDirectoryEntry->OffsetToData)));
		break;
	case 3:
		m_ResourceInfoDlg->m_cListCtrl.InsertItem(index, _T("OffsetToData"));
		m_ResourceInfoDlg->m_cListCtrl.SetItemText(index, 1, Int2CString((DWORD) & (ResourceDataEntry->OffsetToData) - (DWORD)FileAddress, 8));
		m_ResourceInfoDlg->m_cListCtrl.SetItemText(index, 2, GetVariableSize(sizeof(ResourceDataEntry->OffsetToData)));
		m_ResourceInfoDlg->m_cListCtrl.SetItemText(index++, 3, Int2CString(ResourceDataEntry->OffsetToData, 2 * sizeof(ResourceDataEntry->OffsetToData)));
		m_ResourceInfoDlg->m_cListCtrl.InsertItem(index, _T("Size"));
		m_ResourceInfoDlg->m_cListCtrl.SetItemText(index, 1, Int2CString((DWORD) & (ResourceDataEntry->Size) - (DWORD)FileAddress, 8));
		m_ResourceInfoDlg->m_cListCtrl.SetItemText(index, 2, GetVariableSize(sizeof(ResourceDataEntry->Size)));
		m_ResourceInfoDlg->m_cListCtrl.SetItemText(index++, 3, Int2CString(ResourceDataEntry->Size, 2 * sizeof(ResourceDataEntry->Size)));
		m_ResourceInfoDlg->m_cListCtrl.InsertItem(index, _T("CodePage"));
		m_ResourceInfoDlg->m_cListCtrl.SetItemText(index, 1, Int2CString((DWORD) & (ResourceDataEntry->CodePage) - (DWORD)FileAddress, 8));
		m_ResourceInfoDlg->m_cListCtrl.SetItemText(index, 2, GetVariableSize(sizeof(ResourceDataEntry->CodePage)));
		m_ResourceInfoDlg->m_cListCtrl.SetItemText(index++, 3, Int2CString(ResourceDataEntry->CodePage, 2 * sizeof(ResourceDataEntry->CodePage)));
		m_ResourceInfoDlg->m_cListCtrl.InsertItem(index, _T("Reserved"));
		m_ResourceInfoDlg->m_cListCtrl.SetItemText(index, 1, Int2CString((DWORD) & (ResourceDataEntry->Reserved) - (DWORD)FileAddress, 8));
		m_ResourceInfoDlg->m_cListCtrl.SetItemText(index, 2, GetVariableSize(sizeof(ResourceDataEntry->Reserved)));
		m_ResourceInfoDlg->m_cListCtrl.SetItemText(index++, 3, Int2CString(ResourceDataEntry->Reserved, 2 * sizeof(ResourceDataEntry->Reserved)));
		break;
	}
}

bool sortFun(const ExportDirectoryInfo p1, const ExportDirectoryInfo p2)
{
	return p1.FunctionOrdinals < p2.FunctionOrdinals;
}

void CPEInfoDlg::PrintExportTable()
{
	m_TableInfoDlg->ShowWindow(SW_SHOW);
	m_cListCtrl.ShowWindow(SW_HIDE);HideConvertGroup();
	m_ResourceInfoDlg->ShowWindow(SW_HIDE);
	m_CDisasmDlg->ShowWindow(SW_HIDE);
	m_HexEditorDlg->ShowWindow(SW_HIDE);
	m_TableInfoDlg->ClearListCtrl();
	PIMAGE_DOS_HEADER pImageDosHeader = (PIMAGE_DOS_HEADER)FileAddress;
	PIMAGE_FILE_HEADER pImageFileHeader = (PIMAGE_FILE_HEADER)((DWORD)pImageDosHeader + pImageDosHeader->e_lfanew + sizeof(DWORD));
	PIMAGE_OPTIONAL_HEADER pImageOptionalHeader = (PIMAGE_OPTIONAL_HEADER)((DWORD)pImageFileHeader + sizeof(IMAGE_FILE_HEADER));

	PIMAGE_EXPORT_DIRECTORY pImageExportDirectory = (PIMAGE_EXPORT_DIRECTORY)
		(RVA_TO_FOA(pImageOptionalHeader->DataDirectory[0].VirtualAddress) + (DWORD)FileAddress);
	UP_LIST.InsertColumn(0, _T("成员"), LVCFMT_LEFT, 160);
	UP_LIST.InsertColumn(1, _T("偏移量"), LVCFMT_LEFT, 120);
	UP_LIST.InsertColumn(2, _T("大小"), LVCFMT_LEFT, 100);
	UP_LIST.InsertColumn(3, _T("值"), LVCFMT_LEFT, 110);
	int index = 0;
	UP_LIST.InsertItem(index, _T("Characteristics"));
	UP_LIST.SetItemText(index, 1, Int2CString((DWORD) & (pImageExportDirectory->Characteristics) - (DWORD)FileAddress));
	UP_LIST.SetItemText(index, 2, GetVariableSize(sizeof(pImageExportDirectory->Characteristics)));
	UP_LIST.SetItemText(index++, 3, Int2CString(pImageExportDirectory->Characteristics));
	UP_LIST.InsertItem(index, _T("TimeDateStamp"));
	UP_LIST.SetItemText(index, 1, Int2CString((DWORD) & (pImageExportDirectory->TimeDateStamp) - (DWORD)FileAddress));
	UP_LIST.SetItemText(index, 2, GetVariableSize(sizeof(pImageExportDirectory->TimeDateStamp)));
	UP_LIST.SetItemText(index++, 3, Int2CString(pImageExportDirectory->TimeDateStamp));
	UP_LIST.InsertItem(index, _T("MajorVersion"));
	UP_LIST.SetItemText(index, 1, Int2CString((DWORD) & (pImageExportDirectory->MajorVersion) - (DWORD)FileAddress));
	UP_LIST.SetItemText(index, 2, GetVariableSize(sizeof(pImageExportDirectory->MajorVersion)));
	UP_LIST.SetItemText(index++, 3, Int2CString(pImageExportDirectory->MajorVersion));
	UP_LIST.InsertItem(index, _T("MinorVersion"));
	UP_LIST.SetItemText(index, 1, Int2CString((DWORD) & (pImageExportDirectory->MinorVersion) - (DWORD)FileAddress));
	UP_LIST.SetItemText(index, 2, GetVariableSize(sizeof(pImageExportDirectory->MinorVersion)));
	UP_LIST.SetItemText(index++, 3, Int2CString(pImageExportDirectory->MinorVersion));
	UP_LIST.InsertItem(index, _T("Name"));
	UP_LIST.SetItemText(index, 1, Int2CString((DWORD) & (pImageExportDirectory->Name) - (DWORD)FileAddress));
	UP_LIST.SetItemText(index, 2, GetVariableSize(sizeof(pImageExportDirectory->Name)));
	UP_LIST.SetItemText(index++, 3, Int2CString(pImageExportDirectory->Name));
	UP_LIST.InsertItem(index, _T("Base"));
	UP_LIST.SetItemText(index, 1, Int2CString((DWORD) & (pImageExportDirectory->Base) - (DWORD)FileAddress));
	UP_LIST.SetItemText(index, 2, GetVariableSize(sizeof(pImageExportDirectory->Base)));
	UP_LIST.SetItemText(index++, 3, Int2CString(pImageExportDirectory->Base));
	UP_LIST.InsertItem(index, _T("NumberOfFunctions"));
	UP_LIST.SetItemText(index, 1, Int2CString((DWORD) & (pImageExportDirectory->NumberOfFunctions) - (DWORD)FileAddress));
	UP_LIST.SetItemText(index, 2, GetVariableSize(sizeof(pImageExportDirectory->NumberOfFunctions)));
	UP_LIST.SetItemText(index++, 3, Int2CString(pImageExportDirectory->NumberOfFunctions));
	UP_LIST.InsertItem(index, _T("NumberOfNames"));
	UP_LIST.SetItemText(index, 1, Int2CString((DWORD) & (pImageExportDirectory->NumberOfNames) - (DWORD)FileAddress));
	UP_LIST.SetItemText(index, 2, GetVariableSize(sizeof(pImageExportDirectory->NumberOfNames)));
	UP_LIST.SetItemText(index++, 3, Int2CString(pImageExportDirectory->NumberOfNames));
	UP_LIST.InsertItem(index, _T("AddressOfFunctions"));
	UP_LIST.SetItemText(index, 1, Int2CString((DWORD) & (pImageExportDirectory->AddressOfFunctions) - (DWORD)FileAddress));
	UP_LIST.SetItemText(index, 2, GetVariableSize(sizeof(pImageExportDirectory->AddressOfFunctions)));
	UP_LIST.SetItemText(index++, 3, Int2CString(pImageExportDirectory->AddressOfFunctions));
	UP_LIST.InsertItem(index, _T("AddressOfNames"));
	UP_LIST.SetItemText(index, 1, Int2CString((DWORD) & (pImageExportDirectory->AddressOfNames) - (DWORD)FileAddress));
	UP_LIST.SetItemText(index, 2, GetVariableSize(sizeof(pImageExportDirectory->AddressOfNames)));
	UP_LIST.SetItemText(index++, 3, Int2CString(pImageExportDirectory->AddressOfNames));
	UP_LIST.InsertItem(index, _T("AddressOfNameOrdinals"));
	UP_LIST.SetItemText(index, 1, Int2CString((DWORD) & (pImageExportDirectory->AddressOfNameOrdinals) - (DWORD)FileAddress));
	UP_LIST.SetItemText(index, 2, GetVariableSize(sizeof(pImageExportDirectory->AddressOfNameOrdinals)));
	UP_LIST.SetItemText(index++, 3, Int2CString(pImageExportDirectory->AddressOfNameOrdinals));
	DOWN_LIST.InsertColumn(0, _T("序号"), LVCFMT_LEFT, 160);
	DOWN_LIST.InsertColumn(1, _T("函数RVA"), LVCFMT_LEFT, 120);
	DOWN_LIST.InsertColumn(2, _T("名称序号"), LVCFMT_LEFT, 100);
	DOWN_LIST.InsertColumn(3, _T("名称RVA"), LVCFMT_LEFT, 110);
	DOWN_LIST.InsertColumn(4, _T("名称"), LVCFMT_LEFT, 200);

	DWORD* AddressOfFunctions = (DWORD*)((DWORD)FileAddress + RVA_TO_FOA(pImageExportDirectory->AddressOfFunctions));
	DWORD* AddressOfNames = (DWORD*)((DWORD)FileAddress + RVA_TO_FOA(pImageExportDirectory->AddressOfNames));
	WORD* AddressOfNameOrdinals = (WORD*)((DWORD)FileAddress + RVA_TO_FOA(pImageExportDirectory->AddressOfNameOrdinals));


	std::vector <_ExportDirectoryInfo> ExportDirectoryInfoVec(pImageExportDirectory->NumberOfFunctions);

	for (DWORD i = 0; i < pImageExportDirectory->NumberOfNames; i++)
	{
		ExportDirectoryInfoVec[i].FunctionNameRVA = AddressOfNames[i];
		LPCSTR FunctionName = (LPCSTR)(RVA_TO_FOA(AddressOfNames[i]) + (DWORD)FileAddress);
#ifdef UNICODE
		MultiByteToWideChar(CP_ACP, 0, FunctionName, -1, ExportDirectoryInfoVec[i].FunctionName, 100);
#else
		ExportDirectoryInfoVec[i].FunctionName = FunctionName;
#endif
	}
	for (DWORD i = 0; i < pImageExportDirectory->NumberOfFunctions; i++)
	{
		ExportDirectoryInfoVec[i].FunctionAddress = AddressOfFunctions[AddressOfNameOrdinals[i]];
		ExportDirectoryInfoVec[i].FunctionOrdinals = AddressOfNameOrdinals[i];
	}
	sort(ExportDirectoryInfoVec.begin(), ExportDirectoryInfoVec.end(), sortFun);
	for (size_t i = 0; i < ExportDirectoryInfoVec.size(); i++)
	{
		DOWN_LIST.InsertItem(i, Int2CString(i + pImageExportDirectory->Base));
		DOWN_LIST.SetItemText(i, 1, Int2CString(ExportDirectoryInfoVec[i].FunctionAddress));
		DOWN_LIST.SetItemText(i, 2, Int2CString(i));
		DOWN_LIST.SetItemText(i, 3, Int2CString(ExportDirectoryInfoVec[i].FunctionNameRVA));
		DOWN_LIST.SetItemText(i, 4, ExportDirectoryInfoVec[i].FunctionName);
	}
}

void CPEInfoDlg::PrintRelocationTable()
{
	m_TableInfoDlg->ShowWindow(SW_SHOW);
	m_cListCtrl.ShowWindow(SW_HIDE);HideConvertGroup();
	m_ResourceInfoDlg->ShowWindow(SW_HIDE);
	m_CDisasmDlg->ShowWindow(SW_HIDE);
	m_HexEditorDlg->ShowWindow(SW_HIDE);
	m_TableInfoDlg->ClearListCtrl();
	PIMAGE_DOS_HEADER pImageDosHeader = (PIMAGE_DOS_HEADER)FileAddress;
	PIMAGE_FILE_HEADER pImageFileHeader = (PIMAGE_FILE_HEADER)((DWORD)pImageDosHeader + pImageDosHeader->e_lfanew + sizeof(DWORD));
	PIMAGE_OPTIONAL_HEADER pImageOptionalHeader = (PIMAGE_OPTIONAL_HEADER)
		((DWORD)pImageFileHeader + sizeof(IMAGE_FILE_HEADER));
	PIMAGE_BASE_RELOCATION pRelocation = (PIMAGE_BASE_RELOCATION)(
		RVA_TO_FOA((DWORD)pImageOptionalHeader->DataDirectory[5].VirtualAddress) + (DWORD)FileAddress);

	UP_LIST.InsertColumn(0, _T("RVA"), LVCFMT_LEFT, 160);
	UP_LIST.InsertColumn(1, _T("块大小"), LVCFMT_LEFT, 160);
	UP_LIST.InsertColumn(2, _T("Items"), LVCFMT_LEFT, 160);
	int index = 0;
	while (pRelocation->VirtualAddress != 0)
	{
		UP_LIST.InsertItem(index, Int2CString(pRelocation->VirtualAddress));
		UP_LIST.SetItemData(index, (DWORD_PTR)pRelocation);
		UP_LIST.SetItemText(index, 1, Int2CString(pRelocation->SizeOfBlock));
		UP_LIST.SetItemText(index++, 2, Int2CString((pRelocation->SizeOfBlock - 8) / 2));
		pRelocation = (PIMAGE_BASE_RELOCATION)((DWORD)pRelocation + pRelocation->SizeOfBlock);
	}
	DOWN_LIST.InsertColumn(0, _T("Item"), LVCFMT_LEFT, 160);
	DOWN_LIST.InsertColumn(1, _T("RVA"), LVCFMT_LEFT, 160);
	DOWN_LIST.InsertColumn(2, _T("类型"), LVCFMT_LEFT, 160);
}

void CPEInfoDlg::PrintDelayImportTable()
{
	m_TableInfoDlg->ShowWindow(SW_SHOW);
	m_cListCtrl.ShowWindow(SW_HIDE); HideConvertGroup();
	m_ResourceInfoDlg->ShowWindow(SW_HIDE);
	m_CDisasmDlg->ShowWindow(SW_HIDE);
	m_TableInfoDlg->ClearListCtrl();
	m_HexEditorDlg->ShowWindow(SW_HIDE);

	PIMAGE_DOS_HEADER pImageDosHeader = (PIMAGE_DOS_HEADER)FileAddress;
	PIMAGE_FILE_HEADER pImageFileHeader = (PIMAGE_FILE_HEADER)((DWORD)pImageDosHeader + pImageDosHeader->e_lfanew + sizeof(DWORD));
	PIMAGE_OPTIONAL_HEADER pImageOptionalHeader = (PIMAGE_OPTIONAL_HEADER)((DWORD)pImageFileHeader + sizeof(IMAGE_FILE_HEADER));

	PIMAGE_DELAYLOAD_DESCRIPTOR DelayImportDescriptor = (PIMAGE_DELAYLOAD_DESCRIPTOR)((DWORD)FileAddress + RVA_TO_FOA(pImageOptionalHeader->DataDirectory[IMAGE_DIRECTORY_ENTRY_DELAY_IMPORT].VirtualAddress));
	UP_LIST.InsertColumn(0, _T("模块名"), LVCFMT_LEFT, 140);
	UP_LIST.InsertColumn(1, _T("导入"), LVCFMT_LEFT, 80);
	UP_LIST.InsertColumn(2, _T("时间日期戳"), LVCFMT_LEFT, 120);
	UP_LIST.InsertColumn(4, _T("OFTs"), LVCFMT_LEFT, 100);
	UP_LIST.InsertColumn(5, _T("名称RVA"), LVCFMT_LEFT, 110);
	UP_LIST.InsertColumn(6, _T("FTs(IAT)"), LVCFMT_LEFT, 110);
	int index = 0;
	while (DelayImportDescriptor->Attributes.AllAttributes)
	{
		DWORD DllName = RVA_TO_FOA(DelayImportDescriptor->DllNameRVA) + (DWORD)FileAddress;
		TCHAR tDllName[MAX_PATH] = { 0 };
#ifdef UNICODE
		MultiByteToWideChar(CP_ACP, 0, (LPCCH)DllName, -1, tDllName, 100);
#else
		tDllName = DllName;
#endif

		PIMAGE_THUNK_DATA pINT = (PIMAGE_THUNK_DATA)((DWORD)FileAddress + RVA_TO_FOA((DWORD)DelayImportDescriptor->ImportNameTableRVA));
		int Count = 0;
		while (pINT->u1.AddressOfData != 0)
		{
			Count++;
			pINT++;
		}
		DWORD TimeDateStamp = RVA_TO_FOA(DelayImportDescriptor->TimeDateStamp) + (DWORD)FileAddress;
		UP_LIST.InsertItem(index, tDllName);
		UP_LIST.SetItemData(index, (DWORD_PTR)DelayImportDescriptor);
		UP_LIST.SetItemText(index, 1, Int2CString(Count, 3));
		UP_LIST.SetItemText(index, 2, Int2CString(DelayImportDescriptor->TimeDateStamp, 8));
		UP_LIST.SetItemText(index, 3, Int2CString(DelayImportDescriptor->ImportAddressTableRVA, 8));
		UP_LIST.SetItemText(index, 4, Int2CString(DelayImportDescriptor->DllNameRVA, 8));
		UP_LIST.SetItemText(index++, 5, Int2CString(DelayImportDescriptor->ImportNameTableRVA, 8));
		DelayImportDescriptor++;
	}
	DOWN_LIST.InsertColumn(0, _T("OFTs"), LVCFMT_LEFT, 110);
	DOWN_LIST.InsertColumn(1, _T("FTs"), LVCFMT_LEFT, 110);
	DOWN_LIST.InsertColumn(2, _T("Hint"), LVCFMT_LEFT, 110);
	DOWN_LIST.InsertColumn(3, _T("名称"), LVCFMT_LEFT, 200);
}

void CPEInfoDlg::PrintHexEditor()
{
	m_TableInfoDlg->ShowWindow(SW_HIDE);
	m_cListCtrl.ShowWindow(SW_HIDE);HideConvertGroup();
	m_ResourceInfoDlg->ShowWindow(SW_HIDE);
	m_CDisasmDlg->ShowWindow(SW_HIDE);
	m_HexEditorDlg->ShowWindow(SW_SHOW); 
	CRect rectDlg;
	GetWindowRect(rectDlg);
	rectDlg.DeflateRect(315, 34, 12, 11); 
	m_HexEditorDlg->MoveWindow(rectDlg);
	m_HexEditorDlg->m_HexEditor.SetData(m_FileSize, (BYTE*)FileAddress);
}

void CPEInfoDlg::PrintAddressConvert()
{
	m_TableInfoDlg->ShowWindow(SW_HIDE);
	m_cListCtrl.ShowWindow(SW_HIDE); 
	m_ResourceInfoDlg->ShowWindow(SW_HIDE);
	m_CDisasmDlg->ShowWindow(SW_HIDE);
	m_HexEditorDlg->ShowWindow(SW_SHOW);
	m_edit_VA.ShowWindow(SW_SHOW);
	m_edit_RVA.ShowWindow(SW_SHOW);
	m_edit_FOA.ShowWindow(SW_SHOW);
	m_IsConverting = true;
	GetDlgItem(IDC_STATIC_1)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_STATIC2)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_STATIC_3)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_STATIC_4)->ShowWindow(SW_SHOW);
	if (m_HexEditorDlg)
	{
		CRect rectDlg;
		GetWindowRect(rectDlg);
		rectDlg.DeflateRect(315, 170, 12, 11);
		m_HexEditorDlg->MoveWindow(rectDlg);
	}	
	m_HexEditorDlg->m_HexEditor.SetData(m_FileSize, (BYTE*)FileAddress);
}

void CPEInfoDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);
	CRect rectDlg;
	if (m_TableInfoDlg)
	{
		GetClientRect(rectDlg);
		rectDlg.DeflateRect(305, 5, 5, 5);
		m_TableInfoDlg->MoveWindow(rectDlg);
	}
	if (m_ResourceInfoDlg)
	{
		GetClientRect(rectDlg);
		rectDlg.DeflateRect(305, 5, 5, 5);
		m_ResourceInfoDlg->MoveWindow(rectDlg);
	}
	if (m_cListCtrl)
	{
		GetClientRect(rectDlg);
		rectDlg.DeflateRect(305, 5, 5, 5);
		m_cListCtrl.MoveWindow(rectDlg);
	}
	if (m_cTreeCtrl)
	{
		GetClientRect(rectDlg);
		rectDlg.DeflateRect(5, 5, 5, 5);
		rectDlg.right = 300;
		m_cTreeCtrl.MoveWindow(rectDlg);
	}
	if (m_CDisasmDlg)
	{
		GetClientRect(rectDlg);
		rectDlg.DeflateRect(305, 5, 5, 5);
		m_CDisasmDlg->MoveWindow(rectDlg);
	}
	if (m_HexEditorDlg)
	{
		GetWindowRect(rectDlg);
		if (m_IsConverting)
		{
			rectDlg.DeflateRect(315, 170, 12, 11);
		}
		else
		{
			rectDlg.DeflateRect(315, 34, 12, 11);
		}
		m_HexEditorDlg->MoveWindow(rectDlg);
	}	
}

void CPEInfoDlg::OnMove(int x, int y)
{
	CDialogEx::OnMove(x, y);

	CRect rectDlg;
	if (m_TableInfoDlg)
	{
		GetClientRect(rectDlg);
		rectDlg.DeflateRect(305, 5, 5, 5);
		m_TableInfoDlg->MoveWindow(rectDlg);
	}
	if (m_ResourceInfoDlg)
	{
		GetClientRect(rectDlg);
		rectDlg.DeflateRect(305, 5, 5, 5);
		m_ResourceInfoDlg->MoveWindow(rectDlg);
	}
	if (m_cListCtrl)
	{
		GetClientRect(rectDlg);
		rectDlg.DeflateRect(305, 5, 5, 5);
		m_cListCtrl.MoveWindow(rectDlg);
	}
	if (m_cTreeCtrl)
	{
		GetClientRect(rectDlg);
		rectDlg.DeflateRect(5, 5, 5, 5);
		rectDlg.right = 300;
		m_cTreeCtrl.MoveWindow(rectDlg);
	}
	if (m_CDisasmDlg)
	{
		GetClientRect(rectDlg);
		rectDlg.DeflateRect(305, 5, 5, 5);
		m_CDisasmDlg->MoveWindow(rectDlg);
	}
	if (m_HexEditorDlg)
	{
		GetWindowRect(rectDlg);
		if (m_IsConverting)
		{
			rectDlg.DeflateRect(315, 170, 12, 11);
		}
		else
		{
			rectDlg.DeflateRect(315, 34, 12, 11);
		}
		m_HexEditorDlg->MoveWindow(rectDlg);
	}
}

void CPEInfoDlg::MyExpandTree(HTREEITEM hTreeItem)
{
	if (!m_cTreeCtrl.ItemHasChildren(hTreeItem))
	{
		return;
	}
	HTREEITEM hNextItem = m_cTreeCtrl.GetChildItem(hTreeItem);
	while (hNextItem != NULL)
	{
		MyExpandTree(hNextItem);
		hNextItem = m_cTreeCtrl.GetNextItem(hNextItem, TVGN_NEXT);
	}
	m_cTreeCtrl.Expand(hTreeItem, TVE_EXPAND);
}

void CPEInfoDlg::OnLvnItemchangedList2(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	HTREEITEM TreeItem = m_cTreeCtrl.GetSelectedItem();
	CString ItemText = m_cTreeCtrl.GetItemText(TreeItem);
	POSITION ps;
	int nIndex;

	m_TableInfoDlg->ClearDownListCtrl();
	ps = UP_LIST.GetFirstSelectedItemPosition();
	nIndex = UP_LIST.GetNextSelectedItem(ps);
	if (nIndex >= 0)
	{
		if (ItemText == _T("导入目录"))
		{
			PIMAGE_IMPORT_DESCRIPTOR ImportDescriptor = (PIMAGE_IMPORT_DESCRIPTOR)UP_LIST.GetItemData(nIndex);
			PIMAGE_THUNK_DATA pIAT = (PIMAGE_THUNK_DATA)((DWORD)FileAddress + RVA_TO_FOA((DWORD)ImportDescriptor->FirstThunk));
			PIMAGE_THUNK_DATA pINT = (PIMAGE_THUNK_DATA)((DWORD)FileAddress + RVA_TO_FOA((DWORD)ImportDescriptor->OriginalFirstThunk));
			int index = 0;
			while (pINT->u1.AddressOfData)
			{
				if (IMAGE_SNAP_BY_ORDINAL32(pINT->u1.AddressOfData))
				{
					DOWN_LIST.InsertItem(index, Int2CString(pINT->u1.AddressOfData, 8));
					DOWN_LIST.SetItemText(index, 1, Int2CString(pIAT->u1.AddressOfData, 8));
					DOWN_LIST.SetItemText(index, 2, Int2CString(pIAT->u1.Ordinal & 0xFFFF, 2));
					DOWN_LIST.SetItemText(index++, 3, _T(""));
				}
				else
				{
					TCHAR FunctionName[MAX_PATH] = { 0 };
					PIMAGE_IMPORT_BY_NAME pImport =
						(PIMAGE_IMPORT_BY_NAME)(RVA_TO_FOA(pINT->u1.AddressOfData) + FileAddress);
#ifdef UNICODE
					MultiByteToWideChar(CP_ACP, 0, (LPCCH)pImport->Name, -1, FunctionName, 100);
#else
					FunctionName = (LPCCH)pImport->Name;
#endif
					DOWN_LIST.InsertItem(index, Int2CString(pINT->u1.AddressOfData));
					DOWN_LIST.SetItemText(index, 1, Int2CString(pIAT->u1.AddressOfData));
					DOWN_LIST.SetItemText(index, 2, Int2CString(pImport->Hint, 3));
					DOWN_LIST.SetItemText(index++, 3, FunctionName);
				}
				pIAT++;
				pINT++;
				}
			}
		else if (ItemText == _T("重定位目录"))
		{
			PIMAGE_BASE_RELOCATION pImageRelocation = (PIMAGE_BASE_RELOCATION)UP_LIST.GetItemData(nIndex);
			DWORD dwCount = (pImageRelocation->SizeOfBlock - 8) / 2;
			typedef struct _TypeOffset {
				WORD Offset : 12;
				WORD Type : 4;
			}TypeOffset;
			TypeOffset* pTypeOffset = (TypeOffset*)(pImageRelocation + 1);
			for (DWORD i = 0; i < dwCount; ++i) {
				DOWN_LIST.InsertItem(i, Int2CString(*(WORD*)(pTypeOffset + i)));
				DOWN_LIST.SetItemText(i, 1, Int2CString(pTypeOffset[i].Offset + pImageRelocation->VirtualAddress));
				DOWN_LIST.SetItemText(i, 2, _T("HIGHLOW"));
			}
		}
		else if (ItemText == _T("延迟导入目录"))
		{
			PIMAGE_DELAYLOAD_DESCRIPTOR ImportDescriptor = (PIMAGE_DELAYLOAD_DESCRIPTOR)UP_LIST.GetItemData(nIndex); 
			PIMAGE_THUNK_DATA pIAT = (PIMAGE_THUNK_DATA)((DWORD)FileAddress + RVA_TO_FOA((DWORD)ImportDescriptor->ImportAddressTableRVA));
			PIMAGE_THUNK_DATA pINT = (PIMAGE_THUNK_DATA)((DWORD)FileAddress + RVA_TO_FOA((DWORD)ImportDescriptor->ImportNameTableRVA)); 
			int index = 0;
			while (pINT->u1.AddressOfData)
			{
				if (IMAGE_SNAP_BY_ORDINAL32(pINT->u1.AddressOfData))
				{
					DOWN_LIST.InsertItem(index, Int2CString(pINT->u1.AddressOfData, 8));
					DOWN_LIST.SetItemText(index, 1, Int2CString(pIAT->u1.AddressOfData, 8));
					DOWN_LIST.SetItemText(index, 2, Int2CString(pIAT->u1.Ordinal & 0xFFFF, 2));
					DOWN_LIST.SetItemText(index++, 3, _T(""));
				}
				else
				{
					TCHAR FunctionName[MAX_PATH] = { 0 };
					PIMAGE_IMPORT_BY_NAME pImport =
						(PIMAGE_IMPORT_BY_NAME)(RVA_TO_FOA(pINT->u1.AddressOfData) + FileAddress);
#ifdef UNICODE
					MultiByteToWideChar(CP_ACP, 0, (LPCCH)pImport->Name, -1, FunctionName, 100);
#else
					FunctionName = (LPCCH)pImport->Name;
#endif
					DOWN_LIST.InsertItem(index, Int2CString(pINT->u1.AddressOfData));
					DOWN_LIST.SetItemText(index, 1, Int2CString(pIAT->u1.AddressOfData));
					DOWN_LIST.SetItemText(index, 2, Int2CString(pImport->Hint, 3));
					DOWN_LIST.SetItemText(index++, 3, FunctionName);
				}
				pIAT++;
				pINT++;
			}
		}

		}
	*pResult = 0;
	}

void CPEInfoDlg::PrintDisasmDlg()
{
	PIMAGE_DOS_HEADER pImageDosHeader = (PIMAGE_DOS_HEADER)FileAddress;
	PIMAGE_FILE_HEADER pImageFileHeader = (PIMAGE_FILE_HEADER)((DWORD)pImageDosHeader + pImageDosHeader->e_lfanew + sizeof(DWORD));
	PIMAGE_OPTIONAL_HEADER pImageOptionalHeader = (PIMAGE_OPTIONAL_HEADER)
		((DWORD)pImageFileHeader + sizeof(IMAGE_FILE_HEADER));
	int OEP = RVA_TO_FOA(pImageOptionalHeader->AddressOfEntryPoint);
	int OEP_VA = pImageOptionalHeader->ImageBase + pImageOptionalHeader->AddressOfEntryPoint;
	m_TableInfoDlg->ShowWindow(SW_HIDE);
	m_cListCtrl.ShowWindow(SW_HIDE);
	HideConvertGroup();
	m_ResourceInfoDlg->ShowWindow(SW_HIDE);
	m_CDisasmDlg->ShowWindow(SW_SHOW);
	m_HexEditorDlg->ShowWindow(SW_HIDE);
	m_CDisasmDlg->InitEditAddress(OEP, OEP_VA);
	UpdateData(FALSE);
}

int CStringHexToInt(CString str)
{
	int nRet = 0;
	int count = 1;
	for (int i = str.GetLength() - 1; i >= 0; --i)
	{
		int nNum = 0;
		char chTest;
		chTest = str.GetAt(i);
		if (chTest >= '0' && chTest <= '9')
		{
			nNum = chTest - '0';
		}
		else if (chTest >= 'A' && chTest <= 'F')
		{
			nNum = chTest - 'A' + 10;
		}
		else if (chTest >= 'a' && chTest <= 'f')
		{
			nNum = chTest - 'a' + 10;
		}
		nRet += nNum * count;
		count *= 16;

	}
	return nRet;
}

void CPEInfoDlg::StartDisasm()
{
	m_CDisasmDlg->m_cListCtrl.DeleteAllItems();
	DISASM disAsm = { 0 };
	int opcode = CStringHexToInt(m_CDisasmDlg->m_edit_offset) + (int)FileAddress;
	disAsm.EIP = (UIntPtr)opcode; 
	disAsm.VirtualAddr = CStringHexToInt(m_CDisasmDlg->m_edit_base); 
	disAsm.Archi = 0; // 0 => 32 , 1 => 64
	disAsm.Options = 0x000; // masm 汇编指令格式
	int index = 0;
	int nOpcodeSize = _ttoi(m_CDisasmDlg->m_edit_size); 
	int nCount = 0;
	int nLen = 0; 
	while (nCount < nOpcodeSize)
	{
		nLen = Disasm(&disAsm); 
		unsigned int uAddr = disAsm.VirtualAddr;

<<<<<<< HEAD
		printf(" | %s\n", disAsm.CompleteInstr); 
=======
>>>>>>> v1.0
		m_CDisasmDlg->m_cListCtrl.InsertItem(index, Int2CString(uAddr));
		TCHAR Instruct[0x100] = { 0 };
#ifdef UNICODE
		MultiByteToWideChar(CP_ACP, 0, disAsm.CompleteInstr, -1, Instruct, 100);
#else
		strcpy(Instruct, disAsm.CompleteInstr);
#endif
		CString OPCODE;
		for (int i = 0; i < nLen; ++i)
		{
			CString tmp;
			tmp.Format(_T("%02X "), ((const unsigned char*)disAsm.EIP)[i]);
			OPCODE += tmp;
		}
		m_CDisasmDlg->m_cListCtrl.SetItemText(index, 1, OPCODE);
		m_CDisasmDlg->m_cListCtrl.SetItemText(index++, 2, Instruct);
		
		nCount += nLen; 
		disAsm.EIP += nLen;
		disAsm.VirtualAddr += nLen; 
	}
}

void CPEInfoDlg::HideConvertGroup()
{
	m_edit_VA.ShowWindow(SW_HIDE);
	m_edit_RVA.ShowWindow(SW_HIDE);
    m_edit_FOA.ShowWindow(SW_HIDE);
	GetDlgItem(IDC_STATIC_1)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_STATIC2)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_STATIC_3)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_STATIC_4)->ShowWindow(SW_HIDE);
	m_IsConverting = false;
}

BOOL CPEInfoDlg::PreTranslateMessage(MSG* pMsg)
{
	if (WM_KEYDOWN == pMsg->message && VK_RETURN == pMsg->wParam)
	{
		PIMAGE_DOS_HEADER pImageDosHeader = (PIMAGE_DOS_HEADER)FileAddress;
		PIMAGE_FILE_HEADER pImageFileHeader = (PIMAGE_FILE_HEADER)((DWORD)pImageDosHeader + pImageDosHeader->e_lfanew + sizeof(DWORD));
		PIMAGE_OPTIONAL_HEADER pImageOptionalHeader = (PIMAGE_OPTIONAL_HEADER)((DWORD)pImageFileHeader + sizeof(IMAGE_FILE_HEADER));
		CString VA;
		CString RVA;
		CString FOA;
		DWORD dwVA;
		DWORD dwRVA;
		DWORD dwFOA;
		if (GetFocus() == &m_edit_VA)
		{
			UpdateData(TRUE);
			m_edit_VA.GetWindowText(VA);
			dwVA = CStringHexToInt(VA);
			dwRVA = dwVA - pImageOptionalHeader->ImageBase;
			dwFOA = RVA_TO_FOA(dwRVA);
			VA.Format(_T("%08X"), dwVA);
			RVA.Format(_T("%08X"), dwRVA);
			FOA.Format(_T("%08X"), dwFOA);
			m_edit_VA.SetWindowText(VA);
			m_edit_RVA.SetWindowText(RVA);
			m_edit_FOA.SetWindowText(FOA);
			m_HexEditorDlg->m_HexEditor.SetPosition(dwFOA);
			UpdateData(FALSE);
		}
		if (GetFocus() == &m_edit_RVA)
		{
			UpdateData(TRUE);
			m_edit_RVA.GetWindowText(RVA);
			dwRVA = CStringHexToInt(RVA);
			dwVA = pImageOptionalHeader->ImageBase + dwRVA;
			dwFOA = RVA_TO_FOA(dwRVA);
			VA.Format(_T("%08X"), dwVA);
			RVA.Format(_T("%08X"), dwRVA);
			FOA.Format(_T("%08X"), dwFOA);
			m_edit_VA.SetWindowText(VA);
			m_edit_RVA.SetWindowText(RVA);
			m_edit_FOA.SetWindowText(FOA);
			m_HexEditorDlg->m_HexEditor.SetPosition(dwFOA);
			UpdateData(FALSE);
		}
		if (GetFocus() == &m_edit_FOA)
		{
			UpdateData(TRUE);
			m_edit_FOA.GetWindowText(FOA);
			dwFOA = CStringHexToInt(FOA);
			dwRVA = FOA_TO_RVA(dwFOA);
			dwVA = dwRVA + pImageOptionalHeader->ImageBase;
			VA.Format(_T("%08X"), dwVA);
			RVA.Format(_T("%08X"), dwRVA);
			FOA.Format(_T("%08X"), dwFOA);
			m_edit_VA.SetWindowText(VA);
			m_edit_RVA.SetWindowText(RVA);
			m_edit_FOA.SetWindowText(FOA);
			m_HexEditorDlg->m_HexEditor.SetPosition(dwFOA);
			UpdateData(FALSE);
		}
		return true;
	}
	return CDialogEx::PreTranslateMessage(pMsg);
}
