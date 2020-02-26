#pragma once
#include <Psapi.h>
#include <strsafe.h>
#include <afxext.h>
#include <stdio.h>
#include <winsvc.h>
#include <tlhelp32.h> 
#include "openssl/evp.h"
#include "openssl/md5.h"
#include "openssl/md4.h"
#include "openssl/sha.h"
#pragma comment(lib, "openssl/lib/libcrypto.lib")
#pragma comment(lib, "version.lib")
#pragma comment(lib, "advapi32.lib")

#define STATUS_SUCCESS                  ((NTSTATUS)0x00000000L)
#define STATUS_INFO_LENGTH_MISMATCH     ((NTSTATUS)0xC0000004L)
#define NT_SUCCESS(Status)				((NTSTATUS)(Status) >= 0)
#define HIGH_PRIORITY					31

using namespace std;
typedef LONG NTSTATUS;
typedef NTSTATUS* PNTSTATUS;
#ifdef _WIN64
typedef __int64 KPRIORITY;
#else
typedef long KPRIORITY;
#endif
typedef struct _UNICODE_STRING
{
	union {
		USHORT Length;
		USHORT LengthInBytes;
	};
	union {
		USHORT MaximumLength;
		USHORT MaximumLengthInBytes;
	};
	PTSTR Buffer;
} UNICODE_STRING, * PUNICODE_STRING;
typedef enum _SYSTEM_INFORMATION_CLASS
{
	SystemBasicInformation = 0x0,
	SystemProcessorInformation = 0x1,
	SystemPerformanceInformation = 0x2,
	SystemTimeOfDayInformation = 0x3,
	SystemPathInformation = 0x4,
	SystemProcessInformation = 0x5,
	SystemCallCountInformation = 0x6,
	SystemDeviceInformation = 0x7,
	SystemProcessorPerformanceInformation = 0x8,
	SystemFlagsInformation = 0x9,
	SystemCallTimeInformation = 0xa,
	SystemModuleInformation = 0xb,
	SystemLocksInformation = 0xc,
	SystemStackTraceInformation = 0xd,
	SystemPagedPoolInformation = 0xe,
	SystemNonPagedPoolInformation = 0xf,
	SystemHandleInformation = 0x10,
	SystemObjectInformation = 0x11,
	SystemPageFileInformation = 0x12,
	SystemVdmInstemulInformation = 0x13,
	SystemVdmBopInformation = 0x14,
	SystemFileCacheInformation = 0x15,
	SystemPoolTagInformation = 0x16,
	SystemInterruptInformation = 0x17,
	SystemDpcBehaviorInformation = 0x18,
	SystemFullMemoryInformation = 0x19,
	SystemLoadGdiDriverInformation = 0x1a,
	SystemUnloadGdiDriverInformation = 0x1b,
	SystemTimeAdjustmentInformation = 0x1c,
	SystemSummaryMemoryInformation = 0x1d,
	SystemMirrorMemoryInformation = 0x1e,
	SystemPerformanceTraceInformation = 0x1f,
	SystemObsolete0 = 0x20,
	SystemExceptionInformation = 0x21,
	SystemCrashDumpStateInformation = 0x22,
	SystemKernelDebuggerInformation = 0x23,
	SystemContextSwitchInformation = 0x24,
	SystemRegistryQuotaInformation = 0x25,
	SystemExtendServiceTableInformation = 0x26,
	SystemPrioritySeperation = 0x27,
	SystemVerifierAddDriverInformation = 0x28,
	SystemVerifierRemoveDriverInformation = 0x29,
	SystemProcessorIdleInformation = 0x2a,
	SystemLegacyDriverInformation = 0x2b,
	SystemCurrentTimeZoneInformation = 0x2c,
	SystemLookasideInformation = 0x2d,
	SystemTimeSlipNotification = 0x2e,
	SystemSessionCreate = 0x2f,
	SystemSessionDetach = 0x30,
	SystemSessionInformation = 0x31,
	SystemRangeStartInformation = 0x32,
	SystemVerifierInformation = 0x33,
	SystemVerifierThunkExtend = 0x34,
	SystemSessionProcessInformation = 0x35,
	SystemLoadGdiDriverInSystemSpace = 0x36,
	SystemNumaProcessorMap = 0x37,
	SystemPrefetcherInformation = 0x38,
	SystemExtendedProcessInformation = 0x39,
	SystemRecommendedSharedDataAlignment = 0x3a,
	SystemComPlusPackage = 0x3b,
	SystemNumaAvailableMemory = 0x3c,
	SystemProcessorPowerInformation = 0x3d,
	SystemEmulationBasicInformation = 0x3e,
	SystemEmulationProcessorInformation = 0x3f,
	SystemExtendedHandleInformation = 0x40,
	SystemLostDelayedWriteInformation = 0x41,
	SystemBigPoolInformation = 0x42,
	SystemSessionPoolTagInformation = 0x43,
	SystemSessionMappedViewInformation = 0x44,
	SystemHotpatchInformation = 0x45,
	SystemObjectSecurityMode = 0x46,
	SystemWatchdogTimerHandler = 0x47,
	SystemWatchdogTimerInformation = 0x48,
	SystemLogicalProcessorInformation = 0x49,
	SystemWow64SharedInformationObsolete = 0x4a,
	SystemRegisterFirmwareTableInformationHandler = 0x4b,
	SystemFirmwareTableInformation = 0x4c,
	SystemModuleInformationEx = 0x4d,
	SystemVerifierTriageInformation = 0x4e,
	SystemSuperfetchInformation = 0x4f,
	SystemMemoryListInformation = 0x50,
	SystemFileCacheInformationEx = 0x51,
	SystemThreadPriorityClientIdInformation = 0x52,
	SystemProcessorIdleCycleTimeInformation = 0x53,
	SystemVerifierCancellationInformation = 0x54,
	SystemProcessorPowerInformationEx = 0x55,
	SystemRefTraceInformation = 0x56,
	SystemSpecialPoolInformation = 0x57,
	SystemProcessIdInformation = 0x58,
	SystemErrorPortInformation = 0x59,
	SystemBootEnvironmentInformation = 0x5a,
	SystemHypervisorInformation = 0x5b,
	SystemVerifierInformationEx = 0x5c,
	SystemTimeZoneInformation = 0x5d,
	SystemImageFileExecutionOptionsInformation = 0x5e,
	SystemCoverageInformation = 0x5f,
	SystemPrefetchPatchInformation = 0x60,
	SystemVerifierFaultsInformation = 0x61,
	SystemSystemPartitionInformation = 0x62,
	SystemSystemDiskInformation = 0x63,
	SystemProcessorPerformanceDistribution = 0x64,
	SystemNumaProximityNodeInformation = 0x65,
	SystemDynamicTimeZoneInformation = 0x66,
	SystemCodeIntegrityInformation = 0x67,
	SystemProcessorMicrocodeUpdateInformation = 0x68,
	SystemProcessorBrandString = 0x69,
	SystemVirtualAddressInformation = 0x6a,
	SystemLogicalProcessorAndGroupInformation = 0x6b,
	SystemProcessorCycleTimeInformation = 0x6c,
	SystemStoreInformation = 0x6d,
	SystemRegistryAppendString = 0x6e,
	SystemAitSamplingValue = 0x6f,
	SystemVhdBootInformation = 0x70,
	SystemCpuQuotaInformation = 0x71,
	SystemNativeBasicInformation = 0x72,
	SystemErrorPortTimeouts = 0x73,
	SystemLowPriorityIoInformation = 0x74,
	SystemBootEntropyInformation = 0x75,
	SystemVerifierCountersInformation = 0x76,
	SystemPagedPoolInformationEx = 0x77,
	SystemSystemPtesInformationEx = 0x78,
	SystemNodeDistanceInformation = 0x79,
	SystemAcpiAuditInformation = 0x7a,
	SystemBasicPerformanceInformation = 0x7b,
	SystemQueryPerformanceCounterInformation = 0x7c,
	SystemSessionBigPoolInformation = 0x7d,
	SystemBootGraphicsInformation = 0x7e,
	SystemScrubPhysicalMemoryInformation = 0x7f,
	SystemBadPageInformation = 0x80,
	SystemProcessorProfileControlArea = 0x81,
	SystemCombinePhysicalMemoryInformation = 0x82,
	SystemEntropyInterruptTimingInformation = 0x83,
	SystemConsoleInformation = 0x84,
	SystemPlatformBinaryInformation = 0x85,
	SystemThrottleNotificationInformation = 0x86,
	SystemHypervisorProcessorCountInformation = 0x87,
	SystemDeviceDataInformation = 0x88,
	SystemDeviceDataEnumerationInformation = 0x89,
	SystemMemoryTopologyInformation = 0x8a,
	SystemMemoryChannelInformation = 0x8b,
	SystemBootLogoInformation = 0x8c,
	SystemProcessorPerformanceInformationEx = 0x8d,
	SystemSpare0 = 0x8e,
	SystemSecureBootPolicyInformation = 0x8f,
	SystemPageFileInformationEx = 0x90,
	SystemSecureBootInformation = 0x91,
	SystemEntropyInterruptTimingRawInformation = 0x92,
	SystemPortableWorkspaceEfiLauncherInformation = 0x93,
	SystemFullProcessInformation = 0x94,
	SystemKernelDebuggerInformationEx = 0x95,
	SystemBootMetadataInformation = 0x96,
	SystemSoftRebootInformation = 0x97,
	SystemElamCertificateInformation = 0x98,
	SystemOfflineDumpConfigInformation = 0x99,
	SystemProcessorFeaturesInformation = 0x9a,
	SystemRegistryReconciliationInformation = 0x9b,
	MaxSystemInfoClass = 0x9c,
} SYSTEM_INFORMATION_CLASS;
typedef struct _SYSTEM_HANDLE_TABLE_ENTRY_INFO
{
	USHORT UniqueProcessId;
	USHORT CreatorBackTraceIndex;
	UCHAR ObjectTypeIndex;
	UCHAR HandleAttributes;
	USHORT HandleValue;
	PVOID Object;
	ULONG GrantedAccess;
} SYSTEM_HANDLE_TABLE_ENTRY_INFO, * PSYSTEM_HANDLE_TABLE_ENTRY_INFO;
typedef struct _SYSTEM_HANDLE_INFORMATION
{
	ULONG NumberOfHandles;
	SYSTEM_HANDLE_TABLE_ENTRY_INFO Handles[1];
} SYSTEM_HANDLE_INFORMATION, * PSYSTEM_HANDLE_INFORMATION;
<<<<<<< HEAD
=======



