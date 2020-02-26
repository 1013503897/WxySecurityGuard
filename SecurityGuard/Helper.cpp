#pragma once
#include "pch.h"
#include "Helper.h"
#include <tchar.h>

LPFN_NTQUERYINFORMATIONTHREAD	__NtQueryInformationThread = NULL;
LPFN_NTQUERYSYSTEMINFORMATION	__NtQuerySystemInformation = NULL;
LPFN_ZWQUERYOBJECT				__ZwQueryObject = NULL;

double FILETIME2Double(const _FILETIME& filetime)
{
	return double(filetime.dwHighDateTime * 4.294967296e9)
		+ double(filetime.dwLowDateTime);
}

void WxyEnumProcess(vector<PPROCESS_INFO>& ProcessInfoVec)
{
	if (!ProcessInfoVec.empty())
	{
		for (auto& it : ProcessInfoVec)
		{
			if (it != nullptr)
			{
				delete it;
				it = nullptr;
			}
		}
		ProcessInfoVec.clear();
	}
	PROCESSENTRY32 ProcessEntry32 = { 0 };
	ProcessEntry32.dwSize = sizeof(PROCESSENTRY32);

	HANDLE Toolhelp32SnapshotHandle = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

	if (Toolhelp32SnapshotHandle == INVALID_HANDLE_VALUE)
		return;
	BOOL IsOk = Process32First(Toolhelp32SnapshotHandle, &ProcessEntry32);

	while (IsOk)
	{
		if (ProcessEntry32.th32ProcessID == 0)
		{
			IsOk = Process32Next(Toolhelp32SnapshotHandle, &ProcessEntry32);
			continue;
		}
		PPROCESS_INFO ProcessInfo = new PROCESS_INFO();
		ProcessInfo->ProcessID = ProcessEntry32.th32ProcessID;
		HANDLE ProcessHandle = OpenProcess(PROCESS_QUERY_LIMITED_INFORMATION, FALSE, ProcessInfo->ProcessID);
		if (!ProcessHandle)
		{
			_tcscpy(ProcessInfo->ProcessBitInfo, _T("UNKOWN"));
		}
		else
		{
			BOOL IsX64Process;
			if (!IsWow64Process(ProcessHandle, &IsX64Process))
				_tcscpy(ProcessInfo->ProcessBitInfo, _T("UNKOWN"));
			if (IsX64Process)
				_tcscpy(ProcessInfo->ProcessBitInfo, _T("x32"));
			else
				_tcscpy(ProcessInfo->ProcessBitInfo, _T("x64"));
		}
		ProcessInfo->ThreadNum = ProcessEntry32.cntThreads;
		WxyGetProcessFilePath(ProcessInfo->ProcessID, ProcessInfo->FileFullPath);
		SHFILEINFO FileInfo;
		DWORD_PTR dwRet = ::SHGetFileInfo(ProcessInfo->FileFullPath, 0, &FileInfo, sizeof(SHFILEINFO), SHGFI_ICON);
		if (dwRet)
		{
			ProcessInfo->ProcessIcon = FileInfo.hIcon;
		}
		else
		{
			ProcessInfo->ProcessIcon = NULL;
		}
		_tcscpy_s(ProcessInfo->ProcessName, MAX_PATH, ProcessEntry32.szExeFile);
		DWORD BufferLength = GetFileVersionInfoSize(ProcessInfo->FileFullPath, NULL);
		char* BufferData;
		BufferData = new char[BufferLength + 1];
		if (GetFileVersionInfo(ProcessInfo->FileFullPath, NULL, BufferLength, BufferData))
		{
			LPVOID lpBuffer = NULL;
			UINT uLen = 0;
			struct LANGANDCODEPAGE {
				WORD wLanguage;
				WORD wCodePage;
			} *lpTranslate;
			VerQueryValue(BufferData,
				_T("\\VarFileInfo\\Translation"),
				(LPVOID*)&lpTranslate,
				&uLen);
			for (UINT i = 0; i < (uLen / sizeof(struct LANGANDCODEPAGE)); i++)
			{
				TCHAR SubBlock[0x50];
				StringCchPrintf(SubBlock, 0x50,
					_T("\\StringFileInfo\\%04x%04x\\FileDescription"),
					lpTranslate[i].wLanguage,
					lpTranslate[i].wCodePage);
				if (VerQueryValue(BufferData, SubBlock, &lpBuffer, &uLen))
				{
					memcpy_s(ProcessInfo->FileDescription, 0x50 * sizeof(TCHAR), lpBuffer, 0x50 * sizeof(TCHAR));
				}
				TCHAR SubBlock1[0x50];
				StringCchPrintf(SubBlock1, 0x50,
					_T("\\StringFileInfo\\%04x%04x\\CompanyName"),
					lpTranslate[i].wLanguage,
					lpTranslate[i].wCodePage);
				if (VerQueryValue(BufferData, SubBlock1, &lpBuffer, &uLen))
				{
					memcpy_s(ProcessInfo->CompanyName, 0x50 * sizeof(TCHAR), lpBuffer, 0x50 * sizeof(TCHAR));
				}
			}
			delete[]BufferData;
		}
		ProcessInfoVec.push_back(ProcessInfo);
		IsOk = Process32Next(Toolhelp32SnapshotHandle, &ProcessEntry32);
	}
	CloseHandle(Toolhelp32SnapshotHandle);
}

void WxyGetProcessFilePath(DWORD ProcessId, CString& FileFullPath)
{
	FileFullPath = _T("");
	TCHAR tsFileDosPath[MAX_PATH + 1]{};
	HANDLE ProcessHandle = OpenProcess(PROCESS_QUERY_INFORMATION, NULL, ProcessId);
	if (ProcessHandle == NULL)
	{
		return;
	}
	if (!GetProcessImageFileName(ProcessHandle, tsFileDosPath, MAX_PATH + 1))
	{
		return;
	}

	UINT uiLen = GetLogicalDriveStrings(0, NULL);
	if (!uiLen)
	{
		return;
	}

	PTSTR pLogicDriveString = new TCHAR[uiLen + 1];
	ZeroMemory(pLogicDriveString, uiLen + 1);
	uiLen = GetLogicalDriveStrings(uiLen, pLogicDriveString);
	if (!uiLen)
	{
		delete[]pLogicDriveString;
		return;
	}

	TCHAR szDrive[3] = TEXT(" :");
	PTSTR pDosDriveName = new TCHAR[MAX_PATH];
	PTSTR pLogicIndex = pLogicDriveString;

	do
	{
		szDrive[0] = *pLogicIndex;
		uiLen = QueryDosDevice(szDrive, pDosDriveName, MAX_PATH);
		if (0 == uiLen)
		{
			if (ERROR_INSUFFICIENT_BUFFER != GetLastError())
			{
				break;
			}
			delete[]pDosDriveName;
			pDosDriveName = new TCHAR[uiLen + 1];
			uiLen = QueryDosDevice(szDrive, pDosDriveName, uiLen + 1);
			if (!uiLen)
			{
				break;
			}
		}

		uiLen = _tcslen(pDosDriveName);
		if (!_tcsnicmp(tsFileDosPath, pDosDriveName, uiLen))
		{
			FileFullPath.Format(_T("%s%s"), szDrive, tsFileDosPath + uiLen);
			break;
		}
		while (*pLogicIndex++);
	} while (*pLogicIndex);
	delete[]pLogicDriveString;
	delete[]pDosDriveName;
}

void WxyEnumModule(DWORD ProcessID, vector<PMODULE_INFO>& ModuleInfoVector)
{
	if (!ModuleInfoVector.empty())
	{
		for (auto& it : ModuleInfoVector)
		{
			if (it != nullptr)
			{
				delete it;
				it = nullptr;
			}
		}
		ModuleInfoVector.clear();
	}
	HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, ProcessID);
	if (INVALID_HANDLE_VALUE == hSnapshot)
	{
		return;
	}
	MODULEENTRY32 mi;
	mi.dwSize = sizeof(MODULEENTRY32);
	BOOL bRet = Module32First(hSnapshot, &mi);
	while (bRet)
	{
		PMODULE_INFO ModuleInfo = new MODULE_INFO;
		memcpy(ModuleInfo->FileFullPath, mi.szExePath, MAX_PATH * sizeof(TCHAR));
		TCHAR* pLast = mi.szExePath + _tcslen(mi.szExePath);
		while (true)
		{
			if (*pLast == _T('\\'))
			{
				break;
			}
			pLast--;
		}
		memcpy(ModuleInfo->ModuleName, ++pLast, MAX_PATH * sizeof(TCHAR));
		ModuleInfo->ModuleSize = mi.modBaseSize;
		ModuleInfo->ImageBase = (DWORD)mi.modBaseAddr;
		DWORD BufferLength = GetFileVersionInfoSize(ModuleInfo->FileFullPath, NULL);
		char* BufferData;
		BufferData = new char[BufferLength + 1];
		if (GetFileVersionInfo(ModuleInfo->FileFullPath, NULL, BufferLength, BufferData))
		{
			LPVOID lpBuffer = NULL;
			UINT uLen = 0;
			struct LANGANDCODEPAGE {
				WORD wLanguage;
				WORD wCodePage;
			} *lpTranslate;
			VerQueryValue(BufferData,
				_T("\\VarFileInfo\\Translation"),
				(LPVOID*)&lpTranslate,
				&uLen);
			for (UINT i = 0; i < (uLen / sizeof(struct LANGANDCODEPAGE)); i++)
			{
				TCHAR SubBlock[0x50];
				StringCchPrintf(SubBlock, 0x50,
					_T("\\StringFileInfo\\%04x%04x\\FileDescription"),
					lpTranslate[i].wLanguage,
					lpTranslate[i].wCodePage);
				if (VerQueryValue(BufferData, SubBlock, &lpBuffer, &uLen))
				{
					memcpy(ModuleInfo->FileDescription, lpBuffer, (_tcsclen((TCHAR*)lpBuffer) + 1) * sizeof(TCHAR));
				}
				TCHAR SubBlock1[0x50];
				StringCchPrintf(SubBlock1, 0x50,
					_T("\\StringFileInfo\\%04x%04x\\CompanyName"),
					lpTranslate[i].wLanguage,
					lpTranslate[i].wCodePage);
				if (VerQueryValue(BufferData, SubBlock1, &lpBuffer, &uLen))
				{
					int a = _tcsclen((TCHAR*)lpBuffer);
					memcpy(ModuleInfo->CompanyName, lpBuffer, (_tcsclen((TCHAR*)lpBuffer) + 1) * sizeof(TCHAR));
				}
			}
			delete[]BufferData;
		}
		ModuleInfoVector.push_back(ModuleInfo);
		bRet = Module32Next(hSnapshot, &mi);
	}
	return;
}

