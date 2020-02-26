#pragma once
#include "EnumDriver.h"

NTSTATUS DriverEntry(PDRIVER_OBJECT pDriverObject, PUNICODE_STRING pRegistryPath)
{
	NTSTATUS status;
	KdPrint(("Enter Entry\n"));

	pDriverObject->DriverUnload = WxyUnloadRoutine;

	pDriverObject->MajorFunction[IRP_MJ_CREATE] = WxyDispatchRoutine;
	pDriverObject->MajorFunction[IRP_MJ_CLOSE] = WxyDispatchRoutine;
	pDriverObject->MajorFunction[IRP_MJ_WRITE] = WxyDispatchRoutine;
	pDriverObject->MajorFunction[IRP_MJ_READ] = WxyDispatchRoutine;
	pDriverObject->MajorFunction[IRP_MJ_CLEANUP] = WxyDispatchRoutine;
	pDriverObject->MajorFunction[IRP_MJ_DEVICE_CONTROL] = WxyDispatchRoutine;
	pDriverObject->MajorFunction[IRP_MJ_SET_INFORMATION] = WxyDispatchRoutine;
	pDriverObject->MajorFunction[IRP_MJ_SHUTDOWN] = WxyDispatchRoutine;
	pDriverObject->MajorFunction[IRP_MJ_SYSTEM_CONTROL] = WxyDispatchRoutine; 
	pDriverObject->MajorFunction[IRP_MJ_DEVICE_CONTROL] = WxyDeviceIOControlRoutine;

	status = CreateDevice(pDriverObject);

	KdPrint(("Leave DriverEntry\n"));
	return status;
}

VOID WxyUnloadRoutine(IN PDRIVER_OBJECT pDriverObject)
{
	PDEVICE_OBJECT	pNextObj;
	KdPrint(("Enter WxyUnloadRoutine\n"));
	pNextObj = pDriverObject->DeviceObject;
	while (pNextObj != NULL)
	{
		PDEVICE_EXTENSION pDevExt = (PDEVICE_EXTENSION)
			pNextObj->DeviceExtension;
		UNICODE_STRING pLinkName = pDevExt->ustrSymLinkName;
		IoDeleteSymbolicLink(&pLinkName);
		pNextObj = pNextObj->NextDevice;
		IoDeleteDevice(pDevExt->pDevice);
	}
}

NTSTATUS WxyDispatchRoutine(IN PDEVICE_OBJECT pDevObj, IN PIRP pIrp)
{
	KdPrint(("Enter WxyDispatchRoutine\n"));

	PIO_STACK_LOCATION stack = IoGetCurrentIrpStackLocation(pIrp);
	static char* irpname[] =
	{
		"IRP_MJ_CREATE",
		"IRP_MJ_CREATE_NAMED_PIPE",
		"IRP_MJ_CLOSE",
		"IRP_MJ_READ",
		"IRP_MJ_WRITE",
		"IRP_MJ_QUERY_INFORMATION",
		"IRP_MJ_SET_INFORMATION",
		"IRP_MJ_QUERY_EA",
		"IRP_MJ_SET_EA",
		"IRP_MJ_FLUSH_BUFFERS",
		"IRP_MJ_QUERY_VOLUME_INFORMATION",
		"IRP_MJ_SET_VOLUME_INFORMATION",
		"IRP_MJ_DIRECTORY_CONTROL",
		"IRP_MJ_FILE_SYSTEM_CONTROL",
		"IRP_MJ_DEVICE_CONTROL",
		"IRP_MJ_INTERNAL_DEVICE_CONTROL",
		"IRP_MJ_SHUTDOWN",
		"IRP_MJ_LOCK_CONTROL",
		"IRP_MJ_CLEANUP",
		"IRP_MJ_CREATE_MAILSLOT",
		"IRP_MJ_QUERY_SECURITY",
		"IRP_MJ_SET_SECURITY",
		"IRP_MJ_POWER",
		"IRP_MJ_SYSTEM_CONTROL",
		"IRP_MJ_DEVICE_CHANGE",
		"IRP_MJ_QUERY_QUOTA",
		"IRP_MJ_SET_QUOTA",
		"IRP_MJ_PNP",
	};

	UCHAR type = stack->MajorFunction;
	if (type >= arraysize(irpname))
		KdPrint((" - Unknown IRP, major type %X\n", type));
	else
		KdPrint(("\t%s\n", irpname[type]));

	NTSTATUS status = STATUS_SUCCESS;
	pIrp->IoStatus.Status = status;
	pIrp->IoStatus.Information = 0;
	IoCompleteRequest(pIrp, IO_NO_INCREMENT);
	KdPrint(("Leave WxyDispatchRoutine\n"));

	return status;
}

