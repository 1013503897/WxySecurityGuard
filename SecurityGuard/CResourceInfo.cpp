// ResourceInfo.cpp: 实现文件
//

#include "pch.h"
#include "CSecurityGuard.h"
#include "CResourceInfo.h"
#include "afxdialogex.h"


// ResourceInfo 对话框

IMPLEMENT_DYNAMIC(ResourceInfo, CDialogEx)

ResourceInfo::ResourceInfo(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_RESOURCE, pParent)
{

}

ResourceInfo::~ResourceInfo()
{
}

void ResourceInfo::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(ResourceInfo, CDialogEx)
END_MESSAGE_MAP()

