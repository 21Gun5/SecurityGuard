// CDDRelocDlg.cpp: 实现文件
//

#include "stdafx.h"
#include "SecurityGuard.h"
#include "CDDRelocDlg.h"
#include "afxdialogex.h"
#include "CPe.h"
#include "CPeInfoDlg.h"


// CDDRelocDlg 对话框

IMPLEMENT_DYNAMIC(CDDRelocDlg, CDialogEx)

CDDRelocDlg::CDDRelocDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_CDDRelocDlg, pParent)
{

}

CDDRelocDlg::~CDDRelocDlg()
{
}

void CDDRelocDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_SECTION, m_listSection);
	DDX_Control(pDX, IDC_LIST_BLOCK, m_listBlock);
}


BEGIN_MESSAGE_MAP(CDDRelocDlg, CDialogEx)
	ON_NOTIFY(NM_CLICK, IDC_LIST_SECTION, &CDDRelocDlg::OnClickListSection)
END_MESSAGE_MAP()


// CDDRelocDlg 消息处理程序





BOOL CDDRelocDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	// 设置list扩展风格
	m_listSection.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	m_listBlock.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	// dll 列表插入列
	m_listSection.InsertColumn(0, L"Index", 0, 250);
	m_listSection.InsertColumn(1, L"RVA", 0, 250);
	m_listSection.InsertColumn(2, L"Items", 0, 250);
	// func 列表插入列

	m_listBlock.InsertColumn(0, L"Index", 0, 250);
	m_listBlock.InsertColumn(1, L"RVA", 0, 250);
	m_listBlock.InsertColumn(2, L"FOA", 0, 250);

	// 获取pe信息
	CPe pe;
	//pe.InitPe((TCHAR*)PE_PATH);
	bool isPe = pe.InitPe((TCHAR*)g_PePath);
	//pe.ShowRelocalInfo();

	PIMAGE_BASE_RELOCATION pRelocal = pe.GetRelocalDirectory();
	// 遍历所有的重定位数据
	int index = 0;
	while (pRelocal->SizeOfBlock)
	{
		CString  buffer;
		m_listSection.InsertItem(index, _T(""));// 插入行
		//设置单元格内容
		buffer.Format(_T("%d"), index + 1);
		m_listSection.SetItemText(index, 0, buffer);
		buffer.Format(_T("%08X"), pRelocal->VirtualAddress);
		m_listSection.SetItemText(index, 1, buffer);
		DWORD dwCount = (pRelocal->SizeOfBlock - 8) / 2;
		buffer.Format(_T("%d"), dwCount);
		m_listSection.SetItemText(index, 2, buffer);
		index++;
		//pRelocal += pRelocal->SizeOfBlock;
		pRelocal = (PIMAGE_BASE_RELOCATION)((DWORD)pRelocal + pRelocal->SizeOfBlock);
	}
	
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}


void CDDRelocDlg::OnClickListSection(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;

	typedef	struct {
		WORD Offset : 12;  // (1) 大小为12Bit的重定位偏移
		WORD Type : 4;   // (2) 大小为4Bit的重定位信息类型值
	}*PTYPEOFFSET;	    // 这个结构体是A1Pass总结的
	//MessageBox(0, 0, 0);

	m_listBlock.DeleteAllItems();
	int j = (int)m_listSection.GetFirstSelectedItemPosition() - 1;
	DWORD dwCount  = _ttoi(m_listSection.GetItemText(j, 2));
	//DWORD RvaTmp = _ttoi(m_listSection.GetItemText(j, 0));


	// 获取pe信息
	CPe pe;
	bool isPe = pe.InitPe((TCHAR*)g_PePath);
	PIMAGE_BASE_RELOCATION pRelocal = pe.GetRelocalDirectory();
	// 遍历所有的重定位数据
	while (pRelocal->SizeOfBlock)
	{
		
		// 如果不是所点击的，则跳过
		//DWORD Rva = pRelocal->VirtualAddress;
		//if(Rva != RvaTmp)
		//	continue;
		DWORD dwCount2 = (pRelocal->SizeOfBlock - 8) / 2;
		if (dwCount != dwCount2)
		{
			pRelocal = (PIMAGE_BASE_RELOCATION)((DWORD)pRelocal + pRelocal->SizeOfBlock);
			continue;
		}
			

		PTYPEOFFSET pRelocalBlock = (PTYPEOFFSET)(pRelocal + 1);
		int index = 0;
		for (int i = 0; i < dwCount; i++)
		{
			CString buffer;
			m_listBlock.InsertItem(index, _T(""));
			if (pRelocalBlock[i].Type == 3)
			{
				// 要修复数据的RVA
				buffer.Format(_T("%d"), index+1);
				m_listBlock.SetItemText(index, 0, buffer);
				DWORD Rva = pRelocalBlock[i].Offset + pRelocal->VirtualAddress;
				buffer.Format(_T("%08X"), Rva);
				m_listBlock.SetItemText(index, 1, buffer);
				buffer.Format(_T("%08X"), pe.RvaToFoa(Rva));
				m_listBlock.SetItemText(index, 2, buffer);
			}
			else
			{
				buffer.Format(_T("%d"), index + 1);
				m_listBlock.SetItemText(index, 0, buffer);
				m_listBlock.SetItemText(index, 1, L"type != 3");
				m_listBlock.SetItemText(index, 2, L"不需重定位");
			}
			index++;
		}
		return;
		//pRelocal = (PIMAGE_BASE_RELOCATION)((DWORD)pRelocal + pRelocal->SizeOfBlock);
	}
}
