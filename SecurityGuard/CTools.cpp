////#include "stdafx.h"
////#include "CTools.h"
////#include <Winternl.h>
////#include <Psapi.h>
////
////
////
////// ��ȡ�������н���
////void GetAllRunningProcess(std::vector<PROCESSINFO>* processList)
////{
////	// TODO: �ڴ˴����ʵ�ִ���.
////
////	// ��ȡ���̿��վ��
////	HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);
////	// ���������Ϣ�ı���
////	PROCESSENTRY32 proc32 = { sizeof(PROCESSENTRY32) };
////	PROCESSINFO pe = *(PROCESSINFO*)&proc32;
////	// �������̣�����vector
////	Process32First(hSnap, &pe);
////	do {
////		processList->push_back(pe);
////	} while (Process32Next(hSnap, &pe));
////	// �رտ��վ��
////	CloseHandle(hSnap);
////}
////// ��ȡ��������ģ��
////void GetProcessAllModule(DWORD dwPid, std::vector<MODULEENTRY32>* moduleList)
////{
////	// ��ȡ���վ��������ģ��ʱ��ָ��pid
////	HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, dwPid);
////	// �洢ģ����Ϣ
////	MODULEENTRY32 mInfo = { sizeof(MODULEENTRY32) };
////	// ����ģ��
////	Module32First(hSnap, &mInfo);
////	do
////	{
////		moduleList->push_back(mInfo);
////	} while (Module32Next(hSnap, &mInfo));
////}
////void GetProcessAllThread(DWORD dwPid, std::vector<THREADENTRY32>* threadList)
////{
////	//1.�����߳̿���
////	HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, 0);
////
////	//2.��һ�α����߳�
////	THREADENTRY32 th32 = { sizeof(th32) };
////	if (Thread32First(hSnap, &th32))
////	{
////		//3.ѭ�������߳�
////		do
////		{
////			//�жϸ��߳��Ƿ������������
////			if (th32.th32OwnerProcessID == dwPid)
////			{
////				threadList->push_back(th32);
////				//printf("�߳�ID:%d\n", th32.th32ThreadID);
////			}
////
////		} while (Thread32Next(hSnap, &th32));
////
////		CloseHandle(hSnap);
////	}
////}
////// ��ȡ�����ڴ�ռ����
////double GetProcessMemoryused(DWORD pid)
////{
////	//	1. ��ر�������
////	double used = 0;
////	PROCESS_MEMORY_COUNTERS pmc = { sizeof(PROCESS_MEMORY_COUNTERS) };
////	// 2. �򿪽��̻�ȡ���
////	HANDLE hProc = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, pid);
////	// 3. ��ȡ�ڴ���Ϣ
////	GetProcessMemoryInfo(hProc, &pmc, sizeof(pmc));
////	used = pmc.WorkingSetSize;
////	// 4. �رվ��������������
////	CloseHandle(hProc);
////	return used / 1024;
////}
////// ��ȡ����CPUռ����
////double GetProcessCPUUsed(DWORD pid)
////{
////	// 0 . ��ʽ���ڴ�ռ���� =  (�����ں�ʱ�� + �����û�ʱ��) / (CPU�ں�ʱ�� + CPU�û�ʱ��)
////	// 1. ��ȡ����ʱ�䣨�ں�/�û�
////	LARGE_INTEGER kernelTime = { 0 };
////	LARGE_INTEGER userTime = { 0 };
////	getProcessTime(pid, &kernelTime, &userTime);
////	// 2. ��ȡCPUʱ�䣨����/�ں�/�û���
////	FILETIME CPUIdleTime = {}, CPUKernelTime = {}, CPUUserTime = {};
////	GetSystemTimes((FILETIME*)&CPUIdleTime, (FILETIME*)&CPUKernelTime, (FILETIME*)&CPUUserTime);
////	// 3. ����ռ���ʲ�����
////	double proc = FILETIMEtoDouble(*(FILETIME*)&kernelTime) + FILETIMEtoDouble(*(FILETIME*)&userTime);
////	double used = FILETIMEtoDouble(CPUKernelTime) + FILETIMEtoDouble(CPUKernelTime);
////	return proc / used * 100.0;
////}
////// ��ȡ�������ʱ��
////void getProcessTime(DWORD dwPid, LARGE_INTEGER* kernelTime, LARGE_INTEGER* userTime)
////{
////	// 1. ʱ��ṹ��
////	KERNEL_USERTEXTIMES time;
////	// 2. �򿪽��̻�ȡ���
////	HANDLE hProc = OpenProcess(PROCESS_QUERY_INFORMATION, FALSE, dwPid);
////	// 3. ��ȡʱ��
////	decltype(NtQueryInformationProcess)* fnNtQueryInformationProcess;
////	fnNtQueryInformationProcess = (decltype(NtQueryInformationProcess)*)
////		(GetProcAddress(GetModuleHandle(_T("ntdll.dll")), "NtQueryInformationProcess"));
////	DWORD read = 0;
////	fnNtQueryInformationProcess(hProc, (PROCESSINFOCLASS)ProcessTimes, &time, sizeof(time), &read);
////	// 4. ���ػ�õ�ʱ�䣨��ʵ��
////	*kernelTime = time.KernelTime;
////	*userTime = time.UserTime;
////	// 5. �رվ��
////	CloseHandle(hProc);
////}
////// ת��ʱ���ʽ
////double	FILETIMEtoDouble(const _FILETIME&	fileTime)
////{
////	return	double(fileTime.dwHighDateTime*4.294967296e9) + double(fileTime.dwLowDateTime);
////}
////
////
////// ��������ʱ�Ļص�����
////BOOL CALLBACK wndProc(HWND hWnd, LPARAM l)
////{
////	std::list<HWND>* list = (std::list<HWND>*)l;
////	list->push_back(hWnd);
////	return 1;
////}
////// ��ȡĳ����
////void getWindows(std::list<HWND>* wndList)
////{
////	EnumWindows(wndProc, (LPARAM)wndList);
////}
////
////
////// ��ʼ��һ��ȫ�ֻ�����,�ڱ����ļ�ʱʹ��.
////static HANDLE g_hMutex = []() -> HANDLE {
////	return CreateMutex(0, FALSE, NULL);
////}();
////
////// �����ļ�
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
////		// �Ƿ���Ŀ¼
////		if (fData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
////			// �ݹ�����¼�Ŀ¼
////			findFiles(path + "\\" + fData.cFileName, filter, filelist, hWnd, uMsg);
////		}
////		else {
////			// ��ȡ�ļ���չ��
////			TCHAR* pExtName = fData.cFileName + _tcslen(fData.cFileName);
////			while (pExtName != fData.cFileName && *pExtName != '.')
////				--pExtName;
////			// �ж���չ���Ƿ���Ҫ���˳������ļ���չ��
////			if (_tcsstr(filter, pExtName) != NULL) {
////				// ���ļ���Ϣ���浽������
////				memcpy(&fInfo, &fData, sizeof(FILEINFO) - sizeof(fInfo.path));
////				// ƴ��������·��
////				_stprintf_s(fInfo.path, _countof(fInfo.path), _T("%s\\%s"), dir, fData.cFileName);
////
////				// ����б�Ϊ��,�����ݱ��浽�б�
////				if (filelist)
////				{
////					WaitForSingleObject(g_hMutex, -1);
////					filelist->push_back(fInfo);
////					ReleaseMutex(g_hMutex);
////				}
////				// ������ھ����Ϊ��, �����ݷ��͵�����
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
	// 1. �ȸ���ǰϵͳ���еĽ�����һ������.
	HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (hSnap == INVALID_HANDLE_VALUE) {
		return false;
	}

	// 2. �ӿ�������ȡ��������Ϣ
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
	// 1. �ȸ���ǰϵͳ���еĽ�����һ������.
	HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, dwPid);
	if (hSnap == INVALID_HANDLE_VALUE) {
		return false;
	}

	// 2. �ӿ�������ȡ��������Ϣ
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
	//1.�����߳̿���
	HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, 0);

	//2.��һ�α����߳�
	THREADENTRY32 th32 = { sizeof(th32) };
	if (Thread32First(hSnap, &th32))
	{
		//3.ѭ�������߳�
		do
		{
			//�жϸ��߳��Ƿ������������
			if (th32.th32OwnerProcessID == dwPid)
			{
				threadList->push_back(th32);
				//printf("�߳�ID:%d\n", th32.th32ThreadID);
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
	// ��ʽ: (�����ں�ʱ�� + �����û�ʱ��) / (CPU�ں�ʱ�� + CPU�û�ʱ��)

	LARGE_INTEGER kernelTime = { 0 };
	LARGE_INTEGER userTime = { 0 };
	getProcessTime(dwPid, &kernelTime, &userTime);

	//			����ʱ��		�ں�ʱ��		�û�ʱ��
	FILETIME cpuIdleTime = {}, cpuKernelTime = {}, cpuUserTime = {};

	// ��ȡʱ��
	GetSystemTimes((FILETIME*)&cpuIdleTime, (FILETIME*)&cpuKernelTime, (FILETIME*)&cpuUserTime);

	double proc = FILETIME2Double(*(FILETIME*)&kernelTime) + FILETIME2Double(*(FILETIME*)&userTime);
	double cpu = FILETIME2Double(cpuKernelTime) + FILETIME2Double(cpuUserTime);
	return proc / cpu * 100.0;
}

double getCpuUsage()
{
	//			����ʱ��		�ں�ʱ��		�û�ʱ��
	_FILETIME	idleTime, kernelTime, userTime;
	// ��ȡʱ��
	GetSystemTimes(&idleTime, &kernelTime, &userTime);

	// �ȴ�1000����
	HANDLE	hEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
	// �ȴ�1000����,ʹ���ں˶���ȴ������ȷ
	WaitForSingleObject(hEvent, 1000);
	CloseHandle(hEvent);

	// ��ȡ�µ�ʱ��
	_FILETIME	newIdleTime, newKernelTime, newUserTime;
	GetSystemTimes(&newIdleTime, &newKernelTime, &newUserTime);
	// ������ʱ��ת��
	double	dOldIdleTime = FILETIME2Double(idleTime);
	double	dNewIdleTime = FILETIME2Double(newIdleTime);
	double	dOldKernelTime = FILETIME2Double(kernelTime);
	double	dNewKernelTime = FILETIME2Double(newKernelTime);
	double	dOldUserTime = FILETIME2Double(userTime);
	double	dNewUserTime = FILETIME2Double(newUserTime);
	// �����ʹ����
	//		ʹ���� = 100.0 - (�¿���ʱ�� - �ɿ���ʱ��) / (���ں�ʱ��-���ں�ʱ��  + ���û�ʱ�� - ���û�ʱ��) *100.0
	return	int(100.0 - (dNewIdleTime - dOldIdleTime) / (dNewKernelTime - dOldKernelTime + dNewUserTime - dOldUserTime)*100.0);
}


// ��ʼ��һ��ȫ�ֻ�����,�ڱ����ļ�ʱʹ��.
static HANDLE g_hMutex = []() -> HANDLE {
	return CreateMutex(0, FALSE, NULL);
}();

// �����ļ�
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
		// �Ƿ���Ŀ¼
		if (fData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
			// �ݹ�����¼�Ŀ¼
			findFiles(path + "\\" + fData.cFileName, filter, filelist, hWnd, uMsg);
		}
		else {
			// ��ȡ�ļ���չ��
			TCHAR* pExtName = fData.cFileName + _tcslen(fData.cFileName);
			while (pExtName != fData.cFileName && *pExtName != '.')
				--pExtName;
			// �ж���չ���Ƿ���Ҫ���˳������ļ���չ��
			if (_tcsstr(filter, pExtName) != NULL) 
			{
				// ���ļ���Ϣ���浽������
				memcpy(&fInfo, &fData, sizeof(FILEINFO) - sizeof(fInfo.path));
				// ƴ��������·��
				_stprintf_s(fInfo.path, _countof(fInfo.path), _T("%s\\%s"), dir, fData.cFileName);

				// ����б�Ϊ��,�����ݱ��浽�б�
				if (filelist)
				{
					WaitForSingleObject(g_hMutex, -1);
					filelist->push_back(fInfo);
					ReleaseMutex(g_hMutex);
				}
				// ������ھ����Ϊ��, �����ݷ��͵�����
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
