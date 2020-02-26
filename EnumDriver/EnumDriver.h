#pragma once

#include <ntifs.h>

typedef struct _DEVICE_EXTENSION {
	PDEVICE_OBJECT pDevice;
	UNICODE_STRING ustrDeviceName;
	UNICODE_STRING ustrSymLinkName;
} DEVICE_EXTENSION, * PDEVICE_EXTENSION;

typedef struct _LDR_DATA_TABLE_ENTRY {
	LIST_ENTRY InLoadOrderLinks;
	LIST_ENTRY InMemoryOrderLinks;
	LIST_ENTRY InInitializationOrderLinks;
	PVOID DllBase;
	PVOID EntryPoint;
	ULONG SizeOfImage;
	UNICODE_STRING FullDllName;
	UNICODE_STRING BaseDllName;
	ULONG Flags;
	USHORT LoadCount;
	USHORT TlsIndex;
	union {
		LIST_ENTRY HashLinks;
		struct {
			PVOID SectionPointer;
			ULONG CheckSum;
		};
	};
	union {
		struct {
			ULONG TimeDateStamp;
		};
		struct {
			PVOID LoadedImports;
		};
	};
	struct _ACTIVATION_CONTEXT* EntryPointActivationContext;

	PVOID PatchInformation;

} LDR_DATA_TABLE_ENTRY, * PLDR_DATA_TABLE_ENTRY;


typedef struct _DRIVER_INFO_
{
	WCHAR BaseDllName[100];
	PVOID DllBase;
	PVOID EntryPoint;
	ULONG SizeOfImage;
	WCHAR FullDllName[260];
}DRIVER_INFO, * PDRIVER_INFO;

#define DRIVER_COUNT_CODE CTL_CODE(\
			FILE_DEVICE_UNKNOWN, \
			0x800, \
			METHOD_BUFFERED, \
			FILE_ANY_ACCESS)

#define DRIVER_INFO_CODE CTL_CODE(\
			FILE_DEVICE_UNKNOWN, \
			0x801, \
			METHOD_BUFFERED, \
			FILE_ANY_ACCESS)


#define arraysize(p) (sizeof(p)/sizeof((p)[0]))

NTSTATUS CreateDevice(IN PDRIVER_OBJECT pDriverObject);
NTSTATUS DriverEntry(PDRIVER_OBJECT pDriverObject, PUNICODE_STRING pRegistryPath);
VOID WxyUnloadRoutine(IN PDRIVER_OBJECT pDriverObject);
NTSTATUS WxyDispatchRoutine(IN PDEVICE_OBJECT pDevObj, IN PIRP pIrp);
NTSTATUS WxyDeviceIOControlRoutine(IN PDEVICE_OBJECT pDevObj, IN PIRP pIrp);