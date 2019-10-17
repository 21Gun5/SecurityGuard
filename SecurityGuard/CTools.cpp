////#include "stdafx.h"
////#include "CTools.h"
////#include <Winternl.h>
////#include <Psapi.h>
////
////
////
////// 获取所有运行进程
////void GetAllRunningProcess(std::vector<PROCESSINFO>* processList)
////{
////	// TODO: 在此处添加实现代码.
////
////	// 获取进程快照句柄
////	HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);
////	// 保存进程信息的变量
////	PROCESSENTRY32 proc32 = { sizeof(PROCESSENTRY32) };
////	PROCESSINFO pe = *(PROCESSINFO*)&proc32;
////	// 遍历进程，添入vector
////	Process32First(hSnap, &pe);
////	do {
////		processList->push_back(pe);
////	} while (Process32Next(hSnap, &pe));
////	// 关闭快照句柄
////	CloseHandle(hSnap);
////}
////// 获取进程所有模块
////void GetProcessAllModule(DWORD dwPid, std::vector<MODULEENTRY32>* moduleList)
////{
////	// 获取快照句柄（遍历模块时需指定pid
////	HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, dwPid);
////	// 存储模块信息
////	MODULEENTRY32 mInfo = { sizeof(MODULEENTRY32) };
////	// 遍历模块
////	Module32First(hSnap, &mInfo);
////	do
////	{
////		moduleList->push_back(mInfo);
////	} while (Module32Next(hSnap, &mInfo));
////}
////void GetProcessAllThread(DWORD dwPid, std::vector<THREADENTRY32>* threadList)
////{
////	//1.创建线程快照
////	HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, 0);
////
////	//2.第一次遍历线程
////	THREADENTRY32 th32 = { sizeof(th32) };
////	if (Thread32First(hSnap, &th32))
////	{
////		//3.循环遍历线程
////		do
////		{
////			//判断该线程是否属于这个进程
////			if (th32.th32OwnerProcessID == dwPid)
////			{
////				threadList->push_back(th32);
////				//printf("线程ID:%d\n", th32.th32ThreadID);
////			}
////
////		} while (Thread32Next(hSnap, &th32));
////
////		CloseHandle(hSnap);
////	}
////}
////// 获取进程内存占用率
////double GetProcessMemoryused(DWORD pid)
////{
////	//	1. 相关变量设置
////	double used = 0;
////	PROCESS_MEMORY_COUNTERS pmc = { sizeof(PROCESS_MEMORY_COUNTERS) };
////	// 2. 打开进程获取句柄
////	HANDLE hProc = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, pid);
////	// 3. 获取内存信息
////	GetProcessMemoryInfo(hProc, &pmc, sizeof(pmc));
////	used = pmc.WorkingSetSize;
////	// 4. 关闭句柄并返回利用率
////	CloseHandle(hProc);
////	return used / 1024;
////}
////// 获取进程CPU占用率
////double GetProcessCPUUsed(DWORD pid)
////{
////	// 0 . 公式：内存占用率 =  (进程内核时间 + 进程用户时间) / (CPU内核时间 + CPU用户时间)
////	// 1. 获取进程时间（内核/用户
////	LARGE_INTEGER kernelTime = { 0 };
////	LARGE_INTEGER userTime = { 0 };
////	getProcessTime(pid, &kernelTime, &userTime);
////	// 2. 获取CPU时间（空闲/内核/用户）
////	FILETIME CPUIdleTime = {}, CPUKernelTime = {}, CPUUserTime = {};
////	GetSystemTimes((FILETIME*)&CPUIdleTime, (FILETIME*)&CPUKernelTime, (FILETIME*)&CPUUserTime);
////	// 3. 计算占用率并返回
////	double proc = FILETIMEtoDouble(*(FILETIME*)&kernelTime) + FILETIMEtoDouble(*(FILETIME*)&userTime);
////	double used = FILETIMEtoDouble(CPUKernelTime) + FILETIMEtoDouble(CPUKernelTime);
////	return proc / used * 100.0;
////}
////// 获取进程相关时间
////void getProcessTime(DWORD dwPid, LARGE_INTEGER* kernelTime, LARGE_INTEGER* userTime)
////{
////	// 1. 时间结构体
////	KERNEL_USERTEXTIMES time;
////	// 2. 打开进程获取句柄
////	HANDLE hProc = OpenProcess(PROCESS_QUERY_INFORMATION, FALSE, dwPid);
////	// 3. 获取时间
////	decltype(NtQueryInformationProcess)* fnNtQueryInformationProcess;
////	fnNtQueryInformationProcess = (decltype(NtQueryInformationProcess)*)
////		(GetProcAddress(GetModuleHandle(_T("ntdll.dll")), "NtQueryInformationProcess"));
////	DWORD read = 0;
////	fnNtQueryInformationProcess(hProc, (PROCESSINFOCLASS)ProcessTimes, &time, sizeof(time), &read);
////	// 4. 返回获得的时间（改实参
////	*kernelTime = time.KernelTime;
////	*userTime = time.UserTime;
////	// 5. 关闭句柄
////	CloseHandle(hProc);
////}
////// 转换时间格式
////double	FILETIMEtoDouble(const _FILETIME&	fileTime)
////{
////	return	double(fileTime.dwHighDateTime*4.294967296e9) + double(fileTime.dwLowDateTime);
////}
////
////
////// 遍历窗口时的回调函数
////BOOL CALLBACK wndProc(HWND hWnd, LPARAM l)
////{
////	std::list<HWND>* list = (std::list<HWND>*)l;
////	list->push_back(hWnd);
////	return 1;
////}
////// 获取某窗口
////void getWindows(std::list<HWND>* wndList)
////{
////	EnumWindows(wndProc, (LPARAM)wndList);
////}
////
////
////// 初始化一个全局互斥体,在遍历文件时使用.
////static HANDLE g_hMutex = []() -> HANDLE {
////	return CreateMutex(0, FALSE, NULL);
////}();
////
////// 查找文件
////bool findFiles(const TCHAR* dir, const TCHAR* filter, std::list<FILEINFO>* filelist, HWND hWnd, UINT uMsg)
////{
////	CString path = dir;
////	WIN32_FIND_DATA fData = { 0 };
////	HANDLE hFind = FindFirstFile(path + _T("\\*"), &fData);
////	if (hFind == INVALID_HANDLE_VALUE) {
////		return false;
////	}
////	FILEINFO fInfo = { 0 };
////	do
////	{
////		if (_tcscmp(fData.cFileName, _T(".")) == 0 || _tcscmp(fData.cFileName, _T("..")) == 0) {
////			continue;
////		}
////		// 是否是目录
////		if (fData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
////			// 递归遍历下级目录
////			findFiles(path + "\\" + fData.cFileName, filter, filelist, hWnd, uMsg);
////		}
////		else {
////			// 获取文件扩展名
////			TCHAR* pExtName = fData.cFileName + _tcslen(fData.cFileName);
////			while (pExtName != fData.cFileName && *pExtName != '.')
////				--pExtName;
////			// 判断扩展名是否是要过滤出来的文件扩展名
////			if (_tcsstr(filter, pExtName) != NULL) {
////				// 将文件信息保存到数组中
////				memcpy(&fInfo, &fData, sizeof(FILEINFO) - sizeof(fInfo.path));
////				// 拼接完整的路径
////				_stprintf_s(fInfo.path, _countof(fInfo.path), _T("%s\\%s"), dir, fData.cFileName);
////
////				// 如果列表不为空,则将数据保存到列表
////				if (filelist)
////				{
////					WaitForSingleObject(g_hMutex, -1);
////					filelist->push_back(fInfo);
////					ReleaseMutex(g_hMutex);
////				}
////				// 如果窗口句柄不为空, 则将数据发送到窗口
////				if (hWnd)
////				{
////					SendMessage(hWnd, uMsg, (WPARAM)&fInfo, 0);
////				}
////			}
////		}
////	} while (FindNextFile(hFind, &fData));
////	return true;
////}

