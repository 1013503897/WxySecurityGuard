#include "pch.h"
#include "CSecurityGuard.h"
#include "CDlgInject.h"
#include "afxdialogex.h"

IMPLEMENT_DYNAMIC(CDlgInject, CDialogEx)

CDlgInject::CDlgInject(CWnd* pParent)
	: CDialogEx(IDD_DIALOG_INJECT, pParent)
	, m_strFilePath(_T(""))
	, m_nInjectType(0)
	, m_PidAndName(_T(""))
	, m_TipMsg(_T(""))
{
}

CDlgInject::~CDlgInject()
{
}

void CDlgInject::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_strFilePath);
	DDX_Radio(pDX, IDC_RADIO1, m_nInjectType);
	DDX_Text(pDX, IDC_EDIT4, m_PidAndName);
	DDX_Text(pDX, IDC_EDIT2, m_TipMsg);
	DDX_Control(pDX, IDC_EDIT2, m_objEdit);
}


BEGIN_MESSAGE_MAP(CDlgInject, CDialogEx)
	ON_WM_DROPFILES()
	ON_BN_CLICKED(IDC_BUTTON_START, &CDlgInject::OnBnClickedButtonStart)
	ON_BN_CLICKED(IDC_RADIO1, &CDlgInject::OnBnClickedRadio1)
	ON_BN_CLICKED(IDC_RADIO2, &CDlgInject::OnBnClickedRadio1)
	ON_BN_CLICKED(IDC_RADIO3, &CDlgInject::OnBnClickedRadio1)
	ON_BN_CLICKED(IDC_RADIO4, &CDlgInject::OnBnClickedRadio1)
END_MESSAGE_MAP()

void CDlgInject::OnDropFiles(HDROP hDropInfo)
{
	TCHAR szPath[MAX_PATH] = { 0 };
	DragQueryFile(hDropInfo, 0, szPath, MAX_PATH);

	LPTSTR pszExtension = PathFindExtension(szPath);
	if (lstrcmp(pszExtension, L".dll") == 0)
	{
		m_strFilePath = szPath;
		UpdateData(FALSE);
	}
	else
	{
		MessageBox(L"请拖入有效的DLL文件");
	}
	CDialogEx::OnDropFiles(hDropInfo);
}


void CDlgInject::OnBnClickedButtonStart()
{
	if (GetFileAttributes(m_strFilePath) == INVALID_FILE_ATTRIBUTES)
	{
		MessageBox(L"文件不存在 请重试");
		return;
	}
	UpdateData(TRUE);
	switch (m_nInjectType)
	{

	case 0:
	{
		if (m_strFilePath == L"" || m_strFilePath == L"请拖入MyHookDll.dll")
		{
			MessageBox(L"请拖入有效的DLL文件");
		}
		else
		{
			SetGlobalHook();
		}
	}
	break;
	case 1:
	{
		if (m_strFilePath == L"" || m_strFilePath == L"请拖入TestDll.dll")
		{
			MessageBox(L"请拖入有效的DLL文件");
		}
		else if (m_PidAndName == L"请输入需要注入的进程ID" || m_PidAndName == "")
		{
			MessageBox(L"请输入需要注入的进程ID");
		}
		else
		{
			USES_CONVERSION;
			char* szDllPath = T2A(m_strFilePath);
			DWORD dwProcessId = _ttoi(m_PidAndName);

			BOOL bIsSuccess = ZwCreateThreadExInjectDll(dwProcessId, szDllPath);
			if (bIsSuccess)
			{
				m_TipMsg += L"远程线程注入成功\r\n";
			}
			else
			{
				m_TipMsg += L"远程线程注入失败\r\n";
			}
		}

	}
	break;
	case 2:
	{
		if (m_strFilePath == L"" || m_strFilePath == L"请拖入TestDll.dll")
		{
			MessageBox(L"请拖入有效的DLL文件");
		}
		else if (m_PidAndName == L"请输入需要注入的进程ID" || m_PidAndName == "")
		{
			MessageBox(L"请输入需要注入的进程ID");
		}
		else
		{
			USES_CONVERSION;
			char* szProcessName = T2A(m_PidAndName);
			char* szDllPath = T2A(m_strFilePath);
			BOOL bRet = FALSE;
			DWORD dwProcessId = _ttoi(m_PidAndName);
			bRet = APCInject(dwProcessId, szDllPath);
			if (bRet)
			{
				m_TipMsg += L"APC注入成功\r\n";
				m_objEdit.LineScroll(m_objEdit.GetLineCount());
			}
			else
			{
				m_TipMsg += L"APC注入失败\r\n";
				m_objEdit.LineScroll(m_objEdit.GetLineCount());
			}
		}
	}
	break;
	default:
		break;
	}
	UpdateData(FALSE);
}