>>>>>>> v1.0
typedef enum _OBJECT_INFORMATION_CLASS
{
	ObjectBasicInformation,
	ObjectNameInformation,
	ObjectTypeInformation,
	ObjectTypesInformation,
	ObjectHandleFlagInformation,
	ObjectSessionInformation,
	MaxObjectInfoClass
} OBJECT_INFORMATION_CLASS;
typedef struct _OBJECT_TYPE_INFORMATION {
	UNICODE_STRING TypeName;
	ULONG TotalNumberOfObjects;
	ULONG TotalNumberOfHandles;
	ULONG TotalPagedPoolUsage;
	ULONG TotalNonPagedPoolUsage;
	ULONG TotalNamePoolUsage;
	ULONG TotalHandleTableUsage;
	ULONG HighWaterNumberOfObjects;
	ULONG HighWaterNumberOfHandles;
	ULONG HighWaterPagedPoolUsage;
	ULONG HighWaterNonPagedPoolUsage;
	ULONG HighWaterNamePoolUsage;
	ULONG HighWaterHandleTableUsage;
	ULONG InvalidAttributes;
	GENERIC_MAPPING GenericMapping;
	ULONG ValidAccessMask;
	BOOLEAN SecurityRequired;
	BOOLEAN MaintainHandleCount;
	ULONG PoolType;
	ULONG DefaultPagedPoolCharge;
	ULONG DefaultNonPagedPoolCharge;
} OBJECT_TYPE_INFORMATION, * POBJECT_TYPE_INFORMATION;
typedef enum _THREADINFOCLASS
{
	ThreadBasicInformation,
	ThreadTimes,
	ThreadPriority,
	ThreadBasePriority,
	ThreadAffinityMask,
	ThreadImpersonationToken,
	ThreadDescriptorTableEntry,
	ThreadEnableAlignmentFaultFixup,
	ThreadEventPair_Reusable,
	ThreadQuerySetWin32StartAddress,
	ThreadZeroTlsCell,
	ThreadPerformanceCount,
	ThreadAmILastThread,
	ThreadIdealProcessor,
	ThreadPriorityBoost,
	ThreadSetTlsArrayAddress,
	ThreadIsIoPending,
	ThreadHideFromDebugger,
	ThreadBreakOnTermination,
	ThreadSwitchLegacyState,
	ThreadIsTerminated,
	ThreadLastSystemCall,
	ThreadIoPriority,
	ThreadCycleTime,
	ThreadPagePriority,
	ThreadActualBasePriority,
	ThreadTebInformation,
	ThreadCSwitchMon,
	MaxThreadInfoClass
} THREADINFOCLASS;
typedef struct _CLIENT_ID
{
	HANDLE  UniqueProcess;
	HANDLE  UniqueThread;
} CLIENT_ID, * PCLIENT_ID;
typedef struct _THREAD_BASIC_INFORMATION
{
	NTSTATUS ExitStatus;
	PVOID TebBaseAddress;
	CLIENT_ID ClientId;
	KAFFINITY AffinityMask;
	KPRIORITY Priority;
	KPRIORITY BasePriority;
} THREAD_BASIC_INFORMATION, * PTHREAD_BASIC_INFORMATION;
typedef enum _FILE_INFORMATION_CLASS {
	FileDirectoryInformation = 1,
	FileFullDirectoryInformation,
	FileBothDirectoryInformation,
	FileBasicInformation,
	FileStandardInformation,
	FileInternalInformation,
	FileEaInformation,
	FileAccessInformation,
	FileNameInformation,
	FileRenameInformation,
	FileLinkInformation,
	FileNamesInformation,
	FileDispositionInformation,
	FilePositionInformation,
	FileFullEaInformation,
	FileModeInformation,
	FileAlignmentInformation,
	FileAllInformation,
	FileAllocationInformation,
	FileEndOfFileInformation,
	FileAlternateNameInformation,
	FileStreamInformation,
	FilePipeInformation,
	FilePipeLocalInformation,
	FilePipeRemoteInformation,
	FileMailslotQueryInformation,
	FileMailslotSetInformation,
	FileCompressionInformation,
	FileCopyOnWriteInformation,
	FileCompletionInformation,
	FileMoveClusterInformation,
	FileQuotaInformation,
	FileReparsePointInformation,
	FileNetworkOpenInformation,
	FileObjectIdInformation,
	FileTrackingInformation,
	FileOleDirectoryInformation,
	FileContentIndexInformation,
	FileInheritContentIndexInformation,
	FileOleInformation,
	FileMaximumInformation
} FILE_INFORMATION_CLASS, * PFILE_INFORMATION_CLASS;
typedef struct _IO_STATUS_BLOCK {
	union {
		NTSTATUS Status;
		PVOID    Pointer;
	};
	ULONG_PTR Information;
} IO_STATUS_BLOCK, * PIO_STATUS_BLOCK;
typedef struct _OBJECT_NAME_INFORMATION {
	UNICODE_STRING          Name;
} OBJECT_NAME_INFORMATION, * POBJECT_NAME_INFORMATION;

