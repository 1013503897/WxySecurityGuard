// MyHook.cpp : 定义 DLL 应用程序的导出函数。
//

#include "stdafx.h"

<<<<<<< HEAD
extern HMODULE g_hDllMoudle;	//dll的句柄
//共享内存
#pragma data_seg("mydata")	//创建一个名为mydata的数据段
	HHOOK g_hHook = NULL;
#pragma data_seg()
#pragma comment(linker,"/SECTION:mydata,RWS")  //把mydata数据段设置为可读可写可共享
=======
extern HMODULE g_hDllMoudle;	
//共享内存
#pragma data_seg("mydata")	
	HHOOK g_hHook = NULL;
#pragma data_seg()
#pragma comment(linker,"/SECTION:mydata,RWS")  
>>>>>>> v1.0




extern "C" _declspec(dllexport)
<<<<<<< HEAD
//钩子回调函数
=======

>>>>>>> v1.0
LRESULT GetMsgProc(int code, WPARAM wParam, LPARAM lParam)
{
	MessageBoxA(0, "你中毒了", "你中毒了", 0);
	return CallNextHookEx(g_hHook, code, wParam, lParam);
}

extern "C" _declspec(dllexport)
<<<<<<< HEAD
//设置全局钩子
=======

>>>>>>> v1.0
BOOL SetGlobalHook()
{
	g_hHook = SetWindowsHookEx(WH_GETMESSAGE, (HOOKPROC)GetMsgProc, g_hDllMoudle, 0);
	if (g_hHook==NULL)
	{
		return FALSE;
	}
	return TRUE;
}

<<<<<<< HEAD

//卸载全局钩子
=======
>>>>>>> v1.0