#include "stdafx.h"
#include "CTools.h"
#include <Psapi.h>

bool getAllProcess(_Out_ std::vector<PROCESSINFO>* procList)
{
	// 1. 先给当前系统所有的进程拍一个快照.
	HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (hSnap == INVALID_HANDLE_VALUE) {
		return false;
	}

	// 2. 从快照中提取出进程信息
	PROCESSENTRY32 proc32 = { sizeof(PROCESSENTRY32) };
	Process32First(hSnap, &proc32);
	do
	{
		PROCESSINFO pi = *(PROCESSINFO*)&proc32;
		pi.dwMemoryUsage = getProcessMemoryUsage(proc32.th32ProcessID);
		procList->push_back(pi);
	} while (Process32Next(hSnap, &proc32));
	return true;
}
int IndexOfProcessList(std::vector<PROCESSINFO>& proclist, DWORD pid)
{
	DWORD size = proclist.size();
	for (int i = 0; i < size; ++i) {
		if (proclist[i].th32ProcessID == pid) {
			return i;
		}
	}
	return -1;
}
bool getProcessAllModule(DWORD dwPid, std::vector<MODULEENTRY32>* moduleList)
{
	// 1. 先给当前系统所有的进程拍一个快照.
	HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, dwPid);
	if (hSnap == INVALID_HANDLE_VALUE) {
		return false;
	}

	// 2. 从快照中提取出进程信息
	MODULEENTRY32 mInfo = { sizeof(MODULEENTRY32) };
	if (!Module32First(hSnap, &mInfo)) {
		return FALSE;
	}
	do
	{
		moduleList->push_back(mInfo);
	} while (Module32Next(hSnap, &mInfo));
	return true;
}
void GetProcessAllThread(DWORD dwPid, std::vector<THREADENTRY32>* threadList)
{
	//1.创建线程快照
	HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, 0);

	//2.第一次遍历线程
	THREADENTRY32 th32 = { sizeof(th32) };
	if (Thread32First(hSnap, &th32))
	{
		//3.循环遍历线程
		do
		{
			//判断该线程是否属于这个进程
			if (th32.th32OwnerProcessID == dwPid)
			{
				threadList->push_back(th32);
				//printf("线程ID:%d\n", th32.th32ThreadID);
			}

		} while (Thread32Next(hSnap, &th32));

		CloseHandle(hSnap);
	}
}
double getProcessMemoryUsage(DWORD dwPid)
{
	HANDLE hProc = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ,
		FALSE,
		dwPid);
	double usage = 0;
	if (hProc != NULL) {

		PROCESS_MEMORY_COUNTERS pmc = { sizeof(PROCESS_MEMORY_COUNTERS) };;
		GetProcessMemoryInfo(hProc, &pmc, sizeof(pmc));
		usage = pmc.WorkingSetSize;
		CloseHandle(hProc);
	}
	return usage / 1024;
}