typedef struct _MSCONFIG_INFO
{
	TCHAR ConfigName[MAX_PATH];
	TCHAR Key[MAX_PATH];
	TCHAR Description[MAX_PATH];
	TCHAR CompanyName[MAX_PATH];
	TCHAR FileFullPath[MAX_PATH];
}MSCONFIG_INFO,*PMSCONFIG_INFO;
typedef NTSTATUS(NTAPI* LPFN_NTQUERYSYSTEMINFORMATION)(
	_In_ SYSTEM_INFORMATION_CLASS SystemInformationClass,
	_Out_ PVOID SystemInformation,
	_In_ ULONG InformationLength,
	_Out_opt_ PULONG ResultLength);
typedef NTSTATUS(NTAPI* LPFN_ZWQUERYOBJECT)(
	_In_opt_ HANDLE Handle,
	_In_ OBJECT_INFORMATION_CLASS ObjectInformationClass,
	_Out_writes_bytes_opt_(ObjectInformationLength) PVOID ObjectInformation,
	_In_ ULONG ObjectInformationLength,
	_Out_opt_ PULONG ReturnLength);
typedef NTSTATUS(WINAPI* LPFN_NTQUERYINFORMATIONTHREAD)(
	IN HANDLE ThreadHandle,
	IN THREADINFOCLASS ThreadInformationClass,
	OUT PVOID ThreadInformation,
	IN ULONG ThreadInformationLength,
	OUT PULONG ReturnLength OPTIONAL);