void WxyEnumThread(DWORD ProcessID, vector<PTHREAD_INFO>& ThreadInfo)
{
	for (auto it : ThreadInfo)
	{
		delete it;
	}
	ThreadInfo.clear();
	HANDLE hThreadSnap = INVALID_HANDLE_VALUE;
	THREADENTRY32 te32;
	hThreadSnap = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, ProcessID);
	if (hThreadSnap == INVALID_HANDLE_VALUE)
		return;
	te32.dwSize = sizeof(THREADENTRY32);
	if (!Thread32First(hThreadSnap, &te32))
	{
		printf("Thread32First");
		CloseHandle(hThreadSnap);
		return;
	}
	do
	{
		if (te32.th32OwnerProcessID == ProcessID)
		{
			PTHREAD_INFO thread_info = new THREAD_INFO;
			thread_info->ThreadID = te32.th32ThreadID;
			thread_info->ThreadPriorty = te32.tpBasePri;
			WxyGetThreadInfoByThreadId(thread_info->ThreadID, thread_info);
			ThreadInfo.push_back(thread_info);
		}
	} while (Thread32Next(hThreadSnap, &te32));
	CloseHandle(hThreadSnap);
	return;
}

void WxyEnumHeap(DWORD ProcessID, vector<PHEAP_INFO>& HeapInfoVector)
{
	if (!HeapInfoVector.empty())
	{
		for (auto it : HeapInfoVector)
		{
			if (it != nullptr)
			{
				delete it;
				it = nullptr;
			}
		}
		HeapInfoVector.clear();
	}
	HEAPLIST32 hl;

	HANDLE hHeapSnap = CreateToolhelp32Snapshot(TH32CS_SNAPHEAPLIST, ProcessID);

	hl.dwSize = sizeof(HEAPLIST32);

	if (hHeapSnap == INVALID_HANDLE_VALUE)
	{
		return;
	}
	if (Heap32ListFirst(hHeapSnap, &hl))
	{
		do
		{
			PHEAP_INFO HeapInfo = new HEAP_INFO;
			HeapInfo->HeapID = hl.th32HeapID;
			HeapInfoVector.push_back(HeapInfo);
			HEAPENTRY32 he;
			ZeroMemory(&he, sizeof(HEAPENTRY32));
			he.dwSize = sizeof(HEAPENTRY32);
			if (Heap32First(&he, ProcessID, hl.th32HeapID))
			{
				do
				{
					HeapInfo->BlockSize += he.dwBlockSize;
				} while (Heap32Next(&he));
			};
			hl.dwSize = sizeof(HEAPLIST32);
		} while (Heap32ListNext(hHeapSnap, &hl));
	}

	CloseHandle(hHeapSnap);
}

void WxyGetThreadInfoByThreadId(DWORD ThreadID, PTHREAD_INFO ThreadInfo)
{
	HANDLE ThreadHandle = NULL;
	ThreadHandle = OpenThread(THREAD_QUERY_INFORMATION, FALSE, ThreadID);
	if (!ThreadHandle)
	{
		return;
	}
	ThreadInfo->ThreadPriorty = WxyGetThreadPriority(ThreadHandle);
	FILETIME CreateTime;
	FILETIME ExitTime;
	FILETIME KernelTime;
	FILETIME UserTime;
	if (!GetThreadTimes(ThreadHandle, &CreateTime, &ExitTime, &KernelTime, &UserTime))
	{
		CloseHandle(ThreadHandle);
		return;
	}
	SYSTEMTIME SystemTime;
	FileTimeToSystemTime(&CreateTime, &SystemTime);
	ThreadInfo->CreateTime = SystemTime;

	ULONG_PTR ThreadStartAddress = NULL;

	if (__NtQueryInformationThread == NULL)
	{
		__NtQueryInformationThread = (LPFN_NTQUERYINFORMATIONTHREAD)GetProcAddress(GetModuleHandle(_T("ntdll.dll")), "NtQueryInformationThread");
	}
	NTSTATUS NtStatus = __NtQueryInformationThread(ThreadHandle, ThreadQuerySetWin32StartAddress,
		&ThreadStartAddress, sizeof(ULONG_PTR), NULL);
	if (!NT_SUCCESS(NtStatus))
	{
		CloseHandle(ThreadHandle);
		return;
	}
	ThreadInfo->ThreadStartAddress = ThreadStartAddress;
	CloseHandle(ThreadHandle);
}

KPRIORITY WxyGetThreadPriority(HANDLE ThreadHandle)
{
	NTSTATUS Status;
	THREAD_BASIC_INFORMATION ThreadBasicInfo;
	int ReturnValue;
	if (__NtQueryInformationThread == NULL)
	{
		__NtQueryInformationThread = (LPFN_NTQUERYINFORMATIONTHREAD)GetProcAddress(GetModuleHandle(_T("ntdll.dll")), "NtQueryInformationThread");
	}
	Status = __NtQueryInformationThread(
		ThreadHandle,
		ThreadBasicInformation,
		&ThreadBasicInfo,
		sizeof(ThreadBasicInfo),
		NULL
	);
	if (!NT_SUCCESS(Status))
	{
		return (int)THREAD_PRIORITY_ERROR_RETURN;
	}

	ReturnValue = (int)ThreadBasicInfo.BasePriority;
	if (ReturnValue == ((HIGH_PRIORITY + 1) / 2))
	{
		ReturnValue = THREAD_PRIORITY_TIME_CRITICAL;
	}
	else if (ReturnValue == -((HIGH_PRIORITY + 1) / 2))
	{
		ReturnValue = THREAD_PRIORITY_IDLE;
	}
	return ReturnValue;
}

BOOL WxyIsThreadExist(DWORD ThreadID)
{
	BOOL IsOk = FALSE;
	DWORD ExitCode = 0;

	HANDLE ThreadHandle = OpenThread(THREAD_QUERY_INFORMATION, FALSE, ThreadID);
	if (ThreadHandle != NULL)
	{
		if (GetExitCodeThread(ThreadHandle, &ExitCode))
		{
			if (ExitCode == STILL_ACTIVE)
				IsOk = TRUE;
		}
		CloseHandle(ThreadHandle);
	}
	return IsOk;
}

VOID WxyEnableSeDebugPrivilege()
{
	DWORD  LastError;
	HANDLE TokenHandle = 0;

	if (!OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &TokenHandle))
	{
		LastError = GetLastError();
		if (TokenHandle)
			CloseHandle(TokenHandle);
		return;
	}
	TOKEN_PRIVILEGES TokenPrivileges;
	memset(&TokenPrivileges, 0, sizeof(TOKEN_PRIVILEGES));
	LUID v1;
	if (!LookupPrivilegeValue(NULL, SE_DEBUG_NAME, &v1))
	{
		LastError = GetLastError();
		CloseHandle(TokenHandle);
		return;
	}
	TokenPrivileges.PrivilegeCount = 1;
	TokenPrivileges.Privileges[0].Luid = v1;
	TokenPrivileges.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
	AdjustTokenPrivileges(TokenHandle, FALSE, &TokenPrivileges, sizeof(TOKEN_PRIVILEGES), NULL, NULL);
	LastError = GetLastError();
	CloseHandle(TokenHandle);
	return;
}

void WxyEnumHandle(DWORD ProcessID, vector<PHANDLE_INFO>& HandleInfoVector)
{
	ULONG BufferLength = 0x4000;
	ULONG ReturnLength = 0;
	HMODULE ModuleHandle = NULL;
	HANDLE  ProcessHandle = INVALID_HANDLE_VALUE;
	POBJECT_TYPE_INFORMATION ObjectTypeInfo = { 0 };
	HANDLE   v2 = 0;
	NTSTATUS Status;

	PSYSTEM_HANDLE_INFORMATION SystemHandleInfo = (PSYSTEM_HANDLE_INFORMATION)malloc(BufferLength);
	PSYSTEM_HANDLE_INFORMATION v1 = NULL;
	POBJECT_NAME_INFORMATION ObjectNameInfo = NULL;
	HandleInfoVector.clear();
	ModuleHandle = GetModuleHandle(_T("Ntdll.dll"));
	if (ModuleHandle == NULL)
	{
		goto Exit;
	}
	__NtQuerySystemInformation = (LPFN_NTQUERYSYSTEMINFORMATION)GetProcAddress(ModuleHandle,
		"NtQuerySystemInformation");

	if (__NtQuerySystemInformation == NULL)
	{
		goto Exit;
	}
	__ZwQueryObject = (LPFN_ZWQUERYOBJECT)GetProcAddress(ModuleHandle, "ZwQueryObject");
	if (__ZwQueryObject == NULL)
	{
		goto Exit;
	}

	ProcessHandle = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_DUP_HANDLE, FALSE, ProcessID);
	if (ProcessHandle == NULL)
	{
		ProcessHandle = OpenProcess(PROCESS_QUERY_LIMITED_INFORMATION | PROCESS_DUP_HANDLE, 0, ProcessID);
	}
	if (ProcessHandle == NULL)
	{
		goto Exit;
	}
	while (TRUE)
	{
		Status = __NtQuerySystemInformation(SystemHandleInformation, SystemHandleInfo,
			BufferLength, &ReturnLength);
		if (Status != STATUS_SUCCESS)
		{
			if (Status == STATUS_INFO_LENGTH_MISMATCH)
			{
				free(SystemHandleInfo);
				BufferLength = BufferLength * 2;
				SystemHandleInfo = (PSYSTEM_HANDLE_INFORMATION)malloc(BufferLength);
				if (!SystemHandleInfo)
				{
					goto Exit;
				}
			}
			else
			{
				goto Exit;
			}
		}
		else
		{
			break;
		}
	}
	v1 = (PSYSTEM_HANDLE_INFORMATION)(SystemHandleInfo);

	LONG NumberOfHandles = SystemHandleInfo->NumberOfHandles;//¾ä±úÊý

	ObjectTypeInfo = (POBJECT_TYPE_INFORMATION)malloc(0x4000);
	memset(ObjectTypeInfo, 0, 0x4000);
	ObjectNameInfo = (POBJECT_NAME_INFORMATION)malloc(0x4000);
	memset(ObjectNameInfo, 0, 0x4000);
	for (int i = 0; i < NumberOfHandles; i++)
	{
		if (v1->Handles[i].UniqueProcessId != (USHORT)ProcessID)
		{
			continue;
		}
		if (DuplicateHandle(ProcessHandle, (HANDLE)v1->Handles[i].HandleValue, GetCurrentProcess(), &v2, 0, FALSE, DUPLICATE_SAME_ACCESS))
		{
			if (v2 == 0)
			{
				continue;
			}
			Status = __ZwQueryObject(v2, ObjectTypeInformation, ObjectTypeInfo, 0x4000, NULL);
			Status = __ZwQueryObject(v2, ObjectNameInformation, ObjectNameInfo, 0x4000, NULL);
			if (Status != STATUS_SUCCESS)
			{
				continue;
			}
			PHANDLE_INFO HandleInfo = new HANDLE_INFO;
			HandleInfo->HandleValue = (USHORT)v2;
			HandleInfo->Object = v1->Handles[i].Object;
			HandleInfo->ObjectTypeIndex = v1->Handles[i].ObjectTypeIndex;
			if (ObjectNameInfo->Name.Length != 0)
				memcpy(HandleInfo->HandleName, ObjectNameInfo->Name.Buffer, ObjectNameInfo->Name.Length * sizeof(WCHAR));
			if (ObjectTypeInfo->TypeName.Length != 0)
				memcpy(HandleInfo->ObjectTypeName, ObjectTypeInfo->TypeName.Buffer, ObjectTypeInfo->TypeName.Length * sizeof(TCHAR));
			HandleInfoVector.push_back(HandleInfo);

			memset(ObjectTypeInfo, 0, 0x1000);
			memset(ObjectNameInfo, 0, 0x1000);
		}
	}
