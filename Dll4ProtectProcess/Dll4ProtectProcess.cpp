// Dll4ProtectProcess.cpp : 定义 DLL 应用程序的导出函数。
//

#include "stdafx.h"
#include <stdio.h>
#include "DllProtectProcess.h"

#define _countof(a) sizeof(a)/sizeof(a[0])
HHOOK g_Hook = 0;

// 1 拦截消息的回调函数
LRESULT CALLBACK KeyboardProc(int code,WPARAM wParam, LPARAM lParam)
{
	if (code == HCBT_DESTROYWND)
	{
		MessageBox(0, L"本进程被保护，不可关闭", L"进程保护", 0);
		//UnHook();
		//OnHook(g_tid);
		return 0;
	}
	return CallNextHookEx(g_Hook, code, wParam, lParam);
}


// 2 开启Hook的函数
void OnHook(DWORD id)
{
	//..\Dll4ProtectProcess\DllProtectProcess.h
	
	//HMODULE hModule = GetModuleHandle(L".\\res\\ProtectProcess\\Dll4ProtectProcess.dll");
	HMODULE hModule = GetModuleHandle(L"Dll4ProtectProcess.dll");
	g_Hook = SetWindowsHookEx(WH_CBT,KeyboardProc,hModule,id);
}

// 3 关闭Hook的函数
void UnHook()
{
	if (g_Hook != 0)
	{
		UnhookWindowsHookEx(g_Hook);
		g_Hook = 0;
	}
}
