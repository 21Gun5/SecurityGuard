// CThreadDlg.cpp: 实现文件
//

#include "stdafx.h"
#include "SecurityGuard.h"
#include "CThreadDlg.h"
#include "afxdialogex.h"


// CThreadDlg 对话框

IMPLEMENT_DYNAMIC(CThreadDlg, CDialogEx)

CThreadDlg::CThreadDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_CThreadDlg, pParent)
{

}

CThreadDlg::~CThreadDlg()
{
}

void CThreadDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_list);
}


BEGIN_MESSAGE_MAP(CThreadDlg, CDialogEx)
END_MESSAGE_MAP()


// CThreadDlg 消息处理程序


BOOL CThreadDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化

	// 初始化列表控件（设置扩展风格/插入列
	m_list.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	m_list.InsertColumn(0, L"Index", 0, 100);
	m_list.InsertColumn(1, L"ID", 0, 150);
	m_list.InsertColumn(2, L"OwnerPID", 0, 150);
	// 获取进程的所有模块
	std::vector<THREADENTRY32> threadList;
	GetProcessAllThread(m_pID, &threadList);
	// 插入到列表（插入行、设置内容
	int index = 0;
	CString buffer;
	for (auto&i : threadList) 
	{
		m_list.InsertItem(index, L"");

		buffer.Format(L"%d", index);
		m_list.SetItemText(index, 0, buffer);
		buffer.Format(L"%d", i.th32ThreadID);
		m_list.SetItemText(index, 1, buffer);
		buffer.Format(L"%d", i.th32OwnerProcessID);
		m_list.SetItemText(index, 2, buffer);
		index++;
	}
	// 设置窗口标题
	SetWindowText(m_pName);


	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}


void CThreadDlg::SetProcessID(DWORD pID)
{
	// TODO: 在此处添加实现代码.
	m_pID = pID;
}


void CThreadDlg::SetProcessName(CString pName)
{
	// TODO: 在此处添加实现代码.
	m_pName = pName;
}