typedef NTSTATUS(WINAPI* LPFN_ZW_QUERYINFORMATIONFILE)(
	IN HANDLE  FileHandle,
	OUT PIO_STATUS_BLOCK  IoStatusBlock,
	OUT PVOID  FileInformation,
	IN ULONG  Length,
	IN FILE_INFORMATION_CLASS  FileInformationClass
	);

typedef struct _THREAD_INFO_
{
	DWORD ThreadID;								
	ULONG_PTR ThreadStartAddress;			    
	SYSTEMTIME CreateTime;						
	DWORD ThreadPriorty;						
}THREAD_INFO, * PTHREAD_INFO;
typedef struct _PROCESS_INFO_
{
	DWORD ProcessID;
	DWORD ThreadNum;
	TCHAR CompanyName[0x50];
	TCHAR FileDescription[0x50];
	CString FileFullPath;
	TCHAR ProcessName[MAX_PATH];
	HICON ProcessIcon;
	TCHAR ProcessBitInfo[10];
}PROCESS_INFO, * PPROCESS_INFO;
typedef struct _HEAP_INFO_
{
	DWORD HeapID;
	DWORD BlockSize = 0;
}HEAP_INFO, * PHEAP_INFO;
typedef struct _MODULE_INFO_
{
	TCHAR ModuleName[MAX_PATH];
	DWORD ImageBase;
	DWORD ModuleSize;
	TCHAR FileFullPath[MAX_PATH];
	TCHAR CompanyName[0x50] = _T("UNKOWN");
	TCHAR FileDescription[0x50] = _T("UNKOWN");
}MODULE_INFO, * PMODULE_INFO;
typedef struct _HANDLE_INFO_
{
	USHORT HandleValue;
	PVOID  Object;
	UCHAR  ObjectTypeIndex;
	TCHAR  ObjectTypeName[MAX_PATH] = { 0 };
	TCHAR HandleName[MAX_PATH] = { 0 };
}HANDLE_INFO, * PHANDLE_INFO;
typedef struct _DEVICE_INFO_
{
	TCHAR ComputerName[100];
	TCHAR UserName[100];
	TCHAR CPUName[100];
	DWORD CPUClockSpeed;
	DWORD NumberOfProcessors;
	DWORD CpuUsage;
	DWORD MemoryLoad;
	DWORDLONG  MemoryUsage;
}DEVICE_INFO, * PDEVICE_INFO;
typedef struct _FILE_INFO_
{
	TCHAR FileName[MAX_PATH];
	DWORD m_FileSize;
	TCHAR MD5[33];
	TCHAR FileLastChangeTime[30];
	HICON FileIcon;
}FILE_INFO, * PFILE_INFO;
typedef struct _WND_INFO_
{
	TCHAR WndName[MAX_PATH];
	TCHAR ClassName[MAX_PATH];
	DWORD ProcessID;
	BOOL IsVisable;
}WND_INFO, * PWND_INFO;
typedef struct _SERVER_INFO_
{
	TCHAR ServerName[MAX_PATH];
	TCHAR DisPlayName[MAX_PATH];
	BOOL IsRunning;
	DWORD StartType;
	TCHAR BinaryPsth[MAX_PATH];
	TCHAR Description[0x500];
}SERVER_INFO, * PSERVER_INFO;
typedef struct _SOFT_INFO_
{
	TCHAR szSoftName[50];
	TCHAR szSoftVer[50];
	TCHAR szSoftData[20];
	TCHAR szSoftSize[MAX_PATH];
	TCHAR strSoftInsPath[MAX_PATH];
	TCHAR strSoftUniPath[MAX_PATH];
	TCHAR strSoftVenRe[50];
	TCHAR strSoftIco[MAX_PATH];
}SOFT_INFO, * PSOFT_INFO;
typedef struct _HOOK_INFO_
{
	TCHAR ProcessName[50];
	DWORD ProcessID;
	TCHAR HookType[10];
	DWORD SrcAddress;
	TCHAR SrcPlace[MAX_PATH];
	DWORD DstAddress;
	TCHAR DstPlace[MAX_PATH];
	TCHAR DstDll[MAX_PATH];
}HOOK_INFO, * PHOOK_INFO;

