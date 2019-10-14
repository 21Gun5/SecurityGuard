#pragma once

#include <vector>
#include <list>
#include <TlHelp32.h>

#define ProcessTimes  4

// 重构进程信息结构体
struct PROCESSINFO : public PROCESSENTRY32
{
	double	 lfCPUUsage = 0;// CPU占用率
	DWORD dwMemoryUsage;// 内存占用率
};
// 进程时间相关结构体
typedef struct _KERNEL_USERTEXTIMES
{
	LARGE_INTEGER CreateTime;
	LARGE_INTEGER ExitTime;
	LARGE_INTEGER KernelTime;
	LARGE_INTEGER UserTime;
} KERNEL_USERTEXTIMES, *PKERNEL_USERTEXTIMES;
// 文件信息结构体
struct FILEINFO {
	DWORD dwFileAttributes;
	FILETIME ftCreationTime;
	FILETIME ftLastAccessTime;
	FILETIME ftLastWriteTime;
	DWORD nFileSizeHigh;
	DWORD nFileSizeLow;
	TCHAR path[MAX_PATH];
};

void GetAllRunningProcess(std::vector<PROCESSINFO>* processList);// 获取所有运行进程
void GetProcessAllModule(DWORD dwPid, std::vector<MODULEENTRY32>* moduleList);// 获取进程所有模块
double GetProcessCPUUsed(DWORD pid);// 获取进程CPU占用率
double GetProcessMemoryused(DWORD pid);// 获取进程内存占用率
void getProcessTime(DWORD dwPid, LARGE_INTEGER* kernelTIme, LARGE_INTEGER* userTime);// 获取进程相关时间
double	FILETIMEtoDouble(const _FILETIME&	fileTime);// 转换时间格式
void getWindows(std::list<HWND>* wndList); // 获取某窗口
bool findFiles(const TCHAR* dir, const TCHAR* filter, std::list<FILEINFO>* filelist, HWND hWnd, UINT uMsg);// 查找文件