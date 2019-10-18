// CProcessDlg.cpp: 实现文件
//

#include "stdafx.h"
#include "SecurityGuard.h"
#include "CProcessDlg.h"
#include "afxdialogex.h"
#include "CModuleDlg.h"
#include "CThreadDlg.h"
#include <Psapi.h>

CRITICAL_SECTION g_critical_section;

// CProcessDlg 对话框

IMPLEMENT_DYNAMIC(CProcessDlg, CDialogEx)

CProcessDlg::CProcessDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_CProcessDlg, pParent)
{
	InitializeCriticalSection(&g_critical_section);
}

CProcessDlg::~CProcessDlg()
{
}

void CProcessDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_list);
}

BEGIN_MESSAGE_MAP(CProcessDlg, CDialogEx)
	ON_WM_SIZE()
	ON_WM_TIMER()
	ON_NOTIFY(NM_RCLICK, IDC_LIST1, &CProcessDlg::OnRclickList1)
	ON_COMMAND(ID_32771, &CProcessDlg::OnMenuKillproc)
	ON_COMMAND(ID_32772, &CProcessDlg::OnMenuListmodule)
	ON_COMMAND(ID_32774, &CProcessDlg::OnMenuListThread)
	ON_BN_CLICKED(IDC_BUTTON_MEMSTATUS, &CProcessDlg::OnBnClickedButtonMemstatus)
	ON_BN_CLICKED(IDC_BUTTON_MEMCLEAN, &CProcessDlg::OnBnClickedButtonMemclean)
	ON_COMMAND(ID_PROTECTPROC, &CProcessDlg::OnProtectproc)
	ON_COMMAND(ID_deleteProtect, &CProcessDlg::Ondeleteprotect)
END_MESSAGE_MAP()

// CProcessDlg 消息处理程序
BOOL CProcessDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	m_menu.LoadMenu(IDR_MENU1);

	// TODO:  在此添加额外的初始化
	m_list.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);

	m_list.InsertColumn(0, L"名称", 0, 250);
	m_list.InsertColumn(1, L"PID", 0, 150);
	m_list.InsertColumn(2, L"CPU", 0, 150);
	m_list.InsertColumn(3, L"内存", 0, 150);


	SetTimer(0,/*定时器的ID,功能类似于控件ID*/
		1000,/*间隔时间*/
		NULL/*回调函数, 如果为空,那么本窗口将接收到WM_TIMER的消息*/);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}
