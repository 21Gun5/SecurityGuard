// CServiceDlg.cpp: 实现文件
//

#include "stdafx.h"
#include "SecurityGuard.h"
#include "CServiceDlg.h"
#include "afxdialogex.h"


// CServiceDlg 对话框

IMPLEMENT_DYNAMIC(CServiceDlg, CDialogEx)

CServiceDlg::CServiceDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_CServiceDlg, pParent)
{

}

CServiceDlg::~CServiceDlg()
{
}

void CServiceDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_list);
}


BEGIN_MESSAGE_MAP(CServiceDlg, CDialogEx)
	//	ON_NOTIFY(NM_CLICK, IDC_LIST1, &CServiceDlg::OnClickList1)
	ON_NOTIFY(NM_RCLICK, IDC_LIST1, &CServiceDlg::OnRclickList1)
	ON_COMMAND(ID_startService, &CServiceDlg::Onstartservice)
	ON_COMMAND(ID_stopService, &CServiceDlg::Onstopservice)
END_MESSAGE_MAP()


// CServiceDlg 消息处理程序


BOOL CServiceDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化

	m_menu.LoadMenu(IDR_MENU4);

	m_list.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	m_list.InsertColumn(0, L"名称", 0, 150);
	m_list.InsertColumn(1, L"状态", 0, 150);
	m_list.InsertColumn(2, L"类型", 0, 250);
	m_list.InsertColumn(3, L"启动类型", 0, 250);
	//m_list.InsertColumn(4, L"路径", 0, 250);

	UpdateList();


	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}

void CServiceDlg::UpdateList()
{
	//1. 打开远程计算机服务控制管理器
	SC_HANDLE hSCM = OpenSCManagerW(NULL, NULL, SC_MANAGER_ALL_ACCESS);
	//2. 第一次调用，获取需要的内存大小 
	DWORD dwServiceNum = 0;
	DWORD dwSize = 0;
	EnumServicesStatusEx(hSCM,
		SC_ENUM_PROCESS_INFO,
		SERVICE_WIN32,
		SERVICE_STATE_ALL, //所有服务状态
		NULL,//缓冲区
		0,//缓冲区大小
		&dwSize,//需要的大小
		&dwServiceNum, //缓冲区中的服务个数
		NULL, NULL);

	//3. 申请需要的内存，第二次调用
	LPENUM_SERVICE_STATUS_PROCESS pEnumService = (LPENUM_SERVICE_STATUS_PROCESS) new char[dwSize];
	// 第二次枚举
	bool bStatus = FALSE;
	bStatus = EnumServicesStatusEx(hSCM,
		SC_ENUM_PROCESS_INFO,
		SERVICE_WIN32,
		SERVICE_STATE_ALL, //服务状态
		(PBYTE)pEnumService,
		dwSize,//缓冲区大小                  
		&dwSize,//需要的大小
		&dwServiceNum, //缓冲区中的服务个数
		NULL, NULL);
	// 5遍历信息
	CString state[7] = { L"STOPPED",L"START_PENDING",L"STOP_PENDING",
			L"RUNNING",L"CONTINUE_PENDING",L"PAUSE_PENDING",L"PAUSED" };
	CString startType[5] = { L"BOOT_START",L"SYSTEM_START",L"AUTO_START",
		L"DEMAND_START",L"DISABLED" };
	for (DWORD i = 0; i < dwServiceNum; i++)
	{
		CString buffer;
		m_list.InsertItem(i, _T(""));// 插入行
		m_list.SetItemText(i, 0, pEnumService[i].lpServiceName);
		//if(pEnumService[i].ServiceStatusProcess.dwCurrentState - 1 >=0&& pEnumService[i].ServiceStatusProcess.dwCurrentState - 1<=6)
		m_list.SetItemText(i, 1, state[pEnumService[i].ServiceStatusProcess.dwCurrentState - 1]);
		switch (pEnumService[i].ServiceStatusProcess.dwServiceType)
		{
		case 0x00000001:
			buffer = L"KERNEL_DRIVER";
			break;
		case 0x00000002:
			buffer = L"FILE_SYSTEM_DRIVER";
			break;
		case 0x00000010:
			buffer = L"WIN32_OWN_PROCESS";
			break;
		case 0x00000020:
			buffer = L"WIN32_SHARE_PROCESS";
			break;
		case 0x00000050:
			buffer = L"USER_OWN_PROCESS";
			break;
		case 0x00000060:
			buffer = L"USER_SHARE_PROCESS";
			break;
		default:
			break;
		}
		m_list.SetItemText(i, 2, buffer);

		// 6 获取服务的详细信息 
		SC_HANDLE hService = OpenService(hSCM, pEnumService[i].lpDisplayName, SERVICE_QUERY_CONFIG);
		QueryServiceConfig(hService, NULL, 0, &dwSize);
		LPQUERY_SERVICE_CONFIG pServiceConfig = (LPQUERY_SERVICE_CONFIG)new char[dwSize];
		QueryServiceConfig(hService, pServiceConfig, dwSize, &dwSize);
		if (pServiceConfig->dwStartType <= 4 && pServiceConfig->dwStartType >= 0)
			m_list.SetItemText(i, 3, startType[pServiceConfig->dwStartType]);
		/*else
			m_list.SetItemText(i, 3, L"-");*/

		CloseServiceHandle(hService);
	}

	
	CloseServiceHandle(hSCM);
	return;

}

