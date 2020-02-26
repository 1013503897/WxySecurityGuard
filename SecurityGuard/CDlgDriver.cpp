#include "pch.h"
#include <winioctl.h>
#include "CDlgDriver.h"
#include "afxdialogex.h"

IMPLEMENT_DYNAMIC(CDlgDriver, CDialogEx)
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



CDlgDriver::CDlgDriver(CWnd* pParent)
	: CDialogEx(IDD_DIALOG_DRIVER, pParent)
{

}

CDlgDriver::~CDlgDriver()
{
}

void CDlgDriver::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_cListDriverInfo);
}


BEGIN_MESSAGE_MAP(CDlgDriver, CDialogEx)
	ON_WM_SHOWWINDOW()
	ON_WM_SIZE()
END_MESSAGE_MAP()



BOOL CDlgDriver::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	m_cListDriverInfo.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_SUBITEMIMAGES);
	m_cListDriverInfo.InsertColumn(0, _T("驱动模块"), LVCFMT_LEFT, 200);
	m_cListDriverInfo.InsertColumn(1, _T("模块大小"), LVCFMT_LEFT, 100);
	m_cListDriverInfo.InsertColumn(2, _T("基地址"), LVCFMT_LEFT, 100);
	m_cListDriverInfo.InsertColumn(3, _T("OEP"), LVCFMT_LEFT, 100);
	m_cListDriverInfo.InsertColumn(4, _T("绝对路径"), LVCFMT_LEFT, 400);
	return TRUE;  
}


void CDlgDriver::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialogEx::OnShowWindow(bShow, nStatus);
	if (!IsWindowVisible())
	{
		HANDLE hDevice = CreateFile(_T("\\\\.\\HelloDDK"),
			GENERIC_READ | GENERIC_WRITE,
			0,
			NULL,
			OPEN_EXISTING,
			FILE_ATTRIBUTE_NORMAL,
			NULL);

		if (hDevice == INVALID_HANDLE_VALUE)
		{
			MessageBox(_T("驱动读取失败"));
			return;
		}
		MessageBox(_T("驱动读取成功"));
		int DriverCount = 0;
		DWORD dwOutput;

		if (DeviceIoControl(hDevice, DRIVER_COUNT_CODE, NULL, NULL, &DriverCount, 4, &dwOutput, NULL) && dwOutput == sizeof(DriverCount))
		{
			PDRIVER_INFO pDriverInfo = new DRIVER_INFO[DriverCount];
			if (DeviceIoControl(hDevice, DRIVER_INFO_CODE, NULL, NULL, pDriverInfo, DriverCount * sizeof(DRIVER_INFO), &dwOutput, NULL))
			{
				for (int i = 0; i < DriverCount; i++)
				{
					m_cListDriverInfo.InsertItem(i, _T(""));
					m_cListDriverInfo.SetItemText(i, 0, pDriverInfo[i].BaseDllName);
					CString SizeOfImage;
					SizeOfImage.Format(_T("%d"), pDriverInfo[i].SizeOfImage);
					m_cListDriverInfo.SetItemText(i, 1, SizeOfImage);
					CString DllBase;
					DllBase.Format(_T("0x%08X"), pDriverInfo[i].DllBase);
					m_cListDriverInfo.SetItemText(i, 2, DllBase);
					CString EntryPoint;
					EntryPoint.Format(_T("0x%08X"), pDriverInfo[i].EntryPoint);
					m_cListDriverInfo.SetItemText(i, 3, EntryPoint);
					m_cListDriverInfo.SetItemText(i, 4, pDriverInfo[i].FullDllName);
				}
			}

		}
		CloseHandle(hDevice);
	}
}


void CDlgDriver::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	CRect rc;
	this->GetClientRect(rc);
	if (m_cListDriverInfo.m_hWnd)
	{
		m_cListDriverInfo.MoveWindow(rc);
	}
}