NTSTATUS CreateDevice(IN PDRIVER_OBJECT pDriverObject)
{
	NTSTATUS status;
	UNICODE_STRING DevName;
	UNICODE_STRING SymbolName;
	PDEVICE_OBJECT pDeviceObject;
	PDEVICE_EXTENSION pDeviceExtension;
	RtlInitUnicodeString(&DevName, L"\\Device\\MyDDKDevice");
	RtlInitUnicodeString(&SymbolName, L"\\??\\HelloDDK");
	status = IoCreateDevice(pDriverObject,
		sizeof(DEVICE_EXTENSION),
		&DevName,
		FILE_DEVICE_UNKNOWN,
		0, TRUE,
		&pDeviceObject);
	if (!NT_SUCCESS(status))
	{
		return status;
	}
	pDeviceObject->Flags |= DO_BUFFERED_IO;
	pDeviceExtension = pDeviceObject->DeviceExtension;
	pDeviceExtension->pDevice = pDeviceObject;
	pDeviceExtension->ustrDeviceName = DevName;
	pDeviceExtension->ustrSymLinkName = SymbolName;
	status = IoCreateSymbolicLink(&SymbolName, &DevName);
	if (!NT_SUCCESS(status))
	{
		IoDeleteDevice(pDeviceObject);
		return status;
	}
	return STATUS_SUCCESS;
}

NTSTATUS WxyDeviceIOControlRoutine(IN PDEVICE_OBJECT pDevObj, IN PIRP pIrp)
{
	NTSTATUS status = STATUS_SUCCESS;
	KdPrint(("Enter WxyDeviceIOControlRoutine\n"));
	PIO_STACK_LOCATION stack = IoGetCurrentIrpStackLocation(pIrp);
	ULONG cbin = stack->Parameters.DeviceIoControl.InputBufferLength;
	ULONG cbout = stack->Parameters.DeviceIoControl.OutputBufferLength;
	ULONG code = stack->Parameters.DeviceIoControl.IoControlCode;
	PLDR_DATA_TABLE_ENTRY entry = (PLDR_DATA_TABLE_ENTRY)pDevObj->DriverObject->DriverSection;
	PLDR_DATA_TABLE_ENTRY firstentry = entry;

	ULONG info = 0;
	ULONG count = 0;
	switch (code)
	{
	case DRIVER_COUNT_CODE:
	{
		KdPrint(("DRIVER_COUNT_CODE\n"));
		do
		{
			if (entry->FullDllName.Buffer)
			{
				count++;
				entry = (PLDR_DATA_TABLE_ENTRY)entry->InLoadOrderLinks.Blink;
			}
		} while (entry->InLoadOrderLinks.Blink != firstentry);

		UCHAR* OutputBuffer = (UCHAR*)pIrp->AssociatedIrp.SystemBuffer;
		KdPrint(("count = %d\n",count));
		memcpy(OutputBuffer, &count, sizeof(int));
		info = sizeof(int);
		break;
	}
	case DRIVER_INFO_CODE:
	{
		KdPrint(("DRIVER_INFO_CODE\n"));
		ULONG cbin = stack->Parameters.DeviceIoControl.OutputBufferLength;
		ULONG max_count = cbin / sizeof(DRIVER_INFO);
		PDRIVER_INFO pDriverInfo = (PDRIVER_INFO)pIrp->AssociatedIrp.SystemBuffer;
		do
		{
			if (entry->FullDllName.Buffer)
			{
				count++;
				wcscpy(pDriverInfo->BaseDllName, entry->BaseDllName.Buffer);
				wcscpy(pDriverInfo->FullDllName, entry->FullDllName.Buffer);
				pDriverInfo->DllBase = entry->DllBase;
				pDriverInfo->EntryPoint = entry->EntryPoint;
				pDriverInfo->SizeOfImage = entry->SizeOfImage;
				KdPrint(("%wZ\t		%0x\t   %0x\t  %0x\t  %wZ\n", &entry->BaseDllName, entry->DllBase, entry->EntryPoint, entry->SizeOfImage, &entry->FullDllName));
				pDriverInfo++;
				entry = (PLDR_DATA_TABLE_ENTRY)entry->InLoadOrderLinks.Blink;
			}
		} while (entry->InLoadOrderLinks.Blink != firstentry && count<=max_count);
		info = count*sizeof(DRIVER_INFO);
		break;
	}

	default:
		status = STATUS_INVALID_VARIANT;
	}
	pIrp->IoStatus.Status = status;
	pIrp->IoStatus.Information = info;
	IoCompleteRequest(pIrp, IO_NO_INCREMENT);

	KdPrint(("Leave WxyDeviceIOControlRoutine\n"));

	return status;
}