#include <Winternl.h>
void getProcessTime(DWORD dwPid, LARGE_INTEGER* kernelTIme, LARGE_INTEGER* userTime)
{
	typedef struct _KERNEL_USERTEXTIMES
	{
		LARGE_INTEGER CreateTime;
		LARGE_INTEGER ExitTime;
		LARGE_INTEGER KernelTime;
		LARGE_INTEGER UserTime;
	} KERNEL_USERTEXTIMES, *PKERNEL_USERTEXTIMES;
	KERNEL_USERTEXTIMES time;
#define ProcessTimes  4

	HANDLE hProc = OpenProcess(PROCESS_QUERY_INFORMATION, FALSE, dwPid);
	if (hProc == NULL) {
		return;
	}
	decltype(NtQueryInformationProcess)* fnNtQueryInformationProcess;
	fnNtQueryInformationProcess = (decltype(NtQueryInformationProcess)*)
		(GetProcAddress(GetModuleHandle(_T("ntdll.dll")), "NtQueryInformationProcess"));

	DWORD read = 0;
	fnNtQueryInformationProcess(hProc,
		(PROCESSINFOCLASS)ProcessTimes,
		&time,
		sizeof(time),
		&read);
	*kernelTIme = time.KernelTime;
	*userTime = time.UserTime;
	CloseHandle(hProc);
}

double	FILETIME2Double(const _FILETIME&	fileTime)
{
	return	double(fileTime.dwHighDateTime*4.294967296e9) + double(fileTime.dwLowDateTime);
}

double getProcessCpuUsage(DWORD dwPid)
{
	// 公式: (进程内核时间 + 进程用户时间) / (CPU内核时间 + CPU用户时间)

	LARGE_INTEGER kernelTime = { 0 };
	LARGE_INTEGER userTime = { 0 };
	getProcessTime(dwPid, &kernelTime, &userTime);

	//			空闲时间		内核时间		用户时间
	FILETIME cpuIdleTime = {}, cpuKernelTime = {}, cpuUserTime = {};

	// 获取时间
	GetSystemTimes((FILETIME*)&cpuIdleTime, (FILETIME*)&cpuKernelTime, (FILETIME*)&cpuUserTime);

	double proc = FILETIME2Double(*(FILETIME*)&kernelTime) + FILETIME2Double(*(FILETIME*)&userTime);
	double cpu = FILETIME2Double(cpuKernelTime) + FILETIME2Double(cpuUserTime);
	return proc / cpu * 100.0;
}

