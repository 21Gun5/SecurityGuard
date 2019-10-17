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
//// �ع�������Ϣ�ṹ��
//struct PROCESSINFO : public PROCESSENTRY32
//{
//	double	 lfCPUUsage = 0;// CPUռ����
//	DWORD dwMemoryUsage;// �ڴ�ռ����
//};
//// ����ʱ����ؽṹ��
//typedef struct _KERNEL_USERTEXTIMES
//{
//	LARGE_INTEGER CreateTime;
//	LARGE_INTEGER ExitTime;
//	LARGE_INTEGER KernelTime;
//	LARGE_INTEGER UserTime;
//} KERNEL_USERTEXTIMES, *PKERNEL_USERTEXTIMES;
//// �ļ���Ϣ�ṹ��
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
//void GetAllRunningProcess(std::vector<PROCESSINFO>* processList);// ��ȡ�������н���
//void GetProcessAllModule(DWORD dwPid, std::vector<MODULEENTRY32>* moduleList);// ��ȡ��������ģ��
//void GetProcessAllThread(DWORD dwPid, std::vector<THREADENTRY32>* threadList);
//
//double GetProcessCPUUsed(DWORD pid);// ��ȡ����CPUռ����
//double GetProcessMemoryused(DWORD pid);// ��ȡ�����ڴ�ռ����
//void getProcessTime(DWORD dwPid, LARGE_INTEGER* kernelTIme, LARGE_INTEGER* userTime);// ��ȡ�������ʱ��
//double	FILETIMEtoDouble(const _FILETIME&	fileTime);// ת��ʱ���ʽ
//void getWindows(std::list<HWND>* wndList); // ��ȡĳ����
//bool findFiles(const TCHAR* dir, const TCHAR* filter, std::list<FILEINFO>* filelist, HWND hWnd, UINT uMsg);// �����ļ�

#pragma once
#include <windows.h>
#include <vector>
#include <TlHelp32.h>
#include <list>


struct PROCESSINFO : public PROCESSENTRY32
{
	DWORD dwMemoryUsage; // �ڴ�ʹ����
	double dCpuUsage = 0;// cpuʹ����
};

/*!
 * ������ : getAllProcess
 * ��  �� : ��ȡ��ϵͳ���еĽ���
 * ��  �� : _Out_ std::vector<PROCESSENTRY32> * procList
 *         ������͵Ĳ���, ������֮��,���õ��Ľ�����Ϣ���浽vector��.
 * ����ֵ : void
 */
bool getAllProcess(_Out_ std::vector<PROCESSINFO>* procList);
int IndexOfProcessList(std::vector<PROCESSINFO>& proclist, DWORD pid);
/*!
 * ������ : getProcessAllModule
 * ��  �� : ��ȡ���̵�����ģ��
 * ��  �� : DWORD pid
 * ��  �� : std::vector<MODULEENTRY32> * moduleList
 * ����ֵ : bool
 */
bool getProcessAllModule(DWORD dwPid, std::vector<MODULEENTRY32>* moduleList);
void GetProcessAllThread(DWORD dwPid, std::vector<THREADENTRY32>* threadList);
/*!
 * ������ : getProcessMemoryUsage
 * ��  �� : ��ȡ�����ڴ�ʹ����
 * ��  �� : DWORD pid
 * ����ֵ : double
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
 * ������ : findFiles
 * ��  �� : �����ļ�(���������̰߳�ȫ��)
 * ��  �� : const TCHAR * dir Ҫ���ҵ�Ŀ¼
 * ��  �� : const TCHAR * filter �ļ���׺������,ÿ����׺�ÿո���, ����: ".obj .db .o"
 * ��  �� : std::list<FILEINFO> * filelist ��������������ļ���Ϣ������
 * ��  �� : HWND hWnd �����ڽ��ձ����������ļ���Ϣ�Ĵ���
 * ��  �� : UINT uMsg �Զ�����Ϣ.
 *		 ��hWnd��uMsg��ΪNULLʱ, �����ڱ�����һ���ļ���Ϣ��, �Ὣ�ļ���Ϣͨ��SendMessage
 *		 ���͸����� , ���͵���Ϣ��ʹ��uMsgָ����ֵ. ��Ϣ��wParam������FILEINFO���͵�ָ��.
 *		 lParamĬ�ϴ�0
 * ����ֵ : bool
 */
bool findFiles(const TCHAR* dir, const TCHAR* filter, std::list<FILEINFO>* filelist = NULL, HWND hWnd = NULL, UINT uMsg = 0);
void getWindows(std::list<HWND>* wndList);