Exit:
	if (ObjectTypeInfo != NULL)
	{
		free(ObjectTypeInfo);
		ObjectTypeInfo = NULL;
	}
	if (ObjectNameInfo != NULL)
	{
		free(ObjectNameInfo);
		ObjectNameInfo = NULL;
	}
	if (SystemHandleInfo != NULL)
	{
		free(SystemHandleInfo);
		SystemHandleInfo = NULL;
	}
	if (ProcessHandle != NULL)
	{
		CloseHandle(ProcessHandle);
		ProcessHandle = NULL;
	}
}

VOID WxyGetDeviceInfo(PDEVICE_INFO DeviceInfo)
{
	DWORD Size = MAX_PATH;
	GetComputerName(DeviceInfo->ComputerName, &Size);
	GetUserName(DeviceInfo->UserName, &Size);
	CString strPath = CString("HARDWARE\\DESCRIPTION\\System\\CentralProcessor\\0");
	CRegKey regKey;
	LONG lResult;
	lResult = regKey.Open(HKEY_LOCAL_MACHINE, strPath, KEY_READ);
	if (lResult == ERROR_SUCCESS)
	{
		TCHAR chCPUName[256] = { 0 };
		DWORD dwSize = 256;
		regKey.QueryStringValue(_T("ProcessorNameString"), DeviceInfo->CPUName, &dwSize);
		regKey.QueryDWORDValue(_T("~MHz"), DeviceInfo->CPUClockSpeed);
		regKey.Close();
	}
	SYSTEM_INFO si;
	memset(&si, 0, sizeof(SYSTEM_INFO));
	GetSystemInfo(&si);
	DeviceInfo->NumberOfProcessors = si.dwNumberOfProcessors;
	_FILETIME idleTime, kernelTime, userTime;
	GetSystemTimes(&idleTime, &kernelTime, &userTime);
	HANDLE hEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
	WaitForSingleObject(hEvent, 1000);
	_FILETIME newIdleTime, newKernelTime, newUsertime;
	GetSystemTimes(&newIdleTime, &newKernelTime, &newUsertime);
	double dOldIdleTime = FILETIME2Double(idleTime);
	double dNewdleTime = FILETIME2Double(newIdleTime);
	double dOldKernelTime = FILETIME2Double(kernelTime);
	double dNewKernelTime = FILETIME2Double(newKernelTime);
	double dOldUserTime = FILETIME2Double(userTime);
	double dNewUserTime = FILETIME2Double(newUsertime);
	DeviceInfo->CpuUsage = int(100.0 - (dNewdleTime - dOldIdleTime)
		/ (dNewKernelTime - dOldKernelTime + dNewUserTime - dOldUserTime) * 100.0);
	MEMORYSTATUSEX memStatus = { 0 };
	memStatus.dwLength = sizeof(MEMORYSTATUSEX);
	GlobalMemoryStatusEx(&memStatus);
	DeviceInfo->MemoryLoad = memStatus.dwMemoryLoad;
	DeviceInfo->MemoryUsage = (memStatus.ullTotalPhys - memStatus.ullAvailPhys) / (1024);
}

VOID WxyGetFileInfo(TCHAR* FileFullPath, vector<PFILE_INFO>& FileInfoVector)
{
	TCHAR TmpFileFullPath[MAX_PATH] = { 0 };
	memcpy(TmpFileFullPath, FileFullPath, MAX_PATH * sizeof(TCHAR));
	if (TmpFileFullPath[0] != _T('\\'))
		_tcscat(TmpFileFullPath, _T("\\"));
	_tcscat(TmpFileFullPath, _T("*.*"));
	CFileFind FileFind;
	BOOL res = FileFind.FindFile(TmpFileFullPath);

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
		if (FileFind.IsDirectory() && !FileFind.IsDots())
		{
			PFILE_INFO FileInfo = new FILE_INFO{ 0 };
			FileInfoVector.push_back(FileInfo);
			SHFILEINFO SHFileInfo = { 0 };
			::SHGetFileInfo(strPath, 0, &SHFileInfo, sizeof(SHFILEINFO), SHGFI_ICON);
			FileInfo->FileIcon = SHFileInfo.hIcon;
			memcpy(FileInfo->FileName, strTitle, MAX_PATH);

			FILETIME ftCreate;
			FileFind.GetLastWriteTime(&ftCreate);

			SYSTEMTIME stUTC1, stLocal1;
			FileTimeToSystemTime(&ftCreate, &stUTC1);
			SystemTimeToTzSpecificLocalTime(NULL, &stUTC1, &stLocal1);
			CString LastWriteTime;
			LastWriteTime.Format(_T("%02d/%02d/%d  %02d:%02d"), stLocal1.wDay,
				stLocal1.wMonth, stLocal1.wYear,
				stLocal1.wHour, stLocal1.wMinute);
			memcpy(FileInfo->FileLastChangeTime, LastWriteTime, 50);
		}
		else if (!FileFind.IsDirectory() && !FileFind.IsDots())
		{
			TCHAR FilePath[MAX_PATH];
			_tcscpy(FilePath, strPath);
			HANDLE FileHandle = CreateFile(FilePath, GENERIC_READ,
				FILE_SHARE_READ, 0, OPEN_EXISTING, 0, 0);
			if (FileHandle == INVALID_HANDLE_VALUE)
			{
				continue;
			}
			PFILE_INFO FileInfo = new FILE_INFO{ 0 };
			FileInfoVector.push_back(FileInfo);
			SHFILEINFO SHFileInfo = { 0 };
			::SHGetFileInfo(strPath, 0, &SHFileInfo, sizeof(SHFILEINFO), SHGFI_ICON);
			FileInfo->FileIcon = SHFileInfo.hIcon;
			FileInfo->m_FileSize = FileFind.GetLength();
			memcpy(FileInfo->FileName, strTitle, MAX_PATH);
			FILETIME ftCreate;
			FileFind.GetLastWriteTime(&ftCreate);
			SYSTEMTIME stUTC1, stLocal1;

			FileTimeToSystemTime(&ftCreate, &stUTC1);
			SystemTimeToTzSpecificLocalTime(NULL, &stUTC1, &stLocal1);
			CString LastWriteTime;
			LastWriteTime.Format(_T("%02d/%02d/%d  %02d:%02d"), stLocal1.wDay,
				stLocal1.wMonth, stLocal1.wYear,
				stLocal1.wHour, stLocal1.wMinute);
			memcpy(FileInfo->FileLastChangeTime, LastWriteTime, 50);
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
			_tcscpy(FileInfo->MD5, md5);
		}
	}
}

BOOL CALLBACK WxyEnumWndProc(HWND hwnd, LPARAM lParam)
{
	vector<PWND_INFO>* WndInfoVec = (vector<PWND_INFO>*)lParam;
	PWND_INFO WndInfo = new WND_INFO;
	GetWindowText(hwnd, WndInfo->WndName, MAX_PATH);
	GetClassName(hwnd, WndInfo->ClassName, MAX_PATH);
	GetWindowThreadProcessId((HWND)hwnd, &WndInfo->ProcessID);
	if (_tcsclen(WndInfo->WndName) != 0)
	{
		WndInfo->IsVisable = IsWindowVisible(hwnd);
		WndInfoVec->push_back(WndInfo);
	}
	else
	{
		if (WndInfo)
		{
			delete WndInfo;
			WndInfo = nullptr;
		}
	}
	return TRUE;
}

CString Int2CString(int data, int length)
{
	CString format;
	format.Format(_T("%%0%dX"), length);
	CString ret;
	ret.Format(format, data);
	return ret;
}

CString Int2CString(int data)
{
	CString ret;
	ret.Format(_T("%X"), data);
	return ret;
}

DWORD WxyGetCPUUsage(_FILETIME& idleTime, _FILETIME& kernelTime, _FILETIME& userTime)
{
	_FILETIME newIdleTime, newKernelTime, newUsertime;
	GetSystemTimes(&newIdleTime, &newKernelTime, &newUsertime);
	double dOldIdleTime = FILETIME2Double(idleTime);
	double dNewdleTime = FILETIME2Double(newIdleTime);
	double dOldKernelTime = FILETIME2Double(kernelTime);
	double dNewKernelTime = FILETIME2Double(newKernelTime);
	double dOldUserTime = FILETIME2Double(userTime);
	double dNewUserTime = FILETIME2Double(newUsertime);
	idleTime = newIdleTime;
	kernelTime = newKernelTime;
	userTime = newUsertime;
	return  DWORD(100.0 - (dNewdleTime - dOldIdleTime) / (dNewKernelTime - dOldKernelTime + dNewUserTime - dOldUserTime) * 100.0);
}

