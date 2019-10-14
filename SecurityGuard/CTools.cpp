#include "stdafx.h"
#include "CTools.h"
#include <Winternl.h>
#include <Psapi.h>



// ��ȡ�������н���
void GetAllRunningProcess(std::vector<PROCESSINFO>* processList)
{
	// TODO: �ڴ˴����ʵ�ִ���.

	// ��ȡ���̿��վ��
	HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);
	// ���������Ϣ�ı���
	PROCESSENTRY32 proc32 = { sizeof(PROCESSENTRY32) };
	PROCESSINFO pe = *(PROCESSINFO*)&proc32;
	// �������̣�����vector
	Process32First(hSnap, &pe);
	do {
		processList->push_back(pe);
	} while (Process32Next(hSnap, &pe));
	// �رտ��վ��
	CloseHandle(hSnap);
}
// ��ȡ��������ģ��
void GetProcessAllModule(DWORD dwPid, std::vector<MODULEENTRY32>* moduleList)
{
	// ��ȡ���վ��������ģ��ʱ��ָ��pid
	HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, dwPid);
	// �洢ģ����Ϣ
	MODULEENTRY32 mInfo = { sizeof(MODULEENTRY32) };
	// ����ģ��
	Module32First(hSnap, &mInfo);
	do
	{
		moduleList->push_back(mInfo);
	} while (Module32Next(hSnap, &mInfo));
}
// ��ȡ�����ڴ�ռ����
double GetProcessMemoryused(DWORD pid)
{
	//	1. ��ر�������
	double used = 0;
	PROCESS_MEMORY_COUNTERS pmc = { sizeof(PROCESS_MEMORY_COUNTERS) };
	// 2. �򿪽��̻�ȡ���
	HANDLE hProc = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, pid);
	// 3. ��ȡ�ڴ���Ϣ
	GetProcessMemoryInfo(hProc, &pmc, sizeof(pmc));
	used = pmc.WorkingSetSize;
	// 4. �رվ��������������
	CloseHandle(hProc);
	return used / 1024;
}
// ��ȡ����CPUռ����
double GetProcessCPUUsed(DWORD pid)
{
	// 0 . ��ʽ���ڴ�ռ���� =  (�����ں�ʱ�� + �����û�ʱ��) / (CPU�ں�ʱ�� + CPU�û�ʱ��)
	// 1. ��ȡ����ʱ�䣨�ں�/�û�
	LARGE_INTEGER kernelTime = { 0 };
	LARGE_INTEGER userTime = { 0 };
	getProcessTime(pid, &kernelTime, &userTime);
	// 2. ��ȡCPUʱ�䣨����/�ں�/�û���
	FILETIME CPUIdleTime = {}, CPUKernelTime = {}, CPUUserTime = {};
	GetSystemTimes((FILETIME*)&CPUIdleTime, (FILETIME*)&CPUKernelTime, (FILETIME*)&CPUUserTime);
	// 3. ����ռ���ʲ�����
	double proc = FILETIMEtoDouble(*(FILETIME*)&kernelTime) + FILETIMEtoDouble(*(FILETIME*)&userTime);
	double used = FILETIMEtoDouble(CPUKernelTime) + FILETIMEtoDouble(CPUKernelTime);
	return proc / used * 100.0;
}
// ��ȡ�������ʱ��
void getProcessTime(DWORD dwPid, LARGE_INTEGER* kernelTime, LARGE_INTEGER* userTime)
{
	// 1. ʱ��ṹ��
	KERNEL_USERTEXTIMES time;
	// 2. �򿪽��̻�ȡ���
	HANDLE hProc = OpenProcess(PROCESS_QUERY_INFORMATION, FALSE, dwPid);
	// 3. ��ȡʱ��
	decltype(NtQueryInformationProcess)* fnNtQueryInformationProcess;
	fnNtQueryInformationProcess = (decltype(NtQueryInformationProcess)*)
		(GetProcAddress(GetModuleHandle(_T("ntdll.dll")), "NtQueryInformationProcess"));
	DWORD read = 0;
	fnNtQueryInformationProcess(hProc, (PROCESSINFOCLASS)ProcessTimes, &time, sizeof(time), &read);
	// 4. ���ػ�õ�ʱ�䣨��ʵ��
	*kernelTime = time.KernelTime;
	*userTime = time.UserTime;
	// 5. �رվ��
	CloseHandle(hProc);
}
// ת��ʱ���ʽ
double	FILETIMEtoDouble(const _FILETIME&	fileTime)
{
	return	double(fileTime.dwHighDateTime*4.294967296e9) + double(fileTime.dwLowDateTime);
}


// ��������ʱ�Ļص�����
BOOL CALLBACK wndProc(HWND hWnd, LPARAM l)
{
	std::list<HWND>* list = (std::list<HWND>*)l;
	list->push_back(hWnd);
	return 1;
}
// ��ȡĳ����
void getWindows(std::list<HWND>* wndList)
{
	EnumWindows(wndProc, (LPARAM)wndList);
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
			if (_tcsstr(filter, pExtName) != NULL) {
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