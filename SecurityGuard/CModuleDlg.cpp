// CModuleDlg.cpp: 实现文件
//

#include "stdafx.h"
#include "SecurityGuard.h"
#include "CModuleDlg.h"
#include "afxdialogex.h"


// CModuleDlg 对话框

IMPLEMENT_DYNAMIC(CModuleDlg, CDialogEx)

CModuleDlg::CModuleDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_CModuleDlg, pParent)
{

}

CModuleDlg::~CModuleDlg()
{
}

void CModuleDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_list);
}


BEGIN_MESSAGE_MAP(CModuleDlg, CDialogEx)
END_MESSAGE_MAP()


// CModuleDlg 消息处理程序


BOOL CModuleDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化

	// 初始化列表控件（设置扩展风格/插入列
	m_list.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	m_list.InsertColumn(0, L"模块", 0, 160);
	m_list.InsertColumn(1, L"基址", 0, 120);
	m_list.InsertColumn(2, L"大小", 0, 120);
	m_list.InsertColumn(3, L"路径", 0, 480);
	// 获取进程的所有模块
	std::vector<MODULEENTRY32> moduleList;
	getProcessAllModule(m_pID, &moduleList);
	// 插入到列表（插入行、设置内容
	int index = 0;
	CString buffer;
	for (auto&i : moduleList) 
	{
		m_list.InsertItem(index, i.szModule);
		buffer.Format(L"%08X", i.modBaseAddr);
		m_list.SetItemText(index, 1, buffer);
		buffer.Format(L"%08X", i.modBaseSize);
		m_list.SetItemText(index, 2, buffer);
		m_list.SetItemText(index, 3, i.szExePath);
	}
	// 设置窗口标题
	SetWindowText(m_pName);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}


void CModuleDlg::SetProcessID(DWORD pID)
{
	// TODO: 在此处添加实现代码.
	m_pID = pID;
}


void CModuleDlg::SetProcessName(CString pName)
{
	// TODO: 在此处添加实现代码.
	m_pName = pName;
}