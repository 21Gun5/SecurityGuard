// CExportDlg.cpp: 实现文件
//

#include "stdafx.h"
#include "SecurityGuard.h"
#include "CDDExportDlg.h"
#include "afxdialogex.h"
#include "CPe.h"


// CExportDlg 对话框

IMPLEMENT_DYNAMIC(CDDExportDlg, CDialogEx)

CDDExportDlg::CDDExportDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_CDDExportDlg, pParent)
	, m_editName(_T(""))
	, m_editBase(_T(""))
	, m_editFuncNum(_T(""))
	, m_editNameNum(_T(""))
	, m_editNameAddr(_T(""))
	, m_editEAT(_T(""))
	, m_editENT(_T(""))
	, m_editEOT(_T(""))
{

}

CDDExportDlg::~CDDExportDlg()
{
}

void CDDExportDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_editName);
	DDX_Text(pDX, IDC_EDIT12, m_editBase);
	DDX_Text(pDX, IDC_EDIT2, m_editFuncNum);
	DDX_Text(pDX, IDC_EDIT3, m_editNameNum);
	DDX_Text(pDX, IDC_EDIT4, m_editNameAddr);
	DDX_Text(pDX, IDC_EDIT13, m_editEAT);
	DDX_Text(pDX, IDC_EDIT6, m_editENT);
	DDX_Text(pDX, IDC_EDIT7, m_editEOT);
	DDX_Control(pDX, IDC_LIST1, m_list);
}


BEGIN_MESSAGE_MAP(CDDExportDlg, CDialogEx)
END_MESSAGE_MAP()


// CExportDlg 消息处理程序


BOOL CDDExportDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化

	// 设置list扩展风格
	m_list.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	// 插入列
	m_list.InsertColumn(0, L"序号", 0, 250);
	m_list.InsertColumn(1, L"函数名", 0, 250);
	m_list.InsertColumn(2, L"函数地址", 0, 250);
	// 获取导出表信息
	CPe pe;
	bool isPe = pe.InitPe(PE_PATH);
	PIMAGE_EXPORT_DIRECTORY pExport = pe.GetExportDirectory();
	// 填充基本信息（编辑框中
	UpdateData(TRUE);
	char * pDllName = (char*)(pe.RvaToFoa(pExport->Name) + g_PeBuff);
	m_editName.Format(_T("%S"), pDllName);
	m_editBase.Format(_T("%08X"), pExport->Base);
	m_editFuncNum.Format(_T("%08X"), pExport->NumberOfFunctions);
	m_editNameNum.Format(_T("%08X"), pExport->NumberOfNames);
	m_editNameAddr.Format(_T("%08X"), pExport->Name);
	m_editEAT.Format(_T("%08X"), pExport->AddressOfFunctions);
	m_editENT.Format(_T("%08X"), pExport->AddressOfNames);
	m_editEOT.Format(_T("%08X"), pExport->AddressOfNameOrdinals);
	UpdateData(FALSE);


	// 导出名称表、序号表
	DWORD * pEAT = (DWORD*)(pe.RvaToFoa(pExport->AddressOfFunctions) + g_PeBuff);
	DWORD * pENT = (DWORD*)(pe.RvaToFoa(pExport->AddressOfNames) + g_PeBuff);
	WORD * pEOT = (WORD*)(pe.RvaToFoa(pExport->AddressOfNameOrdinals) + g_PeBuff);

	// 遍历地址表
	int index = 0;
	for (int i = 0; i < pExport->NumberOfFunctions; i++)
	{

		CString  buffer;
		m_list.InsertItem(index, _T(""));// 插入行
		// 序号
		buffer.Format(_T("%08X"), i + pExport->Base);
		m_list.SetItemText(index, 0, buffer);
		for (int j = 0; j < pExport->NumberOfNames; j++)// 遍历序号表
		{
			// 函数名
			if (i == pEOT[j]) 
			{
				// 输出这个函数名字
				char * pFunName =(char *)(pe.RvaToFoa(pENT[j]) + g_PeBuff);
				buffer = pFunName;
				m_list.SetItemText(index, 1, buffer);
				break;
			}
			else
			{
				m_list.SetItemText(index, 1, L"-");
			}
		}
		// 地址
		buffer.Format(_T("%08X"), *(DWORD*)(pe.RvaToFoa(pEAT[i]) + (DWORD)g_PeBuff));
		m_list.SetItemText(index, 2, buffer);
		index++;
	}




	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}


//CString  buffer;
//m_list.InsertItem(index, _T(""));// 插入行
//// 设置单元格内容
//buffer = tableName[i];
//m_list.SetItemText(index, 0, buffer);
//buffer.Format(_T("%08X"), pNt->OptionalHeader.DataDirectory[i].VirtualAddress);
//m_list.SetItemText(index, 1, buffer);
//buffer.Format(_T("%08X"), pNt->OptionalHeader.DataDirectory[i].Size);
//m_list.SetItemText(index, 2, buffer);
//index++;