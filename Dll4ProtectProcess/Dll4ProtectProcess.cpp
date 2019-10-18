// Dll4ProtectProcess.cpp : 定义 DLL 应用程序的导出函数。
//

#include "stdafx.h"
#include <stdio.h>
#include "DllProtectProcess.h"

#define _countof(a) sizeof(a)/sizeof(a[0])
HHOOK g_Hook = 0;
// 1 拦截消息的回调函数
LRESULT CALLBACK KeyboardProc(
	int code,           // 消息类型
	WPARAM wParam,      // 虚拟码
	LPARAM lParam) 		// 按键信息
{
	// 判断是否wParam与lParam都有键盘消息，是的话则执行打印操作
	//if (code == HC_ACTION)
	//{
	//	// 将256个虚拟键的状态拷贝到指定的缓冲区中，如果成功则继续
	//	BYTE KeyState[256] = { 0 };
	//	if (GetKeyboardState(KeyState))
	//	{
	//		// 得到第16–23位，键盘虚拟码
	//		LONG  KeyInfo = lParam;
	//		UINT  keyCode = (KeyInfo >> 16) & 0x00ff;
	//		WCHAR wKeyCode = 0;
	//		ToAscii((UINT)wParam, keyCode, KeyState, (LPWORD)&wKeyCode, 0);
	//		// 将其打印出来
	//		WCHAR szInfo[512] = { 0 };
	//		swprintf_s(szInfo, _countof(szInfo), L"Hook_%c", (char)wKeyCode);
	//		MessageBox(0, szInfo, 0, 0);
	//		//OutputDebugString(szInfo);
	//		return 0;
	//	}
	//}

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
	//id = 6340;
	// 获取当前模块的句柄
	HMODULE hModule = GetModuleHandle(L"Dll4ProtectProcess.dll");
	// 开启hook
	//g_Hook = SetWindowsHookEx(
	//	WH_KEYBOARD,// 截获什么类型的消息
	//	KeyboardProc,// 回调函数
	//	hModule,// 当前模块的句柄
	//	id// 可手动指定某个进程/线程，null表所有
	//);
	g_Hook = SetWindowsHookEx(
		WH_CBT,// 截获什么类型的消息
		KeyboardProc,// 回调函数
		hModule,// 当前模块的句柄
		id// 可手动指定某个进程/线程，null表所有
	);
}

// 3 关闭Hook的函数
void UnHook()
{
	if (g_Hook != 0)
	{
		// 关闭hook
		UnhookWindowsHookEx(g_Hook);
		g_Hook = 0;
	}
}