void WxyEnumServers(vector<PSERVER_INFO>& ServerInfo)
{
	for (auto it : ServerInfo)
	{
		delete it;
	}
	ServerInfo.clear();
	SC_HANDLE sc, hServer;
	sc = ::OpenSCManager(NULL, NULL, SC_MANAGER_ENUMERATE_SERVICE);
	if (sc != NULL)
	{
		ENUM_SERVICE_STATUS service_data, * lpservice;
		BOOL retVal;
		DWORD bytesNeeded, srvCount, resumeHandle = 0, srvType, srvState;
		srvType = SERVICE_WIN32;
		srvState = SERVICE_STATE_ALL;
		retVal = ::EnumServicesStatus(sc, srvType, srvState, &service_data, sizeof(service_data),
			&bytesNeeded, &srvCount, &resumeHandle);
		DWORD err = GetLastError();
		if ((retVal == FALSE) || err == ERROR_MORE_DATA)
		{
			DWORD dwBytes = bytesNeeded + sizeof(ENUM_SERVICE_STATUS);
			lpservice = new ENUM_SERVICE_STATUS[dwBytes];
			if (!EnumServicesStatus(sc, srvType, srvState, lpservice, dwBytes,
				&bytesNeeded, &srvCount, &resumeHandle))
			{
				return;
			}
			for (int i = 0; i < srvCount; i++)
			{
				DWORD dwBytesNeeded, cbBufSize, dwError;
				LPQUERY_SERVICE_CONFIG lpsc;
				LPSERVICE_DESCRIPTION lpsd;
				SC_HANDLE hService = OpenService(sc,
					lpservice[i].lpServiceName,
					SERVICE_QUERY_CONFIG | SERVICE_QUERY_STATUS);
				if (!QueryServiceConfig(
					hService,
					NULL,
					0,
					&dwBytesNeeded))
				{
					dwError = GetLastError();
					if (ERROR_INSUFFICIENT_BUFFER == dwError)
					{
						cbBufSize = dwBytesNeeded;
						lpsc = (LPQUERY_SERVICE_CONFIG)LocalAlloc(LMEM_FIXED, cbBufSize);
						if (!QueryServiceConfig(
							hService,
							lpsc,
							cbBufSize,
							&dwBytesNeeded))
						{
							return;
						}
						else
						{
							if (!QueryServiceConfig2(
								hService,
								SERVICE_CONFIG_DESCRIPTION,
								NULL,
								0,
								&dwBytesNeeded))
							{
								dwError = GetLastError();
								if (ERROR_INSUFFICIENT_BUFFER == dwError)
								{
									cbBufSize = dwBytesNeeded;
									lpsd = (LPSERVICE_DESCRIPTION)LocalAlloc(LMEM_FIXED, cbBufSize);
									if (!QueryServiceConfig2(
										hService,
										SERVICE_CONFIG_DESCRIPTION,
										(LPBYTE)lpsd,
										cbBufSize,
										&dwBytesNeeded))
									{
										return;
									}
									PSERVER_INFO pServerInfo = new SERVER_INFO{ 0 };
									SERVICE_STATUS serviceStatus = { 0 };
									if (true == QueryServiceStatus(hService, &serviceStatus))
									{

										if (serviceStatus.dwCurrentState == SERVICE_RUNNING)
										{
											pServerInfo->IsRunning = TRUE;
										}
										else
										{
											pServerInfo->IsRunning = FALSE;
										}
									}
									else
									{
										dwError = GetLastError();
									}
									_tcscpy(pServerInfo->ServerName, lpservice[i].lpServiceName);
									_tcscpy(pServerInfo->DisPlayName, lpservice[i].lpDisplayName);
									pServerInfo->StartType = lpsc->dwStartType;
									_tcscpy(pServerInfo->BinaryPsth, lpsc->lpBinaryPathName);

									if (lpsd->lpDescription != NULL && lstrcmp(lpsd->lpDescription, TEXT("")) != 0)
										_tcscpy(pServerInfo->Description, lpsd->lpDescription);
									LocalFree(lpsc);
									LocalFree(lpsd);
									ServerInfo.push_back(pServerInfo);
								}
								else
								{
									return;
								}
							}
						}
					}
					else
					{
						return;
					}
				}
			}
		}
	}
}

void md5_encrypt(const void* data, size_t len, CString& ret)
{
	unsigned char hex[100] = { 0 };
	MD5_CTX ctx = { 0 };
	MD5_Init(&ctx);
	MD5_Update(&ctx, data, len);
	MD5_Final(hex, &ctx);
	CString tmp;
	for (int i = 0; i < 16; i++)
	{
		tmp.Format(_T("%02x"), hex[i]);
		ret += tmp;
	}
}

void md4_encrypt(const void* data, size_t len, CString& ret)
{
	unsigned char hex[100] = { 0 };
	TCHAR md4[100] = { 0 };
	MD4_CTX ctx = { 0 };
	MD4_Init(&ctx);
	MD4_Update(&ctx, data, len);
	MD4_Final(hex, &ctx);
	CString tmp;
	for (int i = 0; i < 16; i++)
	{
		tmp.Format(_T("%02X"), hex[i]);
		ret += tmp;
	}
}

void sha1_encrypt(const void* data, size_t len, CString& ret)
{
	unsigned char hex[100] = { 0 };
	SHA_CTX ctx = { 0 };
	SHA1_Init(&ctx);
	SHA1_Update(&ctx, data, len);
	SHA1_Final(hex, &ctx);
	CString tmp;
	for (int i = 0; i < 20; i++)
	{
		tmp.Format(_T("%02X"), hex[i]);
		ret += tmp;
	}
}

void ProcessProtect(PVOID param)
{
	while (true)
	{
		//TaskMgr.exe
		HANDLE Toolhelp32SnapshotHandle = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
		PROCESSENTRY32 ProcessEntry32 = { 0 };
		ProcessEntry32.dwSize = sizeof(PROCESSENTRY32);
		if (Toolhelp32SnapshotHandle == INVALID_HANDLE_VALUE)
			return;
		BOOL IsOk = Process32First(Toolhelp32SnapshotHandle, &ProcessEntry32);
		while (IsOk)
		{
			if (ProcessEntry32.th32ProcessID == 0)
			{
				IsOk = Process32Next(Toolhelp32SnapshotHandle, &ProcessEntry32);
				continue;
			}
			if (!_tcscmp(ProcessEntry32.szExeFile, _T("Taskmgr.exe")))
			{
				InjectPorcess(ProcessEntry32.th32ProcessID, DWORD(param));
				return;
			}

			IsOk = Process32Next(Toolhelp32SnapshotHandle, &ProcessEntry32);
		}
		CloseHandle(Toolhelp32SnapshotHandle);
	}
}

void InjectPorcess(DWORD InjectProcessID, DWORD ProtectProcessID)
{
<<<<<<< HEAD

=======
>>>>>>> v1.0
	char IngPath[MAX_PATH] = "D:\\source\\CSecurityGuard\\Debug\\InjectDLL.dll";
	DWORD m_SPid;
	m_SPid = GetCurrentProcessId();
	HANDLE hMap = CreateFileMapping(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, 0x10, L"Pid");
	LPVOID hP = MapViewOfFile(hMap, FILE_MAP_ALL_ACCESS, 0, 0, 0);
	memcpy_s(hP, 0x10, &m_SPid, sizeof(DWORD));

	HANDLE hprocess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, InjectProcessID);