BOOL CDlgInject::ZwCreateThreadExInjectDll(DWORD dwProcessId, char* pszDllFileName)
{
	HANDLE hProcess = NULL;
	SIZE_T dwSize = 0;
	LPVOID pDllAddr = NULL;
	FARPROC pFunProcAddr = NULL;
	HANDLE hRemoteThread = NULL;
	DWORD dwStatus = 0;

	hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwProcessId);
	if (hProcess == NULL)
	{
		m_TipMsg += L"打开进程失败\r\n";
		return FALSE;
	}
	else
	{
		m_TipMsg += L"打开进程成功\r\n";
	}

	dwSize = strlen(pszDllFileName) + 1;
	pDllAddr = VirtualAllocEx(hProcess, NULL, dwSize, MEM_COMMIT, PAGE_READWRITE);
	if (pDllAddr == NULL)
	{
		m_TipMsg += L"申请内存失败\r\n";
		return FALSE;
	}
	else
	{
		m_TipMsg += L"申请内存成功\r\n";
	}

	BOOL bIsSucess = WriteProcessMemory(hProcess, pDllAddr, pszDllFileName, dwSize, NULL);
	if (bIsSucess == FALSE)
	{
		m_TipMsg += L"写入内存失败\r\n";
		return FALSE;
	}
	else
	{
		m_TipMsg += L"写入内存成功\r\n";
	}

	HMODULE hNtdll = LoadLibraryA("ntdll.dll");
	if (hNtdll == NULL)
	{
		m_TipMsg += L"加载ntdll失败\r\n";
		return FALSE;
	}
	else
	{
		m_TipMsg += L"加载ntdll成功\r\n";
	}

	pFunProcAddr = GetProcAddress(GetModuleHandleA("Kernel32.dll"), "LoadLibraryA");
	if (pFunProcAddr == NULL)
	{
		m_TipMsg += L"加载LoadLibraryA函数地址失败\r\n";
		return FALSE;
	}
	else
	{
		m_TipMsg += L"加载LoadLibraryA函数地址成功\r\n";
	}

#ifdef _WIN64
	typedef DWORD(WINAPI* typedef_ZwCreateThreadEx)(
		PHANDLE ThreadHandle,
		ACCESS_MASK DesiredAccess,
		LPVOID ObjectAttributes,
		HANDLE ProcessHandle,
		LPTHREAD_START_ROUTINE lpStartAddress,
		LPVOID lpParameter,
		ULONG CreateThreadFlags,
		SIZE_T ZeroBits,
		SIZE_T StackSize,
		SIZE_T MaximumStackSize,
		LPVOID pUnkown);
#else
	typedef DWORD(WINAPI* typedef_ZwCreateThreadEx)(
		PHANDLE ThreadHandle,
		ACCESS_MASK DesiredAccess,
		LPVOID ObjectAttributes,
		HANDLE ProcessHandle,
		LPTHREAD_START_ROUTINE lpStartAddress,
		LPVOID lpParameter,
		BOOL CreateSuspended,
		DWORD dwStackSize,
		DWORD dw1,
		DWORD dw2,
		LPVOID pUnkown);
