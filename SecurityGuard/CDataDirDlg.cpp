// CDataDirDlg.cpp: 实现文件
//

#include "stdafx.h"
#include "SecurityGuard.h"
#include "CDataDirDlg.h"
#include "afxdialogex.h"
#include "CPe.h"
#include "CDDExportDlg.h"
#include "CDDImportDlg.h"
#include "CDDResDlg.h"
#include "CDDRelocDlg.h"
#include "CDDTLSDlg.h"
#include "CDDDelayLoadDlg.h"


// CDataDirDlg 对话框

IMPLEMENT_DYNAMIC(CDataDirDlg, CDialogEx)

CDataDirDlg::CDataDirDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_CDatadirDlg, pParent)
{

}

CDataDirDlg::~CDataDirDlg()
{
}

void CDataDirDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_list);
}


BEGIN_MESSAGE_MAP(CDataDirDlg, CDialogEx)
	ON_NOTIFY(NM_RCLICK, IDC_LIST1, &CDataDirDlg::OnRclickList1)
	ON_COMMAND(ID_32773, &CDataDirDlg::OnMenuDetailInfo)
END_MESSAGE_MAP()


// CDataDirDlg 消息处理程序


BOOL CDataDirDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	m_menu.LoadMenu(IDR_MENU2);
	// 设置list扩展风格
	m_list.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	// 插入列
	m_list.InsertColumn(0, L"名称", 0, 250);
	m_list.InsertColumn(1, L"RVA", 0, 250);
	m_list.InsertColumn(2, L"Size", 0, 250);
	// 获取pe信息
	CPe pe;
	//bool isPe = pe.InitPe((TCHAR*)L"01PE文件.exe");
	bool isPe = pe.InitPe(PE_PATH);
	
	PIMAGE_NT_HEADERS pNt = pe.GetNtHeader();
	int index = 0;
	// 各种表的名称
	CString tableName[16] = {
		L"导出",L"导入",L"资源",L"异常" ,
		L"安全",L"重定位",L"调试信息",L"版权信息",
		L"全局指针",L"TLS",L"载入配置",L"输入范围" ,
		L"IAT",L"延迟加载",L"COM",L"保留" };
	// 循环插入
	for (int i = 0; i < 16; i++)
	{
		CString  buffer;
		m_list.InsertItem(index, _T(""));// 插入行
		// 设置单元格内容
		buffer = tableName[i];
		m_list.SetItemText(index, 0, buffer);
		buffer.Format(_T("%08X"), pNt->OptionalHeader.DataDirectory[i].VirtualAddress);
		m_list.SetItemText(index, 1, buffer);
		buffer.Format(_T("%08X"), pNt->OptionalHeader.DataDirectory[i].Size);
		m_list.SetItemText(index, 2, buffer);
		index++;
	}



	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}


void CDataDirDlg::OnRclickList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码

	*pResult = 0;
	CMenu * pSubMenu = m_menu.GetSubMenu(0);
	CPoint pos;
	GetCursorPos(&pos);
	pSubMenu->TrackPopupMenu(0, pos.x, pos.y, this);
}


void CDataDirDlg::OnMenuDetailInfo()
{
	// TODO: 在此添加命令处理程序代码

	// 获取下标（通过光标选择序号，序号从1开始，故-1
	int index = (int)m_list.GetFirstSelectedItemPosition() - 1;
	// 未开发
	if (!(index == 0 || index == 1 || index == 2 ||
		index == 5 || index == 9 || index == 13))
	{
		MessageBox(L"敬请期待", L"Tips", 0);
		return;
	}
	// 本表不存在
	if (m_list.GetItemText(index, 2) == L"00000000")
	{
		MessageBox(L"本表不存在", L"Tips", 0);
		return;
	}
	// 调用相应对话框
	switch (index)
	{
	case 0:
	{
		CDDExportDlg dlg;
		dlg.DoModal();
		break;
	}
	case 1:
	{
		CDDImportDlg dlg;
		dlg.DoModal();
		break;
	}
	case 2:
	{
		CDDResDlg dlg;
		//CDDResorcDlg dlg;
		dlg.DoModal();
		break;
	}
	case 5:
	{
		CDDRelocDlg dlg;
		dlg.DoModal();
		break;
	}
	case 9:
	{
		CDDTLSDlg dlg;
		dlg.DoModal();
		break;
	}
	case 13:
	{
		CDDDelayLoadDlg dlg;
		dlg.DoModal();
		break;
	}
	default:
		break;
	}
}