void WxyEnumProcess(vector<PPROCESS_INFO>& ProcessInfoVec);
void WxyEnumModule(DWORD ProcessID, vector<PMODULE_INFO>& ModuleInfoVec);
void WxyEnumThread(DWORD ProcessID, vector<PTHREAD_INFO>& ThreadInfoVec);
void WxyEnumHeap(DWORD ProcessID, vector<PHEAP_INFO>& HeapInfoVec);
KPRIORITY WxyGetThreadPriority(HANDLE ThreadHandle);
void WxyGetProcessFilePath(DWORD ProcessId, CString& FileFullPath);
void WxyGetThreadInfoByThreadId(DWORD ThreadID, PTHREAD_INFO ThreadInfoVec);
VOID WxyEnableSeDebugPrivilege();
void WxyEnumHandle(DWORD ProcessID, vector<PHANDLE_INFO>& HandleInfoVec);
VOID WxyGetDeviceInfo(PDEVICE_INFO DeviceInfo);
VOID WxyGetFileInfo(TCHAR* FileFullPath, vector<PFILE_INFO>& FileInfoVec);
BOOL CALLBACK WxyEnumWndProc(HWND hwnd, LPARAM lParam);
DWORD WxyGetCPUUsage(_FILETIME& idleTime, _FILETIME& kernelTime, _FILETIME& userTime);
void WxyEnumServers(vector<PSERVER_INFO>& ServerInfoVec);
void WxyEnumMsConfig(vector<PMSCONFIG_INFO>& ConfigInfoVec);
void WxyEnumUninstall(vector<SOFT_INFO>& UninstallInfoVec);
void WxyEnumMemory(DWORD ProcessID, PPROCESS_MEMORY_COUNTERS MemoryInfoVec);
CString Int2CString(int data);
CString Int2CString(int data, int length);