<<<<<<< HEAD
	if (NULL == hprocess)
	{
		AfxMessageBox(L"OpenProcess() failure");
=======
	if (!hprocess)
	{
>>>>>>> v1.0
		return;
	}
	DWORD dwSize = strlen(IngPath) + 1;
	LPVOID lpBuf = VirtualAllocEx(hprocess, NULL, dwSize, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
	sizeof(&lpBuf);
	if (NULL == lpBuf)
	{
<<<<<<< HEAD
		AfxMessageBox(L"VirtualAllEx() failure");
=======
>>>>>>> v1.0
		return;
	}

	INT ErrorCode = WriteProcessMemory(hprocess, lpBuf, IngPath, dwSize, &dwSize);
	if (0 >= ErrorCode)
	{
<<<<<<< HEAD
		AfxMessageBox(L"WriteProcessMemory() failuer!");
=======
>>>>>>> v1.0
		VirtualFreeEx(hprocess, lpBuf, dwSize, MEM_RELEASE);
		CloseHandle(hprocess);
		return;
	}

	PVOID pRemoteProcess = CreateRemoteThreadEx(hprocess, NULL, NULL, (LPTHREAD_START_ROUTINE)LoadLibraryA, lpBuf, NULL, NULL, NULL);
<<<<<<< HEAD
	if (NULL == pRemoteProcess)
	{
		AfxMessageBox(L"CreateRemoteThreadEx() failuer");
=======
	if (!pRemoteProcess)
	{
>>>>>>> v1.0
		VirtualFreeEx(hprocess, lpBuf, dwSize, MEM_RELEASE);
		CloseHandle(hprocess);
		return;
	}
	WaitForSingleObject(pRemoteProcess, INFINITE);

	VirtualFreeEx(hprocess, lpBuf, dwSize, MEM_RELEASE);
	CloseHandle(pRemoteProcess);
	CloseHandle(hprocess);
}

void WxyEnumUninstall(vector<SOFT_INFO>& UninstallInfoVec)
{
	UninstallInfoVec.clear();
	HKEY RootKey = HKEY_LOCAL_MACHINE;
	LPCTSTR lpSubKey = _T("SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Uninstall");

	HKEY hkResult = 0;

	LONG lReturn = RegOpenKeyEx(HKEY_LOCAL_MACHINE,
		lpSubKey,
		0,
		KEY_ENUMERATE_SUB_KEYS | KEY_QUERY_VALUE,
		&hkResult);
	DWORD dwIndex = 0;
	while (true)
	{
		SOFT_INFO SoftInfo = { 0 };
		DWORD dwKeyLen = 255;
		WCHAR szNewKeyName[MAX_PATH] = {};
		LONG lReturn = RegEnumKeyEx(hkResult,
			dwIndex,
			szNewKeyName,
			&dwKeyLen,
			0,
			NULL,
			NULL,
			NULL);
		if (lReturn)
			break;
		WCHAR strMidReg[MAX_PATH] = {};
		swprintf_s(strMidReg, _T("%s%s%s"), lpSubKey, _T("\\"), szNewKeyName);

		HKEY hkValueKey = 0;
		RegOpenKeyEx(RootKey, strMidReg, 0, KEY_QUERY_VALUE, &hkValueKey);

		DWORD dwNameLen = 255;
		RegQueryValueEx(hkValueKey, _T("DisplayName"), 0, NULL,
			(LPBYTE)SoftInfo.szSoftName, &dwNameLen);
		if (*SoftInfo.szSoftName == 0)
		{
			dwIndex++;
			continue;
		}
		dwNameLen = 255;
		RegQueryValueEx(hkValueKey, _T("Publisher"), 0, NULL,
			(LPBYTE)SoftInfo.strSoftVenRe, &dwNameLen);
		dwNameLen = 255;

		RegQueryValueEx(hkValueKey, _T("UninstallString"), 0, NULL,
			(LPBYTE)SoftInfo.strSoftUniPath, &dwNameLen);
		if (*SoftInfo.strSoftUniPath == 0)
		{
			dwIndex++;
			continue;
		}
		dwIndex++;
		UninstallInfoVec.push_back(SoftInfo);
	}
}

void WxyEnumMsConfig(vector<PMSCONFIG_INFO>& ConfigInfoVec)
{
	HKEY indexs[] = { HKEY_CURRENT_USER,HKEY_LOCAL_MACHINE };
	vector<CString> KeyVec = { _T("Software\\Microsoft\\Windows\\CurrentVersion\\Run") ,
	_T("Software\\Microsoft\\Windows\\CurrentVersion\\RunOnce") ,
	_T("SOFTWARE\\WOW6432Node\\Microsoft\\Windows\\CurrentVersion\\Run") ,
	_T("S-1-5-19\\Software\\Microsoft\\Windows\\CurrentVersion\\Run") ,
	_T("Software\\Microsoft\\Windows\\CurrentVersion\\RunOnce\\Setup") ,
	_T("Software\\Microsoft\\Windows\\CurrentVersion\\Policies\\Explorer\\Run") ,
	_T("Software\\Microsoft\\Windows\\CurrentVersion\\RunOnceEx") ,
	_T("Software\\Microsoft\\Shared Tools\\MSConfig\\startupreg") ,
	_T("Software\\Microsoft\\Windows NT\\CurrentVersion\\Windows\\load") ,
	_T("Software\\Microsoft\\Windows NT\\CurrentVersion\\Winlogon\\Userinit") ,
	_T("Software\\Microsoft\\Windows NT\\CurrentVersion\\Winlogon")
	};
	HKEY hkey;
	DWORD SubKeyNumbers, MaxSubKeyLen, ValueNamubers, MaxValueNameLen = MAX_PATH, MaxValueDataLen = MAX_PATH; TCHAR szValueName[MAX_PATH] = { 0 }, path[MAX_PATH] = { 0 };
	TCHAR szData[MAX_PATH] = { 0 };
	DWORD lpType, type;
	for (int j = 0; j < 2; ++j)
	{
		for (int i = 0; i < KeyVec.size(); ++i)
		{
			if (ERROR_SUCCESS == RegOpenKeyEx(indexs[j], KeyVec[i],
				NULL, KEY_READ, &hkey))
			{
				if (ERROR_SUCCESS == RegQueryInfoKey(hkey, NULL, NULL, NULL, &SubKeyNumbers, &MaxSubKeyLen, NULL, &ValueNamubers,
					&MaxValueNameLen, &MaxValueDataLen, NULL, NULL))
					if (ValueNamubers != 0)
					{
						for (int k = 0; k < ValueNamubers; ++k)
						{
							ZeroMemory(szData, sizeof(TCHAR) * MAX_PATH);

							long OpenK = RegEnumValue(hkey, k, szValueName, &MaxValueNameLen, NULL, &lpType, NULL, NULL);
							if (OpenK != ERROR_SUCCESS)
							{
								continue;
							}
							RegQueryValueEx(hkey, szValueName, NULL, &type, (LPBYTE)szData, &MaxValueDataLen);
							PMSCONFIG_INFO tmpKeyInfo = new MSCONFIG_INFO;
							_tcscpy(tmpKeyInfo->ConfigName, szValueName);
							TCHAR* tmp = szData;
							if (*szData == _T('\"'))
							{
								tmp++;
								TCHAR* end = tmp;
								while (*end != _T('\"') && end != szData + MAX_PATH - 1)
									end++;
								if (end != szData + MAX_PATH - 1)
									*end = 0;
								_tcscpy(tmpKeyInfo->FileFullPath, tmp);
							}
							else if (*szData >= _T('C') && *szData <= _T('Z'))
							{
								_tcscpy(tmpKeyInfo->FileFullPath, tmp);
							}
							else
							{
								continue;
							}
							CString Key;
							if (indexs[j] == HKEY_CURRENT_USER)
							{
								Key = _T("HKEY_CURRENT_USER\\");
							}
							else
							{
								Key = _T("HKEY_LOCAL_MACHINE\\");
							}
							Key += KeyVec[i];
							_tcscpy(tmpKeyInfo->Key, Key);
							DWORD BufferLength = GetFileVersionInfoSize(tmpKeyInfo->FileFullPath, NULL);
							char* BufferData;
							BufferData = new char[BufferLength + 1];
							BOOL IsOk2 = GetFileVersionInfo(tmpKeyInfo->FileFullPath, NULL, BufferLength, BufferData);
							if (IsOk2)
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
										TEXT("\\StringFileInfo\\%04x%04x\\FileDescription"),
										lpTranslate[i].wLanguage,
										lpTranslate[i].wCodePage);
									IsOk2 = VerQueryValue(BufferData, SubBlock, &lpBuffer, &uLen);
									if (IsOk2)
									{
										memcpy_s(tmpKeyInfo->Description, 0x50 * sizeof(TCHAR), lpBuffer, 0x50 * sizeof(TCHAR));
									}
									TCHAR SubBlock1[0x50];
									StringCchPrintf(SubBlock1, 0x50,
										TEXT("\\StringFileInfo\\%04x%04x\\CompanyName"),
										lpTranslate[i].wLanguage,
										lpTranslate[i].wCodePage);
									IsOk2 = VerQueryValue(BufferData, SubBlock1, &lpBuffer, &uLen);
									if (IsOk2)
									{
										memcpy_s(tmpKeyInfo->CompanyName, 0x50 * sizeof(TCHAR), lpBuffer, 0x50 * sizeof(TCHAR));
									}
								}
								delete[]BufferData;
							}
							MaxValueNameLen = MAX_PATH;
							MaxValueDataLen = MAX_PATH;
							ConfigInfoVec.push_back(tmpKeyInfo);
						}
					}
				RegCloseKey(hkey);
			}
		}
	}
}

void WxyEnumMemory(DWORD ProcessID, PPROCESS_MEMORY_COUNTERS MemoryInfo)
{
	PROCESS_MEMORY_COUNTERS pmc;

	HANDLE ProcessHandle = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, ProcessID);
	if (NULL == ProcessHandle)
		return;
	if (GetProcessMemoryInfo(ProcessHandle, &pmc, sizeof(pmc)))
	{
		memcpy(MemoryInfo, &pmc, sizeof(PROCESS_MEMORY_COUNTERS));
	}
	CloseHandle(ProcessHandle);
}

int evp_en_cipher(unsigned char* source_string, unsigned char* des_string, int length)
{
	EVP_CIPHER_CTX* ctx = EVP_CIPHER_CTX_new();
	EVP_CIPHER_CTX_set_padding(ctx, 1);

	int once_length = 0, out_length = 0;

	unsigned char key[16] = "15pb";
	EVP_CipherInit_ex(ctx, EVP_des_ede3_ecb(), NULL, key, nullptr, 1);

	EVP_CipherUpdate(ctx, des_string, &once_length, source_string, length);
	out_length += once_length;
	EVP_CipherFinal(ctx, des_string + once_length, &once_length);
	out_length += once_length;

	EVP_CIPHER_CTX_free(ctx);
	return out_length;
}

int evp_de_cipher(unsigned char* source_string, unsigned char* des_string, int length)
{
	EVP_CIPHER_CTX* ctx = EVP_CIPHER_CTX_new();
	EVP_CIPHER_CTX_set_padding(ctx, 1);

	int once_length = 0, out_length = 0;

	unsigned char key[16] = "15pb";
	EVP_CipherInit_ex(ctx, EVP_des_ede3_ecb(), NULL, key, nullptr, 0);

	EVP_CipherUpdate(ctx, des_string, &once_length, source_string, length);
	out_length += once_length;
	EVP_CipherFinal(ctx, des_string + once_length, &once_length);
	out_length += once_length;

	des_string[out_length] = 0;

	EVP_CIPHER_CTX_free(ctx);
	return out_length;
}

DWORD RvaToOffset(PBYTE pPeFile, DWORD Rva)
{
	PIMAGE_DOS_HEADER pDos = (PIMAGE_DOS_HEADER)pPeFile;
	PIMAGE_NT_HEADERS pNt = (PIMAGE_NT_HEADERS)(pPeFile + pDos->e_lfanew);
	PIMAGE_SECTION_HEADER pSection = IMAGE_FIRST_SECTION(pNt);
	for (int i = 0; i < pNt->FileHeader.NumberOfSections; i++)
	{
		if (Rva < pSection->VirtualAddress)
		{
			return Rva;
		}
		DWORD dwAlignment = pNt->OptionalHeader.SectionAlignment;
		DWORD dwCount = pSection->Misc.VirtualSize / dwAlignment;
		dwCount += (pSection->Misc.VirtualSize % dwAlignment == 0) ? 0 : 1;
		if (Rva >= pSection->VirtualAddress && Rva < pSection->VirtualAddress + dwCount * dwAlignment)
		{
			return Rva - pSection->VirtualAddress + pSection->PointerToRawData;
		}
		pSection++;
	}
	return 0;
}

void mytoupperA(char* str)
{
	for (int i = 0; i < strlen(str); i++)
	{
		if (str[i] >= 'a' && str[i] <= 'z')
		{
			str[i] = toupper(str[i]);
		}
	}
}

void mytoupper(TCHAR* str)
{
	for (int i = 0; i < _tcslen(str); i++)
	{
		if (str[i] >= _T('a') && str[i] <= _T('z'))
		{
#ifdef UNICODE
			str[i] = towupper(str[i]);
#else
			str[i] = toupper(str[i]);
#endif
		}
	}
}

bool isforwardstring(char* str)
{
	if (strlen(str) >= 99 || strlen(str) <= 3)
		return false;
	for (int i = 0; i < strlen(str); i++)
	{
		if ((str[i] >= 'A' && str[i] <= 'Z') || (str[i] >= 'a' && str[i] <= 'z') ||
			str[i] == '-' || str[i] == '_' || str[i] == '.' || (str[i] >= '0' && str[i] <= '9'))
			continue;
		else
		{
			return false;
		}
	}
	return true;
}