// 定时器消息
void CProcessDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CDialogEx::OnTimer(nIDEvent);

	// 更新进程列表(cpu使用率,内存使用率)
	updateProcessList();
}
void CProcessDlg::updateProcessList()
{

	// 获取最新的进程列表
	std::vector<PROCESSINFO> newProcList;
	if (!getAllProcess(&newProcList))
	{
		return;
	}
	// 删除和增加属于原子操作, 所以需要进入临界区
	EnterCriticalSection(&g_critical_section);

	// 1. 找到已退出进程的名字,然后在列表控件中删除掉.
	// 1.1 使用旧列表的元素到新的列表中查找,没有找到,说明进程已退出
	int index = 0;
	for (auto itr = m_procList.begin();
		itr != m_procList.end();
		)
	{
		// 判断旧列表的元素是否存在于新列表中.
		if (-1 == IndexOfProcessList(newProcList, itr->th32ProcessID)) {
			// 不存在, 说明进程已退出, 将已退出的进程在
			//  数组和列表控件中删除,
			itr = m_procList.erase(itr);
			m_list.DeleteItem(index);
			continue;
		}
		// 刷新内存使用率
		CString buffer;
		DWORD memUsage = getProcessMemoryUsage(itr->th32ProcessID);
		if (itr->dwMemoryUsage != memUsage)
		{
			buffer.Format(_T("%12dKb"), memUsage);
			m_list.SetItemText(index, 3, buffer);
			itr->dwMemoryUsage = memUsage;
		}

		// 刷新CPU使用率
		double cpuUsage = getProcessCpuUsage(itr->th32ProcessID);

		if (abs(itr->dCpuUsage - cpuUsage) >= 0.001)
		{
			buffer.Format(_T("%.1lf%%"), cpuUsage);
			m_list.SetItemText(index, 2, buffer);
			itr->dCpuUsage = cpuUsage;
		}

		// 下一个
		++index;
		++itr;
	}

	// 2. 找到新创建的进程, 插入到列表控件中.
	// 2.1 使用新列表的元素到旧列表中查找, 没有找到,说明进程是新建的.		
	index = m_list.GetItemCount();
	for (auto&i : newProcList) {
		if (-1 == IndexOfProcessList(m_procList, i.th32ProcessID)) {
			// 插入到数组中
			m_procList.push_back(i);
			// 插入到列表控件中
			CString buffer;
			m_list.InsertItem(index, _T(""));
			m_list.UpdateWindow();
			buffer.Format(_T("%d"), i.th32ProcessID);
			m_list.SetItemText(index, 0, i.szExeFile);
			m_list.SetItemText(index, 1, buffer);
			m_list.SetItemText(index, 2, L"");
			buffer.Format(_T("%12dKb"), (double)i.dwMemoryUsage);
			m_list.SetItemText(index, 3, buffer);
			++index;
		}
	}

	// 离开临界区
	LeaveCriticalSection(&g_critical_section);
}
void CProcessDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	// TODO: 在此处添加消息处理程序代码
	if (m_list.m_hWnd) {
		m_list.SetWindowPos(0, 0, 0, cx, cy, SWP_NOZORDER);
	}
}

// 进程列表控件右键单击
void CProcessDlg::OnRclickList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);

	// 弹出菜单
	*pResult = 0;
	CMenu* pSubMenu = m_menu.GetSubMenu(0);
	CPoint pos;
	GetCursorPos(&pos);
	pSubMenu->TrackPopupMenu(0, pos.x, pos.y, this);
}
void CProcessDlg::OnMenuKillproc()
{
	// TODO: 在此添加命令处理程序代码

	// 获取待结束进程（通过光标选择序号，序号从1开始，故-1
	int index = (int)m_list.GetFirstSelectedItemPosition() - 1;
	// 获取进程id（字符串转整形
	CString strPid = m_list.GetItemText(index, 1);// 第1列是pid
	DWORD dwPid = _wtoi(strPid);
	// 获取进程句柄（要通过句柄来结束进程
	HANDLE hProc = OpenProcess(PROCESS_TERMINATE, FALSE, dwPid);
	// 结束进程
	TerminateProcess(hProc, 0);
	// 关闭句柄
	CloseHandle(hProc);
}
void CProcessDlg::OnMenuListmodule()
{
	// TODO: 在此添加命令处理程序代码

	// 创建模块对话框
	CModuleDlg moduleDlg(this);
	// 获取被点击的进程（通过光标选择序号，序号从1开始，故-1
	int index = (int)m_list.GetFirstSelectedItemPosition() - 1;
	// 传递进程PID/名字给模块对话框
	moduleDlg.SetProcessID(m_procList[index].th32ProcessID);
	CString buffer;
	buffer.Format(L"%s - 模块列表", m_procList[index].szExeFile);
	moduleDlg.SetProcessName(buffer);
	// 运行对话框
	moduleDlg.DoModal();
}
void CProcessDlg::OnMenuListThread()
{
	// TODO: 在此添加命令处理程序代码

	// 创建模块对话框
	CThreadDlg moduleDlg(this);
	// 获取被点击的进程（通过光标选择序号，序号从1开始，故-1
	int index = (int)m_list.GetFirstSelectedItemPosition() - 1;
	// 传递进程PID/名字给模块对话框
	moduleDlg.SetProcessID(m_procList[index].th32ProcessID);
	CString buffer;
	buffer.Format(L"%s - 线程列表", m_procList[index].szExeFile);
	moduleDlg.SetProcessName(buffer);
	// 运行对话框
	moduleDlg.DoModal();
}


