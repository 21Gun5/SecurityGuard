//#pragma once
//
//#include <vector>
//#include <list>
//#include <TlHelp32.h>
//
//#define ProcessTimes  4
//
//
//
//// 重构进程信息结构体
//struct PROCESSINFO : public PROCESSENTRY32
//{
//	double	 lfCPUUsage = 0;// CPU占用率
//	DWORD dwMemoryUsage;// 内存占用率
//};
//// 进程时间相关结构体
//typedef struct _KERNEL_USERTEXTIMES
//{
//	LARGE_INTEGER CreateTime;
//	LARGE_INTEGER ExitTime;
//	LARGE_INTEGER KernelTime;
//	LARGE_INTEGER UserTime;
//} KERNEL_USERTEXTIMES, *PKERNEL_USERTEXTIMES;
//// 文件信息结构体
//struct FILEINFO {
//	DWORD dwFileAttributes;
//	FILETIME ftCreationTime;
//	FILETIME ftLastAccessTime;
//	FILETIME ftLastWriteTime;
//	DWORD nFileSizeHigh;
//	DWORD nFileSizeLow;
//	TCHAR path[MAX_PATH];
//};
//
//void GetAllRunningProcess(std::vector<PROCESSINFO>* processList);// 获取所有运行进程
//void GetProcessAllModule(DWORD dwPid, std::vector<MODULEENTRY32>* moduleList);// 获取进程所有模块
//void GetProcessAllThread(DWORD dwPid, std::vector<THREADENTRY32>* threadList);
//
//double GetProcessCPUUsed(DWORD pid);// 获取进程CPU占用率
//double GetProcessMemoryused(DWORD pid);// 获取进程内存占用率
//void getProcessTime(DWORD dwPid, LARGE_INTEGER* kernelTIme, LARGE_INTEGER* userTime);// 获取进程相关时间
//double	FILETIMEtoDouble(const _FILETIME&	fileTime);// 转换时间格式
//void getWindows(std::list<HWND>* wndList); // 获取某窗口
//bool findFiles(const TCHAR* dir, const TCHAR* filter, std::list<FILEINFO>* filelist, HWND hWnd, UINT uMsg);// 查找文件

#pragma once
#include <windows.h>
#include <vector>
#include <TlHelp32.h>
#include <list>


struct PROCESSINFO : public PROCESSENTRY32
{
	DWORD dwMemoryUsage; // 内存使用率
	double dCpuUsage = 0;// cpu使用率
};

/*!
 * 函数名 : getAllProcess
 * 功  能 : 获取本系统所有的进程
 * 参  数 : _Out_ std::vector<PROCESSENTRY32> * procList
 *         输出类型的参数, 遍历完之后,将得到的进程信息保存到vector中.
 * 返回值 : void
 */
bool getAllProcess(_Out_ std::vector<PROCESSINFO>* procList);
int IndexOfProcessList(std::vector<PROCESSINFO>& proclist, DWORD pid);
/*!
 * 函数名 : getProcessAllModule
 * 功  能 : 获取进程的所有模块
 * 参  数 : DWORD pid
 * 参  数 : std::vector<MODULEENTRY32> * moduleList
 * 返回值 : bool
 */
bool getProcessAllModule(DWORD dwPid, std::vector<MODULEENTRY32>* moduleList);
void GetProcessAllThread(DWORD dwPid, std::vector<THREADENTRY32>* threadList);
/*!
 * 函数名 : getProcessMemoryUsage
 * 功  能 : 获取进程内存使用率
 * 参  数 : DWORD pid
 * 返回值 : double
 */
double getProcessMemoryUsage(DWORD dwPid);

double getProcessCpuUsage(DWORD dwPid);

double getCpuUsage();

struct FILEINFO {
	DWORD dwFileAttributes;
	FILETIME ftCreationTime;
	FILETIME ftLastAccessTime;
	FILETIME ftLastWriteTime;
	DWORD nFileSizeHigh;
	DWORD nFileSizeLow;
	TCHAR path[MAX_PATH];
};

/*!
 * 函数名 : findFiles
 * 功  能 : 查找文件(本函数是线程安全的)
 * 参  数 : const TCHAR * dir 要查找的目录
 * 参  数 : const TCHAR * filter 文件后缀过滤器,每个后缀用空格间隔, 例如: ".obj .db .o"
 * 参  数 : std::list<FILEINFO> * filelist 保存遍历出来的文件信息的链表
 * 参  数 : HWND hWnd 可用于接收遍历出来的文件信息的窗口
 * 参  数 : UINT uMsg 自定义消息.
 *		 当hWnd和uMsg不为NULL时, 函数在遍历到一个文件信息后, 会将文件信息通过SendMessage
 *		 发送给窗口 , 发送的消息就使用uMsg指定的值. 消息的wParam将保存FILEINFO类型的指针.
 *		 lParam默认传0
 * 返回值 : bool
 */
bool findFiles(const TCHAR* dir, const TCHAR* filter, std::list<FILEINFO>* filelist = NULL, HWND hWnd = NULL, UINT uMsg = 0);
void getWindows(std::list<HWND>* wndList);