void md4_encrypt(const void* data, size_t len, CString& ret);
void md5_encrypt(const void* data, size_t len, CString& ret);
void sha1_encrypt(const void* data, size_t len, CString& ret);

void ProcessProtect(PVOID param);

void InjectPorcess(DWORD InjectProcessID, DWORD ProtectProcessID);

int evp_de_cipher(unsigned char* source_string, unsigned char* des_string, int length);
int evp_en_cipher(unsigned char* source_string, unsigned char* des_string, int length);

DWORD RvaToOffset(PBYTE pPeFile, DWORD Rva);
void mytoupperA(char* str);
void mytoupper(TCHAR* str);
bool isforwardstring(char* str);

void WxyCheckIAT(DWORD dwProcessID, vector<PHOOK_INFO> &HookInfoVec);
void WxyCheckEAT(DWORD dwProcessID, vector<PHOOK_INFO> &HookInfoVec);
void WxyCheckInline(DWORD dwProcessID, vector<PHOOK_INFO>& HookInfoVec);
HMODULE WxyGetRemoteImageBase(DWORD dwProcessID);
VOID WxyGetRemoteModuleInfo(DWORD dwProcessID, vector<MODULE_INFO>& ModuleVec);
char* WxyGetProcName(TCHAR* FileFullPath, DWORD FuncAddr);
DWORD WxyGetProcAddress(LPCSTR lpDllName, LPCSTR lpProcName, HANDLE hProcess, DWORD& FOAFunction, vector<MODULE_INFO>& ModuleVec);
DWORD WxyGetProcAddress(DWORD hModule, LPCSTR lpProcName);