#endif
	typedef_ZwCreateThreadEx ZwCreateThreadEx = (typedef_ZwCreateThreadEx)GetProcAddress(hNtdll, "ZwCreateThreadEx");
	if (ZwCreateThreadEx == NULL)
	{
		m_TipMsg += L"加载ZwCreateThreadEx函数地址失败\r\n";
		return FALSE;
	}
	else
	{
		m_TipMsg += L"加载ZwCreateThreadEx函数地址成功\r\n";
	}
	dwStatus = ZwCreateThreadEx(&hRemoteThread, THREAD_ALL_ACCESS, NULL,
		hProcess, (LPTHREAD_START_ROUTINE)pFunProcAddr, pDllAddr, 0, 0, 0, 0, NULL);
	if (hRemoteThread == NULL)
	{
		m_TipMsg += L"远程线程注入失败\r\n";
		return FALSE;
	}
	else
	{
		m_TipMsg += L"远程线程注入成功\r\n";
	}

	CloseHandle(hProcess);
	FreeLibrary(hNtdll);
	return TRUE;
}


void CDlgInject::OnBnClickedRadio1()
{
	UpdateData(TRUE);
	switch (m_nInjectType)
	{
	case 0:
	{
		m_strFilePath = L"请拖入MyHookDll.dll";
		GetDlgItem(IDC_EDIT4)->ShowWindow(FALSE);
		GetDlgItem(IDC_STATIC2)->ShowWindow(FALSE);
		m_TipMsg = "";
	}
	break;
	case 1:
	{
		m_strFilePath = L"请拖入TestDll.dll";
		m_PidAndName = L"请输入需要注入的进程ID";
		GetDlgItem(IDC_EDIT4)->ShowWindow(TRUE);
		GetDlgItem(IDC_STATIC2)->ShowWindow(TRUE);
		m_TipMsg = "";
	}
	break;
	case 2:
	{
		m_strFilePath = L"请拖入TestDll.dll";
<<<<<<< HEAD
		//m_Edit = L"C:\\Users\\GuiShou\\Desktop\\TestDll.dll";
		m_PidAndName = L"请输入需要注入的进程ID";
		//m_PidAndName = L"QQ.exe";
=======
		m_PidAndName = L"请输入需要注入的进程ID";
>>>>>>> v1.0
		GetDlgItem(IDC_EDIT4)->ShowWindow(TRUE);
		GetDlgItem(IDC_STATIC2)->ShowWindow(TRUE);
		m_TipMsg = "";
	}
	break;
	}
	UpdateData(FALSE);
}

void CDlgInject::SetGlobalHook()
{
	typedef BOOL(*typedef_SetGlobalHook)();
	typedef_SetGlobalHook fnSetGlobalHook = NULL;

	HMODULE hDll = LoadLibrary(m_strFilePath);
	if (hDll)
	{
		m_TipMsg += L"DLL加载成功\r\n";
	}
	else
	{
		m_TipMsg += L"DLL加载失败\r\n";
	}

	fnSetGlobalHook = (typedef_SetGlobalHook)GetProcAddress(hDll, "SetGlobalHook");
	if (fnSetGlobalHook)
	{
		m_TipMsg += L"加载函数地址成功\r\n";
	}
	else
	{
		m_TipMsg += L"加载函数地址失败\r\n";
	}
	BOOL bRet = fnSetGlobalHook();
	if (bRet)
	{
		m_TipMsg += L"设置全局钩子成功 开始无限弹框\r\n";
	}
	else
	{
		m_TipMsg += L"设置全局钩子失败\r\n";
	}
	UpdateData(FALSE);
}