void CServiceDlg::OnRclickList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;

	// 弹出菜单
	*pResult = 0;
	CMenu* pSubMenu = m_menu.GetSubMenu(0);
	CPoint pos;
	GetCursorPos(&pos);
	pSubMenu->TrackPopupMenu(0, pos.x, pos.y, this);
}

void CServiceDlg::Onstartservice()
{
	// TODO: 在此添加命令处理程序代码
	int index = (int)m_list.GetFirstSelectedItemPosition() - 1;
	CString serviceName = m_list.GetItemText(index, 0);
	// 打开服务管理对象
	SC_HANDLE hSCM = OpenSCManagerW(NULL, NULL, SC_MANAGER_ALL_ACCESS);
	// 打开服务。
	SC_HANDLE hSvc = OpenService(hSCM, serviceName, SERVICE_START | SERVICE_QUERY_STATUS | SERVICE_STOP);
	// 获得服务的状态
	SERVICE_STATUS status;
	QueryServiceStatus(hSvc, &status);
	//如果处于停止状态则启动服务，否则停止服务。
	if (status.dwCurrentState == SERVICE_RUNNING)
	{
		MessageBox(L"原本就是运行状态");
	}
	else if (status.dwCurrentState == SERVICE_STOPPED)
	{
		// 启动服务
		StartService(hSvc, NULL, NULL);
		// 等待服务启动
		while (QueryServiceStatus(hSvc, &status) == TRUE)
		{
			Sleep(status.dwWaitHint);
			if (status.dwCurrentState == SERVICE_RUNNING)
			{
				MessageBox(L"开启成功", 0, 0);
				m_list.DeleteAllItems();
				UpdateList();
				CloseServiceHandle(hSvc);
				CloseServiceHandle(hSCM);
				return;
			}
		}
	}
	else
	{
		MessageBox(L"暂未提供服务", 0, 0);
	}
		

	CloseServiceHandle(hSvc);
	CloseServiceHandle(hSCM);
	return;
}


void CServiceDlg::Onstopservice()
{
	// TODO: 在此添加命令处理程序代码
	int index = (int)m_list.GetFirstSelectedItemPosition() - 1;
	CString serviceName = m_list.GetItemText(index, 0);
	// 打开服务管理对象
	SC_HANDLE hSCM = OpenSCManagerW(NULL, NULL, SC_MANAGER_ALL_ACCESS);
	// 打开服务。
	SC_HANDLE hSvc = OpenService(hSCM, serviceName, SERVICE_START | SERVICE_QUERY_STATUS | SERVICE_STOP);
	// 获得服务的状态
	SERVICE_STATUS status;
	QueryServiceStatus(hSvc, &status);
	if (status.dwCurrentState == SERVICE_STOPPED)
	{
		MessageBox(L"原本就是停止状态");
	}
	else if (status.dwCurrentState == SERVICE_RUNNING)
	{
		// 停止服务
		ControlService(hSvc, SERVICE_CONTROL_STOP, &status);
		// 等待服务停止
		while (QueryServiceStatus(hSvc, &status) == TRUE)
		{
			Sleep(status.dwWaitHint);
			if (status.dwCurrentState == SERVICE_STOPPED)
			{
				MessageBox(L"关闭成功", 0, 0);
				//m_list.SetItemText(index, 0, L"STOPPED");
				m_list.DeleteAllItems();
				UpdateList();
				CloseServiceHandle(hSvc);
				CloseServiceHandle(hSCM);
				return;
			}
		}
	}
	else
	{
		MessageBox(L"暂未提供服务", 0, 0);
	}

	CloseServiceHandle(hSvc);
	CloseServiceHandle(hSCM);
	return;
}