void WxyCheckIAT(DWORD dwProcessID, vector<PHOOK_INFO>& HookInfoVec)
{
	vector<MODULE_INFO> ModuleVec;
	HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwProcessID);
	if (hProcess == NULL)
		return;
	WxyGetRemoteModuleInfo(dwProcessID, ModuleVec);
	HMODULE hProcessImage = (HMODULE)ModuleVec[0].ImageBase;
	PBYTE pModuleBuf = NULL;
	DWORD dwReadModuleSize = 0;
	pModuleBuf = new BYTE[sizeof(IMAGE_DOS_HEADER)];
	ReadProcessMemory(hProcess, hProcessImage, pModuleBuf, sizeof(IMAGE_DOS_HEADER), &dwReadModuleSize);
	PIMAGE_DOS_HEADER pDos = (PIMAGE_DOS_HEADER)pModuleBuf;
	DWORD dwNewFile = pDos->e_lfanew;
	delete[]pModuleBuf;
	DWORD dwHeaderSize = dwNewFile + sizeof(IMAGE_NT_HEADERS);
	pModuleBuf = new BYTE[dwHeaderSize];
	ReadProcessMemory(hProcess, hProcessImage, pModuleBuf, dwHeaderSize, &dwReadModuleSize);
	PIMAGE_NT_HEADERS pNt = (PIMAGE_NT_HEADERS)(pModuleBuf + dwNewFile);
	PIMAGE_OPTIONAL_HEADER32 pOptionalHeader = &pNt->OptionalHeader;
	PIMAGE_DATA_DIRECTORY pImportDir = pOptionalHeader->DataDirectory + 1;
	DWORD dwImportVA = (DWORD)((DWORD)hProcessImage + pImportDir->VirtualAddress);
	DWORD dwSize = pImportDir->Size;
	delete[]pModuleBuf;
	PBYTE pImportBuf = new BYTE[dwSize];
	ReadProcessMemory(hProcess, (LPVOID)dwImportVA, pImportBuf, dwSize, &dwReadModuleSize);
	PIMAGE_IMPORT_DESCRIPTOR pImport = PIMAGE_IMPORT_DESCRIPTOR(pImportBuf);
	while (pImport->Name)
	{
		DWORD pIatRVA = (DWORD)(pImport->FirstThunk + (DWORD)hProcessImage);
		DWORD pIntRVA = (DWORD)(pImport->OriginalFirstThunk + (DWORD)hProcessImage);
		PIMAGE_THUNK_DATA pIat = (PIMAGE_THUNK_DATA)new BYTE[0x1000]{ 0 };
		PIMAGE_THUNK_DATA pInt = (PIMAGE_THUNK_DATA)new BYTE[0x1000]{ 0 };
		PIMAGE_THUNK_DATA OriginalPIat = pIat;
		PIMAGE_THUNK_DATA OriginalPInt = pInt;
		ReadProcessMemory(hProcess, (LPVOID)pIatRVA, pIat, 0x1000, &dwReadModuleSize);
		ReadProcessMemory(hProcess, (LPVOID)pIntRVA, pInt, 0x1000, &dwReadModuleSize);
		char pDllname[MAX_PATH]{ 0 };
		ReadProcessMemory(hProcess, (LPVOID)((DWORD)hProcessImage + pImport->Name), pDllname, MAX_PATH, &dwReadModuleSize);
		mytoupperA((char*)pDllname);
		HMODULE hModule = NULL;
		DWORD RemoteImageBase = NULL;
		for (int i = 0; i < ModuleVec.size(); i++)
		{
			USES_CONVERSION;
			CHAR FileFullPath[MAX_PATH]{};
			strcpy(FileFullPath, T2A(ModuleVec[i].FileFullPath));
			if (strstr(FileFullPath, (char*)pDllname))
			{
				if (strstr(FileFullPath, "WINSXS") || strstr(FileFullPath, "LIBTCMALLOC")) //|| strstr(FileFullPath, "ANTI") || strstr(FileFullPath, "SAFE") || strstr(FileFullPath, "PROTECT"))
				{
					goto Next_Dll;
				}
				RemoteImageBase = ModuleVec[i].ImageBase;
				hModule = LoadLibraryA(FileFullPath);
				break;
			}
		}
		if (!RemoteImageBase)
		{
			goto Next_Dll;
		}
		if (!hModule)
		{
			hModule = LoadLibraryA(pDllname);
			if (!hModule)
			{
				goto Next_Dll;
			}
		}
		while (pIat->u1.AddressOfData != 0)
		{
			DWORD ProcRVA = 0;
			DWORD LocalFunVA = 0;
			char* pFunnameVA;
			if (!IMAGE_SNAP_BY_ORDINAL32(pInt->u1.AddressOfData))
			{
				pFunnameVA = (char*)(pInt->u1.AddressOfData + (DWORD)hProcessImage);
				char pFunname[MAX_PATH] = { 0 };
				if (!ReadProcessMemory(hProcess, (LPVOID)pFunnameVA, pFunname, MAX_PATH, &dwReadModuleSize))
					goto Next_IAT;
				ProcRVA = WxyGetProcAddress((DWORD)hModule, (LPCSTR)PIMAGE_IMPORT_BY_NAME(pFunname)->Name);
			}
			else
			{
				DWORD Ordinal = pInt->u1.Ordinal & 0x0000FFFF;
				ProcRVA = WxyGetProcAddress((DWORD)hModule, (LPCSTR)Ordinal);
			}
			if (!ProcRVA)
			{
				goto Next_IAT;
			}
			if (ProcRVA + RemoteImageBase != pIat->u1.Function)
			{
				char pTempDll[100] = { 0 };
				char pTempFuction[100] = { 0 };
				LocalFunVA = ProcRVA + (DWORD)hModule;
				while (isforwardstring((char*)LocalFunVA))
				{
					strcpy(pTempDll, (char*)LocalFunVA);
					char* p = strchr(pTempDll, '.');
					*p = 0;
					strcpy(pTempFuction, p + 1);
					strcat(pTempDll, ".dll");
					DWORD lastProcRva = ProcRVA;
					HMODULE hModule = LoadLibraryA(pTempDll);
					if (!hModule)
					{
						goto Next_IAT;
					}
					ProcRVA = WxyGetProcAddress((DWORD)hModule, pTempFuction);
					if (!ProcRVA || ProcRVA == lastProcRva)
					{
						goto Next_IAT;
					}
					FreeLibrary(hModule);
				}
				if (ProcRVA + RemoteImageBase != pIat->u1.Function)
				{
					pFunnameVA = (char*)(pInt->u1.AddressOfData + (DWORD)hProcessImage);
					char pFunname[MAX_PATH] = { 0 };
					ReadProcessMemory(hProcess, (LPVOID)pFunnameVA, pFunname, MAX_PATH, &dwReadModuleSize);
					PHOOK_INFO HookInfo = new HOOK_INFO{};
					TCHAR ImageFileName[MAX_PATH] = { 0 };
					GetProcessImageFileName(hProcess, ImageFileName, MAX_PATH);
					_tcscpy(HookInfo->ProcessName, _tcsrchr(ImageFileName, '\\') + 1);
					CString SrcPlace;
					CString DstPlace;
					USES_CONVERSION;
					SrcPlace.Format(_T("%s + %s"), A2T(pDllname), A2T(pFunname + 2));
					_tcscpy(HookInfo->HookType, _T("IAT"));
					_tcscpy(HookInfo->SrcPlace, SrcPlace);
					for (auto it : ModuleVec)
					{
						if (pIat->u1.Function >= it.ImageBase && pIat->u1.Function <= it.ImageBase + it.ModuleSize)
						{
							char* FuncName = nullptr;
							if (!_tcscmp(it.ModuleName + _tcslen(it.ModuleName) - 4, _T(".exe")))
							{
								FuncName = new char[50]{};
								FuncName[0] = '0';
								FuncName[1] = 'x';
								_itoa(pIat->u1.Function - it.ImageBase, FuncName + 2, 16);
							}
							else
							{
								FuncName = WxyGetProcName(it.ModuleName, pIat->u1.Function - it.ImageBase);
							}
							USES_CONVERSION;
							DstPlace.Format(_T("%s + %s"), it.ModuleName, A2T(FuncName));
							_tcscpy(HookInfo->DstDll, it.FileFullPath);
							delete[] FuncName;
							break;
						}
					}
					_tcscpy(HookInfo->DstPlace, DstPlace);
					HookInfo->ProcessID = dwProcessID;
					HookInfo->SrcAddress = ProcRVA;
					HookInfo->DstAddress = pIat->u1.Function;
					HookInfoVec.push_back(HookInfo);
				}
			}
Next_IAT:
			pInt++;
			pIat++;
		}
		delete[] OriginalPIat;
		delete[] OriginalPInt;
		FreeLibrary(hModule);
		Next_Dll:
		pImport++;
	}
	delete[] pImportBuf;
}