BOOL CDlgInject::APCInject(DWORD dwProcessId, char* pszDllName)
{
	BOOL bRet = FALSE;
	DWORD* pThreadId = NULL;
	DWORD dwThreadIdLength = 0;
	HANDLE hProcess = NULL;
	HANDLE hThread = NULL;
	PVOID pBaseAddress = NULL;
	PVOID pLoadLibraryAFunc = NULL;
	SIZE_T dwRet = 0;
	DWORD dwDllPathLen = strlen(pszDllName) + 1;
	DWORD i = 0;

	do
	{
		bRet = GetAllTidByPid(dwProcessId, &pThreadId, &dwThreadIdLength);
		if (bRet == FALSE)
<<<<<<< HEAD
		{
=======
		{	
>>>>>>> v1.0
			m_TipMsg += L"获取所有线程ID失败\r\n";
			bRet = FALSE;
			break;
		}
		else
		{
			m_TipMsg += L"获取所有线程ID成功\r\n";
		}

		hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwProcessId);
		if (hProcess == NULL)
		{
			m_TipMsg += L"打开注入进程失败\r\n";
			bRet = FALSE;
			break;
		}
		else
		{
			m_TipMsg += L"打开注入进程成功\r\n";
		}
		pBaseAddress = VirtualAllocEx(hProcess, NULL, dwDllPathLen, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
		if (pBaseAddress == NULL)
		{
			m_TipMsg += L"在注入进程申请内存失败\r\n";
			bRet = FALSE;
			break;
		}
		else
		{
			m_TipMsg += L"在注入进程申请内存成功\r\n";
		}
		WriteProcessMemory(hProcess, pBaseAddress, pszDllName, dwDllPathLen, &dwRet);
		if (dwRet != dwDllPathLen)
		{
			m_TipMsg += L"在注入进程写入内存失败\r\n";
			bRet = FALSE;
			break;
		}
		else
		{
			m_TipMsg += L"在注入进程写入内存成功\r\n";
		}
		pLoadLibraryAFunc = GetProcAddress(GetModuleHandleA("kernel32.dll"), "LoadLibraryA");
		if (pLoadLibraryAFunc == NULL)
		{
			m_TipMsg += L"获取LoadLibrary地址失败\r\n";
			bRet = FALSE;
			break;
		}
		else
		{
			m_TipMsg += L"获取LoadLibrary地址成功\r\n";
		}

		for (i = 0; i < dwThreadIdLength; i++)
		{
			hThread = OpenThread(THREAD_ALL_ACCESS, FALSE, pThreadId[i]);
			if (hThread)
			{
				QueueUserAPC((PAPCFUNC)pLoadLibraryAFunc, hThread, (ULONG_PTR)pBaseAddress);
				CloseHandle(hThread);
				hThread = NULL;
			}
		}
		bRet = TRUE;
	} while (FALSE);

	if (hProcess)
	{
		CloseHandle(hProcess);
		hProcess = NULL;
	}
	if (hThread)
	{
		delete[] pThreadId;
		pThreadId = NULL;
	}
	return bRet;
}

BOOL CDlgInject::GetAllTidByPid(DWORD dwProcessId, DWORD** ppThreadId, DWORD* pdwThreadIdLength)
{
	DWORD* pThreadId = NULL;
	DWORD dwThreadIdLength = 0;
	DWORD dwBuffLength = 1000;
	THREADENTRY32 te32 = { 0 };
	HANDLE hSnapShot = NULL;
	BOOL bRet = TRUE;

	do
	{
		pThreadId = new DWORD[dwBuffLength];
		if (pThreadId == NULL)
		{
			m_TipMsg += L"申请内存失败!\r\n";
			bRet = FALSE;
			break;
		}
		else
		{
			m_TipMsg += L"申请内存成功!\r\n";
		}
		RtlZeroMemory(pThreadId, (dwBuffLength * sizeof(DWORD)));

		RtlZeroMemory(&te32, sizeof(te32));
		te32.dwSize = sizeof(te32);
		hSnapShot = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, 0);
		if (hSnapShot == NULL)
		{
			m_TipMsg += L"创建线程快照失败!\r\n";
			bRet = FALSE;
			break;
		}
		else
		{
			m_TipMsg += L"创建线程快照成功!\r\n";
		}

		bRet = Thread32First(hSnapShot, &te32);
		while (bRet)
		{
			if (te32.th32OwnerProcessID == dwProcessId)
			{
				pThreadId[dwThreadIdLength] = te32.th32ThreadID;
				dwThreadIdLength++;
			}
			bRet = Thread32Next(hSnapShot, &te32);
		}
		*ppThreadId = pThreadId;
		*pdwThreadIdLength = dwThreadIdLength;
		bRet = TRUE;

	} while (FALSE);

	if (FALSE == bRet)
	{
		if (pThreadId)
		{
			delete[] pThreadId;
			pThreadId = NULL;
		}
	}
	return bRet;
}


BOOL CDlgInject::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	GetDlgItem(IDC_EDIT4)->ShowWindow(FALSE);
	GetDlgItem(IDC_STATIC2)->ShowWindow(FALSE);
	return TRUE;  
}
