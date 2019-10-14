#pragma once

#include <vector>
#include <list>
#include <TlHelp32.h>

#define ProcessTimes  4

// �ع�������Ϣ�ṹ��
struct PROCESSINFO : public PROCESSENTRY32
{
	double	 lfCPUUsage = 0;// CPUռ����
	DWORD dwMemoryUsage;// �ڴ�ռ����
};
// ����ʱ����ؽṹ��
typedef struct _KERNEL_USERTEXTIMES
{
	LARGE_INTEGER CreateTime;
	LARGE_INTEGER ExitTime;
	LARGE_INTEGER KernelTime;
	LARGE_INTEGER UserTime;
} KERNEL_USERTEXTIMES, *PKERNEL_USERTEXTIMES;
// �ļ���Ϣ�ṹ��
struct FILEINFO {
	DWORD dwFileAttributes;
	FILETIME ftCreationTime;
	FILETIME ftLastAccessTime;
	FILETIME ftLastWriteTime;
	DWORD nFileSizeHigh;
	DWORD nFileSizeLow;
	TCHAR path[MAX_PATH];
};

void GetAllRunningProcess(std::vector<PROCESSINFO>* processList);// ��ȡ�������н���
void GetProcessAllModule(DWORD dwPid, std::vector<MODULEENTRY32>* moduleList);// ��ȡ��������ģ��
double GetProcessCPUUsed(DWORD pid);// ��ȡ����CPUռ����
double GetProcessMemoryused(DWORD pid);// ��ȡ�����ڴ�ռ����
void getProcessTime(DWORD dwPid, LARGE_INTEGER* kernelTIme, LARGE_INTEGER* userTime);// ��ȡ�������ʱ��
double	FILETIMEtoDouble(const _FILETIME&	fileTime);// ת��ʱ���ʽ
void getWindows(std::list<HWND>* wndList); // ��ȡĳ����
bool findFiles(const TCHAR* dir, const TCHAR* filter, std::list<FILEINFO>* filelist, HWND hWnd, UINT uMsg);// �����ļ