void WxyCheckEAT(DWORD dwProcessID, vector<PHOOK_INFO>& HookInfoVec)	
{
	vector<MODULE_INFO> ModuleVec;
	HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwProcessID);
	WxyGetRemoteModuleInfo(dwProcessID, ModuleVec);
	for (auto it : ModuleVec)
	{
		HMODULE hRemoteImageBase = (HMODULE)it.ImageBase;
		PBYTE pModuleBuf = NULL;
		DWORD dwReadModuleSize = 0;
		pModuleBuf = new BYTE[sizeof(IMAGE_DOS_HEADER)];
		ReadProcessMemory(hProcess, hRemoteImageBase, pModuleBuf, sizeof(IMAGE_DOS_HEADER), &dwReadModuleSize);
		PIMAGE_DOS_HEADER pDos = (PIMAGE_DOS_HEADER)pModuleBuf;
		DWORD dwNewFile = pDos->e_lfanew;
		delete[] pModuleBuf;
		DWORD dwHeaderSize = dwNewFile + sizeof(IMAGE_NT_HEADERS);
		pModuleBuf = new BYTE[dwHeaderSize];
		ReadProcessMemory(hProcess, hRemoteImageBase, pModuleBuf, dwHeaderSize, &dwReadModuleSize);
		PIMAGE_NT_HEADERS pNt = (PIMAGE_NT_HEADERS)(pModuleBuf + dwNewFile);
		PIMAGE_OPTIONAL_HEADER32 pOptionalHeader = &pNt->OptionalHeader;
		PIMAGE_DATA_DIRECTORY pExportDir = pOptionalHeader->DataDirectory;
		if (!pExportDir->VirtualAddress)
		{
			continue;
		}
		HMODULE hModule = LoadLibrary(it.FileFullPath);
		if (!hModule)
		{
			hModule = LoadLibrary(it.ModuleName);
			if (!hModule) continue;
		}
		PIMAGE_EXPORT_DIRECTORY pExport = (PIMAGE_EXPORT_DIRECTORY)new BYTE[pExportDir->Size];
		ReadProcessMemory(hProcess, (LPVOID)((DWORD)hRemoteImageBase + pExportDir->VirtualAddress), pExport, pExportDir->Size, NULL);
		DWORD* pFunctions = new DWORD[pExport->NumberOfFunctions];
		ReadProcessMemory(hProcess, (LPVOID)((DWORD)hRemoteImageBase + pExport->AddressOfFunctions), pFunctions, sizeof(DWORD) * pExport->NumberOfFunctions, NULL);
		DWORD* pNames = new DWORD[pExport->NumberOfNames];
		ReadProcessMemory(hProcess, (LPVOID)((DWORD)hRemoteImageBase + pExport->AddressOfNames), pNames, sizeof(DWORD) * pExport->NumberOfNames, NULL);
		WORD* pOridianl = new WORD[pExport->NumberOfNames];
		ReadProcessMemory(hProcess, (LPVOID)((DWORD)hRemoteImageBase + pExport->AddressOfNameOrdinals), pOridianl, sizeof(WORD) * pExport->NumberOfNames, NULL);
		WORD Order = 0;
		for (int i = 0; i < pExport->NumberOfNames; i++)
		{
			char* pFunnameVA = (char*)(pNames[i] + (DWORD)hRemoteImageBase);
			char* pFunname = new char[MAX_PATH];
			DWORD FOAFunction;
			ReadProcessMemory(hProcess, (LPVOID)pFunnameVA, pFunname, MAX_PATH, &dwReadModuleSize);
			DWORD FuntionRVA = WxyGetProcAddress((DWORD)hModule, pFunname);
			if (!FuntionRVA)
				continue;
			if (FuntionRVA != pFunctions[pOridianl[i]])
			{
				PHOOK_INFO HookInfo = new HOOK_INFO{};
				TCHAR ImageFileName[MAX_PATH] = { 0 };
				GetProcessImageFileName(hProcess, ImageFileName, MAX_PATH);
				_tcscpy(HookInfo->ProcessName, _tcsrchr(ImageFileName, '\\') + 1);
				HookInfo->ProcessID = dwProcessID;
				HookInfo->SrcAddress = FuntionRVA + (DWORD)hModule;
				HookInfo->DstAddress = (DWORD)hRemoteImageBase + pFunctions[pOridianl[i]];
				_tcscpy(HookInfo->HookType, _T("EAT"));
				CString SrcPlace;
				CString DstPlace;
				USES_CONVERSION;
				SrcPlace.Format(_T("%s!%s"), it.ModuleName, A2T(pFunname));
				for (auto it2 : ModuleVec)
				{
					if ((DWORD)hRemoteImageBase + pFunctions[pOridianl[i]] >= it2.ImageBase && (DWORD)hRemoteImageBase + pFunctions[pOridianl[i]] <= it2.ImageBase + it2.ModuleSize)
					{
						char* FuncName = nullptr;
						if (!_tcscmp(it.ModuleName + _tcslen(it.ModuleName) - 4, _T(".exe")))
						{
							FuncName = new char[50]{};
							FuncName[0] = '0';
							FuncName[1] = 'x';
							_itoa((DWORD)hRemoteImageBase + pFunctions[pOridianl[i]] - it2.ImageBase, FuncName + 2, 16);
						}
						else
						{
							FuncName = WxyGetProcName(it2.FileFullPath, (DWORD)hRemoteImageBase + pFunctions[pOridianl[i]] - it2.ImageBase);
						}
						USES_CONVERSION;
						DstPlace.Format(_T("%s + %s"), it2.ModuleName, A2T(FuncName));
						_tcscpy(HookInfo->DstDll, it2.FileFullPath);
						delete[] FuncName;
						break;
					}
				}
				_tcscpy(HookInfo->DstPlace, DstPlace);
				_tcscpy(HookInfo->SrcPlace, SrcPlace);
				HookInfoVec.push_back(HookInfo);
			}
			delete[] pFunname;
		}
		FreeLibrary(hModule);
		delete[] pExport;
		delete[] pModuleBuf;
		delete[] pFunctions;
		delete[] pNames;
		delete[] pOridianl;
	}
}

void WxyCheckInline(DWORD dwProcessID, vector<PHOOK_INFO>& HookInfoVec)
{
	vector<MODULE_INFO> ModuleVec;
	WxyGetRemoteModuleInfo(dwProcessID, ModuleVec);
	HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwProcessID);
	if (hProcess == NULL)
		return;
	HMODULE hModule = WxyGetRemoteImageBase(dwProcessID);
	PBYTE pModuleBuf = NULL;
	DWORD dwReadModuleSize = 0;
	pModuleBuf = new BYTE[sizeof(IMAGE_DOS_HEADER)];
	ReadProcessMemory(hProcess, hModule, pModuleBuf, sizeof(IMAGE_DOS_HEADER), &dwReadModuleSize);
	PIMAGE_DOS_HEADER pDos = (PIMAGE_DOS_HEADER)pModuleBuf;
	DWORD dwNewFile = pDos->e_lfanew;
	delete[]pModuleBuf;
	DWORD dwHeaderSize = dwNewFile + sizeof(IMAGE_NT_HEADERS);
	pModuleBuf = new BYTE[dwHeaderSize];
	ReadProcessMemory(hProcess, hModule, pModuleBuf, dwHeaderSize, &dwReadModuleSize);
	PIMAGE_NT_HEADERS pNt = (PIMAGE_NT_HEADERS)(pModuleBuf + dwNewFile);
	PIMAGE_OPTIONAL_HEADER32 pOptionalHeader = &pNt->OptionalHeader;
	PIMAGE_DATA_DIRECTORY pImportDir = pOptionalHeader->DataDirectory + 1;
	DWORD dwImportVA = (DWORD)((DWORD)hModule + pImportDir->VirtualAddress);
	DWORD dwSize = pImportDir->Size;
	delete[]pModuleBuf;
	PBYTE pImportBuf = new BYTE[dwSize];
	ReadProcessMemory(hProcess, (LPVOID)dwImportVA, pImportBuf, dwSize, &dwReadModuleSize);
	PIMAGE_IMPORT_DESCRIPTOR pImport = PIMAGE_IMPORT_DESCRIPTOR(pImportBuf);
	while (pImport->Name != 0)
	{
		DWORD pIatRVA = (DWORD)(pImport->FirstThunk + (DWORD)hModule);
		DWORD pIntRVA = (DWORD)(pImport->OriginalFirstThunk + (DWORD)hModule);
		PIMAGE_THUNK_DATA pIat = (PIMAGE_THUNK_DATA)new BYTE[0x1000];
		PIMAGE_THUNK_DATA pInt = (PIMAGE_THUNK_DATA)new BYTE[0x1000]; 
		PIMAGE_THUNK_DATA OriginalPIat = pIat;
		PIMAGE_THUNK_DATA OriginalPInt = pInt;
		ReadProcessMemory(hProcess, (LPVOID)pIatRVA, pIat, 0x1000, &dwReadModuleSize);
		ReadProcessMemory(hProcess, (LPVOID)pIntRVA, pInt, 0x1000, &dwReadModuleSize);
		char* pDllnameVA = (char*)((DWORD)hModule + pImport->Name);
		char pDllname[MAX_PATH]{ 0 };
		ReadProcessMemory(hProcess, (LPVOID)pDllnameVA, pDllname, MAX_PATH, &dwReadModuleSize);
		while (pIat->u1.AddressOfData != 0)
		{
			DWORD RemoteFunVA = 0;
			DWORD LocalFunVA = 0;
			char* pFunnameVA;
			char* pFunname;
			char* pFunData = new char[5];
			ReadProcessMemory(hProcess, (LPVOID)pIat->u1.Function, pFunData, 5, &dwReadModuleSize);
			if (pFunData[0] == (char)0xE9 )
			{
				char pTempDll[100] = { 0 };
				char pTempFuction[100] = { 0 };
				pFunnameVA = (char*)(pInt->u1.AddressOfData + (DWORD)hModule);
				pFunname = new char[MAX_PATH];
				ReadProcessMemory(hProcess, (LPVOID)pFunnameVA, pFunname, MAX_PATH, &dwReadModuleSize);
				HMODULE hModule = LoadLibraryA(pDllname);

				if (hModule != NULL)
				{
					DWORD pFunData2 = (DWORD)GetProcAddress(hModule, pFunname + 2);
					if (pFunData2 != NULL && memcmp((char*)pFunData2, pFunData, 5))
					{
						PHOOK_INFO HookInfo = new HOOK_INFO{};
						TCHAR ImageFileName[MAX_PATH] = { 0 };
						GetProcessImageFileName(hProcess, ImageFileName, MAX_PATH);
						_tcscpy(HookInfo->ProcessName, _tcsrchr(ImageFileName, '\\') + 1);
						CString SrcPlace;
						CString DstPlace;
						USES_CONVERSION;
						SrcPlace.Format(_T("%s + %s"), A2T(pDllname), A2T(pFunname + 2));
						_tcscpy(HookInfo->HookType, _T("Inline"));
						_tcscpy(HookInfo->SrcPlace, SrcPlace);

						DWORD jmpOffset = 0;
						DWORD jmpAddr = pIat->u1.Function;
						while (pFunData[0] == (char)0xE9)
						{
							jmpOffset = *(int*)(pFunData + 1);
							jmpAddr = jmpAddr + jmpOffset + 5;
							ReadProcessMemory(hProcess, (LPVOID)jmpAddr, pFunData, 5, &dwReadModuleSize);
						}
						HookInfo->SrcAddress = pIat->u1.Function;
						HookInfo->DstAddress = jmpAddr;
						for (auto it : ModuleVec)
						{
							if (jmpAddr >= it.ImageBase && jmpAddr <= it.ImageBase + it.ModuleSize)
							{
								char* FuncName = nullptr;
								if (!_tcscmp(it.ModuleName + _tcslen(it.ModuleName) - 4, _T(".exe")))
								{
									FuncName = new char[50]{};
									FuncName[0] = '0';
									FuncName[1] = 'x';
									_itoa(jmpAddr - it.ImageBase, FuncName + 2, 16);
								}
								else
								{
									FuncName = WxyGetProcName(it.ModuleName, pIat->u1.Function - it.ImageBase);
								}
								USES_CONVERSION;
								_tcscpy(HookInfo->DstDll, it.FileFullPath);
								DstPlace.Format(_T("%s + %s"), it.ModuleName, A2T(FuncName));
								_tcscpy(HookInfo->DstPlace, DstPlace);
								delete[] FuncName;
								break;
							}
						}
						HookInfo->ProcessID = dwProcessID;
						HookInfo->SrcAddress = pIat->u1.Function;
						HookInfoVec.push_back(HookInfo);
					}
				}
				delete[] pFunname;
			}
			pInt++;
			pIat++;
		}
		delete[] OriginalPIat;
		delete[] OriginalPInt;
		pImport++;
	}
	delete[] pImportBuf;
}