// 获取内存使用状态
void CProcessDlg::OnBnClickedButtonMemstatus()
{
	// TODO: 在此添加控件通知处理程序代码
	MEMORYSTATUSEX stcMemStatusEx = { 0 };
	stcMemStatusEx.dwLength = sizeof(stcMemStatusEx);
	GlobalMemoryStatusEx(&stcMemStatusEx);
	DWORDLONG preUsedMem = stcMemStatusEx.ullTotalPhys - stcMemStatusEx.ullAvailPhys;
	CString buff;
	buff.Format(L"内存已使用: %lld MB", preUsedMem / (1024 * 1024));
	MessageBox(buff);
}

// 清理内存
void CProcessDlg::OnBnClickedButtonMemclean()
{
	// TODO: 在此添加控件通知处理程序代码

	MEMORYSTATUSEX stcMemStatusEx = { 0 };
	stcMemStatusEx.dwLength = sizeof(stcMemStatusEx);
	GlobalMemoryStatusEx(&stcMemStatusEx);
	DWORDLONG preUsedMem = stcMemStatusEx.ullTotalPhys - stcMemStatusEx.ullAvailPhys;

	DWORD dwPIDlist[1000] = { 0 };
	DWORD bufSize = sizeof(dwPIDlist);
	DWORD dwNeedSize = 0;
	EnumProcesses(dwPIDlist, bufSize, &dwNeedSize);
	for (DWORD i = 0; i < dwNeedSize / sizeof(DWORD); i++)
	{
		HANDLE hProcess = OpenProcess(PROCESS_SET_QUOTA, false, dwPIDlist[i]);
		SetProcessWorkingSetSize(hProcess, -1, -1);
	}

	GlobalMemoryStatusEx(&stcMemStatusEx);
	DWORDLONG afterCleanUsedMem = stcMemStatusEx.ullTotalPhys - stcMemStatusEx.ullAvailPhys;
	CString buff;
	buff.Format(L"成功释放内存: %lld MB", (preUsedMem - afterCleanUsedMem) / (1024 * 1024));
	MessageBox(buff);

}


#include "..\Dll4ProtectProcess\DllProtectProcess.h"
#pragma comment (lib,".\\res\\ProtectProcess\\Dll4ProtectProcess.lib")

DWORD GetThreadID(DWORD ProcessId)
{
	puts("Getting Thread ID");
	HANDLE    ProcessHandle = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, 0);
	if (ProcessHandle != INVALID_HANDLE_VALUE)
	{
		THREADENTRY32 te;
		te.dwSize = sizeof(te);
		if (Thread32First(ProcessHandle, &te))
		{
			do
			{
				if (te.dwSize >= FIELD_OFFSET(THREADENTRY32, th32OwnerProcessID) + sizeof(te.th32OwnerProcessID))
				{
					if (te.th32OwnerProcessID == ProcessId)
					{
						HANDLE ThreadHandle = OpenThread(READ_CONTROL, FALSE, te.th32ThreadID);
						if (!ThreadHandle)
						{
							puts("Couldn't get thread handle");
						}
						else
						{
							//DWORD tpid = GetProcessIdOfThread(hThread);
							//printf("Got one: %u\n", tpid);
							return te.th32ThreadID;
						}
					}
				}
			} while (Thread32Next(ProcessHandle, &te));
		}
	}
	CloseHandle(ProcessHandle);
	return (DWORD)0;
}

void CProcessDlg::OnProtectproc()
{
	// TODO: 在此添加命令处理程序代码

	//// 获取待保护进程的PID
	int index = (int)m_list.GetFirstSelectedItemPosition() - 1;
	DWORD dwId = m_procList[index].th32ProcessID;

	DWORD dwTId  = GetThreadID(dwId);

	OnHook(dwTId);

}

void CProcessDlg::Ondeleteprotect()
{
	// TODO: 在此添加命令处理程序代码
	UnHook();
}
