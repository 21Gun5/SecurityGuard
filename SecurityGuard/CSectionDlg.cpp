// CSectionDlg.cpp: 实现文件
//

#include "stdafx.h"
#include "SecurityGuard.h"
#include "CSectionDlg.h"
#include "afxdialogex.h"
#include "CPe.h"


// CSectionDlg 对话框

IMPLEMENT_DYNAMIC(CSectionDlg, CDialogEx)

CSectionDlg::CSectionDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_CSectionDlg, pParent)
{

}

CSectionDlg::~CSectionDlg()
{
}

void CSectionDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_list);
}


BEGIN_MESSAGE_MAP(CSectionDlg, CDialogEx)
END_MESSAGE_MAP()


// CSectionDlg 消息处理程序


BOOL CSectionDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化

	// 设置list扩展风格
	m_list.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	// 插入列
	m_list.InsertColumn(0, L"名称", 0, 150);
	m_list.InsertColumn(1, L"VOffset", 0, 150);
	m_list.InsertColumn(2, L"VSize", 0, 150);
	m_list.InsertColumn(3, L"ROffset", 0, 150);
	m_list.InsertColumn(4, L"RSize", 0, 150);
	m_list.InsertColumn(5, L"标记", 0, 150);
	// 获取pe信息
	CPe pe;
	//bool isPe = pe.InitPe((TCHAR*)L"01PE文件.exe");
	bool isPe = pe.InitPe(PE_PATH);
	PIMAGE_NT_HEADERS pNt = pe.GetNtHeader();
	PIMAGE_SECTION_HEADER pSection = IMAGE_FIRST_SECTION(pNt);
	int index = 0;
	// 循环插入
	for (int i = 0; i < pNt->FileHeader.NumberOfSections; i++)
	{
		CString  buffer;
		m_list.InsertItem(index, _T(""));// 插入行

		buffer.Format(_T("%S"), pSection[i].Name);
		m_list.SetItemText(index, 0, buffer);
		buffer.Format(_T("%08X"), pSection[i].VirtualAddress);
		m_list.SetItemText(index, 1, buffer);
		buffer.Format(_T("%08X"), pSection[i].Misc);
		m_list.SetItemText(index, 2, buffer);
		buffer.Format(_T("%08X"), pSection[i].PointerToRawData);
		m_list.SetItemText(index, 3, buffer);
		buffer.Format(_T("%08X"), pSection[i].SizeOfRawData);
		m_list.SetItemText(index, 4, buffer);
		buffer.Format(_T("%08X"), pSection[i].Characteristics);
		m_list.SetItemText(index, 5, buffer);
		index++;
	}

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}



