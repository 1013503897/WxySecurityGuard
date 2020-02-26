#include "pch.h"

DWORD ProcessID;
BYTE g_orginalFunc[5];
BYTE g_newFunc[5] = { 0xE9 };
void InstallHOOK();
void UninstallHOOK();

HANDLE WINAPI FakeOpenProcess(DWORD dwDesiredAccess, BOOL bInheritHandle, DWORD dwProcessId);

BOOL APIENTRY DllMain(HMODULE hModule,
	DWORD  ul_reason_for_call,
	LPVOID lpReserved
)
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
	{
		MessageBox(NULL, L"任务管理器", L"注入", NULL);
		InstallHOOK();
	}
	break;
	case DLL_PROCESS_DETACH:
		UninstallHOOK();
		break;
	}
	return TRUE;
}


HANDLE WINAPI FakeOpenProcess(DWORD dwDesiredAccess,
	BOOL bInheritHandle, DWORD dwProcessId)
{
	if (dwProcessId == ProcessID && dwDesiredAccess & PROCESS_TERMINATE)
	{
		::MessageBox(NULL, L"无权关闭wxy的安全卫士", L"想屁吃", NULL);
		return NULL;
	}
	else
	{
		UninstallHOOK();
		HANDLE hProc = OpenProcess(dwDesiredAccess, bInheritHandle, dwProcessId);
		InstallHOOK();
		return hProc;
	}
}

void InstallHOOK()
{
	HANDLE hMap = OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, L"Pid");
	if (hMap != NULL)
	{
		LPVOID hAddr = MapViewOfFile(hMap, FILE_MAP_ALL_ACCESS, 0, 0, 0x10);
		ProcessID = *(DWORD*)hAddr;

		memcpy(g_orginalFunc, OpenProcess, 5);
		DWORD Offset = (DWORD)FakeOpenProcess - (DWORD)OpenProcess - 5;
		*(DWORD*)(g_newFunc + 1) = Offset;
		DWORD dwOldAttrubet = 0;
		VirtualProtect(OpenProcess, 5, PAGE_EXECUTE_READWRITE, &dwOldAttrubet);
		memcpy(OpenProcess, g_newFunc, 5);
		VirtualProtect(OpenProcess, 5, dwOldAttrubet, &dwOldAttrubet);
	}
}

void UninstallHOOK()
{
	DWORD dwoldAttrubet;
	VirtualProtect(OpenProcess, 5, PAGE_EXECUTE_READWRITE, &dwoldAttrubet);
	memcpy(OpenProcess, g_orginalFunc, 5);
	VirtualProtect(OpenProcess, 5, dwoldAttrubet, &dwoldAttrubet);
}