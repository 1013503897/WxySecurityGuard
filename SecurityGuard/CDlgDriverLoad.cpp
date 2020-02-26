#include "pch.h"
#include "CDlgDriverLoad.h"
#include "afxdialogex.h"

IMPLEMENT_DYNAMIC(CDlgDriverLoad, CDialogEx)

CDlgDriverLoad::CDlgDriverLoad(CWnd* pParent)
	: CDialogEx(IDD_DIALOG_DRIVERLOAD, pParent)
{

}

CDlgDriverLoad::~CDlgDriverLoad()
{
}

void CDlgDriverLoad::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_cListBoxMsg);
}


BEGIN_MESSAGE_MAP(CDlgDriverLoad, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON7, &CDlgDriverLoad::OnBnClickedButtonInstall)
	ON_BN_CLICKED(IDC_BUTTON1, &CDlgDriverLoad::OnBnClickedButtonStart)
	ON_BN_CLICKED(IDC_BUTTON9, &CDlgDriverLoad::OnBnClickedButtonStop)
	ON_BN_CLICKED(IDC_BUTTON8, &CDlgDriverLoad::OnBnClickedButtonUnistall)
	ON_WM_DROPFILES()
END_MESSAGE_MAP()

void CDlgDriverLoad::OnBnClickedButtonInstall()
{
	if (wcslen(m_strFilePath) == 0)
	{
		Msg(_T("请选择文件"));
		return;
	}
	sh = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);
	if (!sh)
	{
		Msg(_T("打开服务控制器失败,请检查是否以管理员权限运行"));
		CloseServiceHandle(sh);
		return;
	}
	Msg(_T("Loading----"));
	Msg(m_strDriverName);
	SC_HANDLE m_hServiceDDK = CreateService(
		sh,//SMC句柄
		m_strDriverName,//驱动服务名称(驱动程序的在注册表中的名字)
		m_strDriverName,//驱动服务显示名称(注册表驱动程序的DisplayName值)
		SERVICE_ALL_ACCESS,//权限(所有访问权限)
		SERVICE_KERNEL_DRIVER,//服务类型(驱动程序)
		SERVICE_DEMAND_START,//启动方式(需要时启动,注册表驱动程序的Start值)
		SERVICE_ERROR_IGNORE,//错误控制(忽略,注册表驱动程序的ErrorControl值)
		m_strFilePath,//服务的二进制文件路径(驱动程序文件路径, 注册表驱动程序的ImagePath值)
		NULL,//加载组命令
		NULL,//TagId
		NULL,//依存关系
		NULL,//服务启动名
		NULL);//密码
	if (!m_hServiceDDK)
	{
		if (GetLastError() == ERROR_SERVICE_EXISTS)
		{
			Msg(_T("驱动已经存在"));
			if (!m_hServiceDDK)m_hServiceDDK = OpenService(sh, m_strDriverName, SERVICE_ALL_ACCESS);
		}
		else {
			TCHAR msg[100];
			wprintf_s(msg, "安装失败，错误码 %p", GetLastError());
			Msg(msg);

			Msg(_T("Error while Install ,error code:" + GetLastError()));
			MessageBox(NULL, m_strDriverName, MB_OK);
		}

	}
	else {
		Msg(_T("驱动安装成功!"));
	}
	CloseServiceHandle(sh);
	CloseServiceHandle(m_hServiceDDK);
}


void CDlgDriverLoad::OnBnClickedButtonStart()
{
	if (!m_hServiceDDK)
	{
		Msg(_T("请先安装驱动"));
		return;
	}
	sh = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);
	if (!sh)
	{
		Msg(_T("打开服务控制器失败,请检查是否以管理员权限运行"));
		CloseServiceHandle(sh);
		return;
	}
	Msg(_T("启动中"));
	m_hServiceDDK = OpenService(sh, m_strDriverName, SERVICE_START);

	if (!StartService(m_hServiceDDK, NULL, NULL))
	{
		DWORD ErrorCode = GetLastError();
		switch (ErrorCode)
		{
		case ERROR_SERVICE_ALREADY_RUNNING:
			Msg(_T("驱动已经运行"));
			break;
		case ERROR_SERVICE_NOT_FOUND:
			Msg(_T("驱动未找到"));
			break;
		default:
			CString msg("启动失败，错误码 %d " + ErrorCode);
			Msg(msg);
			break;
		}
		Msg(_T("启动失败"));
	}
	else {
		Msg(_T("启动成功"));
	}
	CloseServiceHandle(sh);
	CloseServiceHandle(m_hServiceDDK);
}


void CDlgDriverLoad::OnBnClickedButtonStop()
{
	sh = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);
	m_hServiceDDK = OpenService(sh, m_strDriverName, SERVICE_STOP);
	SERVICE_STATUS svcsta = { 0 };
	BOOL bRet = ControlService(m_hServiceDDK, SERVICE_CONTROL_STOP, &svcsta);
	if (!bRet)
	{
		DWORD ErrorCode = GetLastError();
		switch (ErrorCode)
		{
		case ERROR_SERVICE_NEVER_STARTED:
		case ERROR_SERVICE_NOT_ACTIVE:
			Msg(_T("驱动未启动"));
			break;
		case ERROR_SERVICE_NOT_FOUND:
			Msg(_T("驱动不存在"));
			break;
		default:
			CString msg;
			wprintf_s(msg, "停止失败，错误码 %d", ErrorCode);
			Msg(msg);
			break;
		}
	}
	else {
		Msg(_T("驱动已停止"));
	}
	CloseServiceHandle(sh);
	CloseServiceHandle(m_hServiceDDK);
}


void CDlgDriverLoad::OnBnClickedButtonUnistall()
{
	sh = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);
	m_hServiceDDK = OpenService(sh, m_strDriverName, SERVICE_STOP | DELETE);
	if (!DeleteService(m_hServiceDDK))
	{
		DWORD ErrorCode = GetLastError();
		switch (ErrorCode)
		{
		case ERROR_SERVICE_DOES_NOT_EXIST:
		case ERROR_SERVICE_NOT_FOUND:
			Msg(_T("驱动不存在"));
			break;
		default:
			CString msg;
			wprintf_s(msg, "停止失败，错误码 %d", ErrorCode);
			Msg(msg);
			break;
		}
	}
	else {
		Msg(_T("卸载驱动成功"));
	}
	CloseServiceHandle(sh);
	CloseServiceHandle(m_hServiceDDK);
}


void CDlgDriverLoad::OnDropFiles(HDROP hDropInfo)
{
	DragQueryFile(hDropInfo, 0, m_strFilePath, sizeof(m_strFilePath)); 
	SetDlgItemText(IDC_STATIC, m_strFilePath);
	_tcscpy(m_strDriverName, _tcsrchr(m_strFilePath, _T('\\')) + 1);
	Msg(m_strFilePath);
	DragFinish(hDropInfo);                                   
	CDialogEx::OnDropFiles(hDropInfo);
}

void CDlgDriverLoad::Msg(TCHAR* p) {
	m_cListBoxMsg.AddString(CString(p));
}
void CDlgDriverLoad::Msg(CString msg) {
	m_cListBoxMsg.AddString(msg);
}