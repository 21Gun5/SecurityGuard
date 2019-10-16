// CDDDelayLoadDlg.cpp: 实现文件
//

#include "stdafx.h"
#include "SecurityGuard.h"
#include "CDDDelayLoadDlg.h"
#include "afxdialogex.h"
#include "CPe.h"


// CDDDelayLoadDlg 对话框

IMPLEMENT_DYNAMIC(CDDDelayLoadDlg, CDialogEx)

CDDDelayLoadDlg::CDDDelayLoadDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_CDDDelayLoadDlg, pParent)
{

}

CDDDelayLoadDlg::~CDDDelayLoadDlg()
{
}

void CDDDelayLoadDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_list);
}


BEGIN_MESSAGE_MAP(CDDDelayLoadDlg, CDialogEx)
END_MESSAGE_MAP()


// CDDDelayLoadDlg 消息处理程序


BOOL CDDDelayLoadDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化

	m_list.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	m_list.InsertColumn(0, L"Dll名称", 0, 150);
	m_list.InsertColumn(1, L"模块句柄", 0, 150);
	m_list.InsertColumn(2, L"IAT", 0, 150);
	m_list.InsertColumn(3, L"INT", 0, 150);
	m_list.InsertColumn(4, L"BoundIAT", 0, 150);

	// 获取pe信息
	CPe pe;
	bool isPe = pe.InitPe(PE_PATH);

	PIMAGE_DELAYLOAD_DESCRIPTOR pDelayLoad = pe.GetDelayLoadDirectory();
	// 循环设置dll列表
	int index = 0;
	while (pDelayLoad->DllNameRVA)
	{
		CString  buffer;
		m_list.InsertItem(index, _T(""));// 插入行
		// 设置单元格内容
		char * pDllname = (char*)(pe.RvaToFoa(pDelayLoad->DllNameRVA) + (DWORD)g_PeBuff);
		buffer = pDllname;
		m_list.SetItemText(index, 0, buffer);
		buffer.Format(_T("%08X"), pDelayLoad->ModuleHandleRVA);
		m_list.SetItemText(index, 1, buffer);
		buffer.Format(_T("%08X"), pDelayLoad->ImportAddressTableRVA);
		m_list.SetItemText(index, 2, buffer);
		buffer.Format(_T("%08X"), pDelayLoad->ImportNameTableRVA);
		m_list.SetItemText(index, 3, buffer);
		buffer.Format(_T("%08X"), pDelayLoad->BoundImportAddressTableRVA);
		m_list.SetItemText(index, 4, buffer);
		index++;
		// 下一个导入表
		pDelayLoad++;
	}

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}