HMODULE WxyGetRemoteImageBase(DWORD dwProcessID)
{
	HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwProcessID);
	DWORD dwNeed1 = 0;
	DWORD dwNeed2 = 0;
	EnumProcessModulesEx(hProcess, NULL, 0, &dwNeed1, LIST_MODULES_ALL);
	HMODULE* pModule = new HMODULE[dwNeed1]{};
	EnumProcessModulesEx(hProcess, pModule, dwNeed1, &dwNeed2, LIST_MODULES_ALL);
	return *pModule;
}

VOID WxyGetRemoteModuleInfo(DWORD dwProcessID, vector<MODULE_INFO>& ModuleVec)
{
	HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION, FALSE, dwProcessID);
	HANDLE hModuleSnap = INVALID_HANDLE_VALUE;
	MODULEENTRY32 me32;
	hModuleSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, dwProcessID);
	me32.dwSize = sizeof(MODULEENTRY32);

	if (!Module32First(hModuleSnap, &me32))
	{
		CloseHandle(hModuleSnap);
		return;
	}
	ModuleVec.clear();
	do
	{
		MODULE_INFO ModuleInfo{};
		_tcscpy(ModuleInfo.FileFullPath, me32.szExePath);
		mytoupper(ModuleInfo.FileFullPath);
		_tcscpy(ModuleInfo.ModuleName, me32.szModule);
		ModuleInfo.ModuleSize = me32.modBaseSize;
		ModuleInfo.ImageBase = (DWORD)me32.modBaseAddr;
		TCHAR* tmp = _tcsstr(ModuleInfo.FileFullPath, _T("ANTI"));
		if (tmp)
		{
			continue;
		}
		ModuleVec.push_back(ModuleInfo);
	} while (Module32Next(hModuleSnap, &me32));
	for (int i = 1; i < ModuleVec.size(); i++)
	{
		TCHAR* tmp = _tcsstr(ModuleVec[i].FileFullPath, _T("SYSTEM32"));
		if (!tmp)
		{
			continue;
		}
		_tcscpy(tmp, _T("SYSWOW64"));
		tmp[8] = _T('\\');
	}
	CloseHandle(hModuleSnap);
	CloseHandle(hProcess);
}

char* WxyGetProcName(TCHAR* FileFullPath, DWORD FuncAddr)
{
	DWORD hDllFile = (DWORD)LoadLibrary(FileFullPath);
	if (!hDllFile)
		return nullptr;
	PIMAGE_DOS_HEADER pDestanceDos = (PIMAGE_DOS_HEADER)hDllFile;
	PIMAGE_NT_HEADERS pDestanceNt = (PIMAGE_NT_HEADERS)(hDllFile + pDestanceDos->e_lfanew);
	PIMAGE_DATA_DIRECTORY pExportDir = &(pDestanceNt->OptionalHeader.DataDirectory[0]);
	PIMAGE_EXPORT_DIRECTORY pExport = (PIMAGE_EXPORT_DIRECTORY)(pExportDir->VirtualAddress + hDllFile);
	PINT pFunction = (PINT)(pExport->AddressOfFunctions + hDllFile);
	PINT pFunctionName = (PINT)(pExport->AddressOfNames + hDllFile);
	PWORD pFunctionOrdin = (PWORD)(pExport->AddressOfNameOrdinals + hDllFile);
	WORD Order = 0;

	char* pFunname = new char[MAX_PATH]{};
	pFunname[0] = '0';
	pFunname[1] = 'x';
	_itoa(FuncAddr, pFunname + 2, 16);
	for (int i = 0; i < pExport->NumberOfFunctions; i++)
	{
		if (FuncAddr == pFunction[i])
		{
			for (int j = 0; j < pExport->NumberOfNames; j++)
			{
				if (pFunctionOrdin[j] == i)
				{
					ZeroMemory(pFunname, MAX_PATH);
					if (strlen((char*)(pFunctionName[j]) + hDllFile) >= MAX_PATH)
						memcpy(pFunname, (char*)(pFunctionName[j]) + hDllFile, MAX_PATH - 1);
					else
						strcpy(pFunname, (char*)(pFunctionName[j]) + hDllFile);
					break;
				}
			}
		}
	}
	FreeLibrary((HMODULE)hDllFile);
	return pFunname;
}

DWORD WxyGetProcAddress(LPCSTR lpDllName, LPCSTR lpProcName, HANDLE hProcess, DWORD& FOAFunction, vector<MODULE_INFO>& ModuleVec)
{
	DWORD hModule = NULL;
	HMODULE hDllFile = NULL;
	BOOL IsLoaded = TRUE;
	for (int i = 0; i < ModuleVec.size(); i++)
	{
		USES_CONVERSION;

		CHAR FileFullPath[MAX_PATH]{};
		strcpy(FileFullPath, T2A(ModuleVec[i].FileFullPath));
		mytoupperA(FileFullPath);
		mytoupperA((char*)lpDllName);
		if (strstr(FileFullPath, (char*)lpDllName))
		{
			if (strstr(FileFullPath, "WINSXS") || strstr(FileFullPath, "HUORONG") || strstr(FileFullPath, "ANTI") || strstr(FileFullPath, "SAFE") || strstr(FileFullPath, "PROTECT"))
			{
				return 0;
			}

			hModule = ModuleVec[i].ImageBase;
			hDllFile = GetModuleHandle(ModuleVec[i].FileFullPath);
			break;
		}
	}
	if (!hDllFile)
	{
		IsLoaded = FALSE;
		hDllFile = LoadLibraryA(lpDllName);
	}
	if (!hDllFile)
	{
		return 0;
	}
	if (!hModule)
	{
		hModule = (DWORD)hDllFile;
	}
	//if (FOAFunction == (DWORD)GetProcAddress(hDllFile, lpProcName))
	//{
		//FOAFunction = 0;
		//return 0;
	//}
	//FOAFunction = (DWORD)GetProcAddress(hDllFile, lpProcName);
	// FreeLibrary(hDllFile);
	//return (DWORD)hModule + FOAFunction - (DWORD)hDllFile;

	PIMAGE_DOS_HEADER pDestanceDos = (PIMAGE_DOS_HEADER)hDllFile;
	PIMAGE_NT_HEADERS pDestanceNt = (PIMAGE_NT_HEADERS)((DWORD)hDllFile + pDestanceDos->e_lfanew);
	PIMAGE_DATA_DIRECTORY pExportDir = &(pDestanceNt->OptionalHeader.DataDirectory[0]);
	PIMAGE_EXPORT_DIRECTORY pExport = (PIMAGE_EXPORT_DIRECTORY)(pExportDir->VirtualAddress + (DWORD)hDllFile);
	PINT pFunction = (PINT)(pExport->AddressOfFunctions + (DWORD)hDllFile);
	PINT pFunctionName = (PINT)(pExport->AddressOfNames + (DWORD)hDllFile);
	PWORD pFunctionOrdin = (PWORD)(pExport->AddressOfNameOrdinals + (DWORD)hDllFile);
	WORD Order = 0;
	if (((DWORD)lpProcName & 0xFFFF0000) == 0)
	{
		Order = (DWORD)lpProcName - pExport->Base;
		FOAFunction = (DWORD)pFunction[Order] + (DWORD)hDllFile;
		return (DWORD)hModule + pFunction[Order];
	}
	else
	{
		for (int i = 0; i < pExport->NumberOfNames; i++)
		{
			char* pFunname2 = (char*)(pFunctionName[i]) + (DWORD)hDllFile;
			if (!strcmp(pFunname2, lpProcName))
			{
				Order = i;
				break;
			}
		}
	}
	if (FOAFunction == (DWORD)pFunction[pFunctionOrdin[Order]] + (DWORD)hDllFile)
	{
		FOAFunction = 0;
		if (IsLoaded == FALSE)
			FreeLibrary(hDllFile);
		return 0;
	}
	else
	{
		FOAFunction = (DWORD)pFunction[pFunctionOrdin[Order]] + (DWORD)hDllFile;
		DWORD ret = (DWORD)hModule + pFunction[pFunctionOrdin[Order]];
		if (IsLoaded == FALSE)
			FreeLibrary(hDllFile);
		return ret;
	}
}

DWORD WxyGetProcAddress(DWORD hModule, LPCSTR lpProcName)
{
	PIMAGE_DOS_HEADER pDestanceDos = (PIMAGE_DOS_HEADER)hModule;
	PIMAGE_NT_HEADERS pDestanceNt = (PIMAGE_NT_HEADERS)(hModule + pDestanceDos->e_lfanew);
	PIMAGE_DATA_DIRECTORY pExportDir = &(pDestanceNt->OptionalHeader.DataDirectory[0]);
	PIMAGE_EXPORT_DIRECTORY pExport = (PIMAGE_EXPORT_DIRECTORY)(pExportDir->VirtualAddress + hModule);
	PINT pFunction = (PINT)(pExport->AddressOfFunctions + hModule);
	PINT pFunctionName = (PINT)(pExport->AddressOfNames + hModule);
	PWORD pFunctionOrdin = (PWORD)(pExport->AddressOfNameOrdinals + hModule);
	WORD Order = -1;
	if (((DWORD)lpProcName & 0xFFFF0000) == 0)
	{
		Order = (DWORD)lpProcName - pExport->Base;
		return pFunction[Order];
	}
	else
	{
		for (int i = 0; i < pExport->NumberOfNames; i++)
		{
			char* pFunname2 = (char*)(pFunctionName[i]) + (DWORD)hModule;
			if (!strcmp(pFunname2, lpProcName))
			{
				Order = i;
				break;
			}
		}
	}
	if (Order == (WORD)-1)
		return 0;
	return pFunction[pFunctionOrdin[Order]];
<<<<<<< HEAD
}

=======
}
>>>>>>> v1.0