double getCpuUsage()
{
	//			空闲时间		内核时间		用户时间
	_FILETIME	idleTime, kernelTime, userTime;
	// 获取时间
	GetSystemTimes(&idleTime, &kernelTime, &userTime);

	// 等待1000毫秒
	HANDLE	hEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
	// 等待1000毫秒,使用内核对象等待会更精确
	WaitForSingleObject(hEvent, 1000);
	CloseHandle(hEvent);

	// 获取新的时间
	_FILETIME	newIdleTime, newKernelTime, newUserTime;
	GetSystemTimes(&newIdleTime, &newKernelTime, &newUserTime);
	// 将各个时间转换
	double	dOldIdleTime = FILETIME2Double(idleTime);
	double	dNewIdleTime = FILETIME2Double(newIdleTime);
	double	dOldKernelTime = FILETIME2Double(kernelTime);
	double	dNewKernelTime = FILETIME2Double(newKernelTime);
	double	dOldUserTime = FILETIME2Double(userTime);
	double	dNewUserTime = FILETIME2Double(newUserTime);
	// 计算出使用率
	//		使用率 = 100.0 - (新空闲时间 - 旧空闲时间) / (新内核时间-旧内核时间  + 新用户时间 - 旧用户时间) *100.0
	return	int(100.0 - (dNewIdleTime - dOldIdleTime) / (dNewKernelTime - dOldKernelTime + dNewUserTime - dOldUserTime)*100.0);
}


// 初始化一个全局互斥体,在遍历文件时使用.
static HANDLE g_hMutex = []() -> HANDLE {
	return CreateMutex(0, FALSE, NULL);
}();

// 查找文件
bool findFiles(const TCHAR* dir, const TCHAR* filter, std::list<FILEINFO>* filelist, HWND hWnd, UINT uMsg)
{
	CString path = dir;
	WIN32_FIND_DATA fData = { 0 };
	HANDLE hFind = FindFirstFile(path + _T("\\*"), &fData);
	if (hFind == INVALID_HANDLE_VALUE) {
		return false;
	}
	FILEINFO fInfo = { 0 };
	do
	{
		if (_tcscmp(fData.cFileName, _T(".")) == 0 || _tcscmp(fData.cFileName, _T("..")) == 0) {
			continue;
		}
		// 是否是目录
		if (fData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
			// 递归遍历下级目录
			findFiles(path + "\\" + fData.cFileName, filter, filelist, hWnd, uMsg);
		}
		else {
			// 获取文件扩展名
			TCHAR* pExtName = fData.cFileName + _tcslen(fData.cFileName);
			while (pExtName != fData.cFileName && *pExtName != '.')
				--pExtName;
			// 判断扩展名是否是要过滤出来的文件扩展名
			if (_tcsstr(filter, pExtName) != NULL) 
			{
				// 将文件信息保存到数组中
				memcpy(&fInfo, &fData, sizeof(FILEINFO) - sizeof(fInfo.path));
				// 拼接完整的路径
				_stprintf_s(fInfo.path, _countof(fInfo.path), _T("%s\\%s"), dir, fData.cFileName);

				// 如果列表不为空,则将数据保存到列表
				if (filelist)
				{
					WaitForSingleObject(g_hMutex, -1);
					filelist->push_back(fInfo);
					ReleaseMutex(g_hMutex);
				}
				// 如果窗口句柄不为空, 则将数据发送到窗口
				if (hWnd)
				{
					SendMessage(hWnd, uMsg, (WPARAM)&fInfo, 0);
				}
			}
		}
	} while (FindNextFile(hFind, &fData));
	return true;
}



BOOL CALLBACK wndProc(HWND hWnd, LPARAM l)
{
	std::list<HWND>* list = (std::list<HWND>*)l;
	list->push_back(hWnd);
	return 1;
}

void getWindows(std::list<HWND>* wndList)
{
	EnumWindows(wndProc, (LPARAM)wndList);
}
