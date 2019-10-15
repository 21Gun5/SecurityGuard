// CImportDlg.cpp: 实现文件
//

#include "stdafx.h"
#include "SecurityGuard.h"
#include "CDDImportDlg.h"
#include "afxdialogex.h"
#include "CPe.h"


// CImportDlg 对话框

IMPLEMENT_DYNAMIC(CDDImportDlg, CDialogEx)

CDDImportDlg::CDDImportDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_CDDImportDlg, pParent)
{

}

CDDImportDlg::~CDDImportDlg()
{
}

void CDDImportDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_DLL, m_listDll);
	DDX_Control(pDX, IDC_LIST_FUNC, m_listFunc);
}


BEGIN_MESSAGE_MAP(CDDImportDlg, CDialogEx)
	ON_NOTIFY(NM_CLICK, IDC_LIST_DLL, &CDDImportDlg::OnClickListDll)
END_MESSAGE_MAP()


// CImportDlg 消息处理程序


BOOL CDDImportDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化

	// 设置list扩展风格
	m_listDll.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	m_listFunc.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	// dll 列表插入列
	m_listDll.InsertColumn(0, L"DllName", 0, 250);
	m_listDll.InsertColumn(1, L"OriginalFirstTrunk", 0, 250);
	m_listDll.InsertColumn(2, L"FirstTrunk", 0, 150);
	// func 列表插入列
	m_listFunc.InsertColumn(0, L"DllName", 0, 250);
	m_listFunc.InsertColumn(1, L"序号", 0, 250);
	m_listFunc.InsertColumn(2, L"函数名", 0, 250);
	// 获取pe信息
	CPe pe;
	//bool isPe = pe.InitPe((TCHAR*)L"01PE文件.exe");
	bool isPe = pe.InitPe(PE_PATH);
	PIMAGE_IMPORT_DESCRIPTOR pImport = pe.GetImportDirectory();
	// 循环设置dll列表
	int index = 0;
	while (pImport->Name)
	{
		CString  buffer;
		m_listDll.InsertItem(index, _T(""));// 插入行
		// 设置单元格内容
		char * pDllname = (char*)(pe.RvaToFoa(pImport->Name) + (DWORD)g_PeBuff);
		buffer = pDllname;
		m_listDll.SetItemText(index, 0, buffer);
		buffer.Format(_T("%08X"), pImport->OriginalFirstThunk);
		m_listDll.SetItemText(index, 1, buffer);
		buffer.Format(_T("%08X"), pImport->FirstThunk);
		m_listDll.SetItemText(index, 2, buffer);
		index++;
		// 下一个导入表
		pImport++;
	}
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}

void CDDImportDlg::OnClickListDll(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;

	//
	m_listFunc.DeleteAllItems();

	int i = (int)m_listDll.GetFirstSelectedItemPosition() - 1;
	CString curDllName = m_listDll.GetItemText(i, 0);
	//DWORD curFirstThunk = _ttoi(m_listDll.GetItemText(index, 1));

	CPe pe;
	//bool isPe = pe.InitPe((TCHAR*)L"01PE文件.exe");
	bool isPe = pe.InitPe(PE_PATH);
	//bool isPe = pe.InitPe((TCHAR*)L"02calldll.exe"); 
	PIMAGE_IMPORT_DESCRIPTOR pImport = pe.GetImportDirectory() + i;
	// 获取IAT
	PIMAGE_THUNK_DATA pIAT = (PIMAGE_THUNK_DATA)(pe.RvaToFoa(pImport->OriginalFirstThunk) + (DWORD)g_PeBuff);
	
	// 遍历IAT中函数
	int index = 0;
	while (pIAT->u1.Ordinal)
	{
		CString  buffer;
		m_listFunc.InsertItem(index, _T(""));// 插入行
		// 设置单元格内容
		m_listFunc.SetItemText(index, 0, curDllName);
		// 判断最高位是否位1 ，如果为1那么需要导入
		if (IMAGE_SNAP_BY_ORDINAL(pIAT->u1.Ordinal))
		{
			buffer.Format(_T("%08X"), pIAT->u1.Ordinal & 0x0000FFFF);
			m_listFunc.SetItemText(index, 1, buffer);
			//m_listFunc.SetItemText(index, 2, buffer);
			//m_listFunc.SetItemText(index, 2, L"没有名字");
		}
		else 
		{
			// 导入的函数是名称
			PIMAGE_IMPORT_BY_NAME pName =(PIMAGE_IMPORT_BY_NAME)(pe.RvaToFoa(pIAT->u1.AddressOfData) + (DWORD)g_PeBuff);
			buffer.Format(_T("%08X"), pName->Hint);
			m_listFunc.SetItemText(index, 1, buffer);

			buffer.Format(_T("%S"), pName->Name);
			m_listFunc.SetItemText(index, 2, buffer);
		}
		// 解析下一个函数
		pIAT++;
		index++;
	}
}
