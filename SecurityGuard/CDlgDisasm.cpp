#include "pch.h"
#include "CDlgDisasm.h"
#include "afxdialogex.h"
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



IMPLEMENT_DYNAMIC(CDlgDisasm, CDialogEx)

CDlgDisasm::CDlgDisasm(CWnd* pParent )
	: CDialogEx(IDD_DIALOG_DISASM2, pParent)
{

}

CDlgDisasm::~CDlgDisasm()
{
}

void CDlgDisasm::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_cListCode);
	DDX_Control(pDX, IDC_EDIT1, m_cEditHexData);
}


BEGIN_MESSAGE_MAP(CDlgDisasm, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &CDlgDisasm::OnBnClickedButtonDisasm)
END_MESSAGE_MAP()



void CDlgDisasm::OnBnClickedButtonDisasm()
{
	m_cListCode.DeleteAllItems();
	DISASM disAsm = { 0 };
	CString strHexData;
	m_cEditHexData.GetWindowText(strHexData);
	USES_CONVERSION;
	disAsm.EIP = (UIntPtr)CharToHex(T2A(strHexData));
	disAsm.Archi = 0;
	disAsm.Options = 0x000;
	int index = 0;
	int nOpcodeSize = 1 + _tcslen(strHexData) / 3;
	int nCount = 0;
	int nLen = 0;
	while (nCount < nOpcodeSize)
	{
		nLen = Disasm(&disAsm);
		TCHAR Instruct[0x100] = { 0 };
		MultiByteToWideChar(CP_ACP, 0, disAsm.CompleteInstr, -1, Instruct, 100);
		CString OPCODE;
		for (int i = 0; i < nLen; ++i)
		{
			CString tmp;
			tmp.Format(_T("%02X "), ((const unsigned char*)disAsm.EIP)[i]);
			OPCODE += tmp;
		}
		m_cListCode.InsertItem(index, OPCODE);
		m_cListCode.SetItemText(index++, 1, Instruct);

		nCount += nLen;
		disAsm.EIP += nLen;
		disAsm.VirtualAddr += nLen;
	}
}


BOOL CDlgDisasm::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	m_cListCode.InsertColumn(0, _T("机器码"), LVCFMT_LEFT, 250);
	m_cListCode.InsertColumn(1, _T("指令"), LVCFMT_LEFT, 340);
	return TRUE;  
}

char* CharToHex(char* AsciiChar)
{
	char* ch = AsciiChar;
	char* ch2 = AsciiChar + 1;
	char* ret = new char[strlen(AsciiChar)]{};
	char* tmp = ret;
	while (*ch!=(char)0xcc)
	{
		if ((*ch >= 0x30) && (*ch <= 0x39))
			*ch -= 0x30;
		else if ((*ch >= 0x41) && (*ch <= 0x46))
			*ch -= 0x37;
		else if ((*ch >= 0x61) && (*ch <= 0x66))
			*ch -= 0x57;
		if ((*ch2 >= 0x30) && (*ch2 <= 0x39))
			*ch2 -= 0x30;
		else if ((*ch2 >= 0x41) && (*ch2 <= 0x46))
			*ch2 -= 0x37;
		else if ((*ch2 >= 0x61) && (*ch2 <= 0x66))
			*ch2 -= 0x57;
		*tmp = 0x10 * *ch + *ch2;
		ch += 3;
		ch2 += 3;
		tmp++;
	}
	return ret;
}
