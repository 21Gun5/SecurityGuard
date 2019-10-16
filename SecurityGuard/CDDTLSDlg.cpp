// CDDTLSDlg.cpp: 实现文件
//

#include "stdafx.h"
#include "SecurityGuard.h"
#include "CDDTLSDlg.h"
#include "afxdialogex.h"
#include "CPe.h"

// CDDTLSDlg 对话框

IMPLEMENT_DYNAMIC(CDDTLSDlg, CDialogEx)

CDDTLSDlg::CDDTLSDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_CDDTLSDlg, pParent)
{

}

CDDTLSDlg::~CDDTLSDlg()
{
}

void CDDTLSDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_list);
}


BEGIN_MESSAGE_MAP(CDDTLSDlg, CDialogEx)
END_MESSAGE_MAP()


// CDDTLSDlg 消息处理程序


BOOL CDDTLSDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	m_list.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	m_list.InsertColumn(0, L"StartAddress", 0, 150);
	m_list.InsertColumn(1, L"EndAddress", 0, 150);
	m_list.InsertColumn(2, L"AddressOfIndex", 0, 250);
	m_list.InsertColumn(3, L"AddressOfCallBacks", 0, 250);

	// 获取pe信息
	CPe pe;
	bool isPe = pe.InitPe(PE_PATH);

	PIMAGE_TLS_DIRECTORY32 pTLS = pe.GetTLSDirectory();
	// 循环设置dll列表
	int index = 0;
	while (pTLS->StartAddressOfRawData)
	{
		CString  buffer;
		m_list.InsertItem(index, _T(""));// 插入行
		// 设置单元格内容
		buffer.Format(_T("%08X"), pTLS->StartAddressOfRawData);
		m_list.SetItemText(index, 0, buffer);
		buffer.Format(_T("%08X"), pTLS->EndAddressOfRawData);
		m_list.SetItemText(index, 1, buffer);
		buffer.Format(_T("%08X"), pTLS->AddressOfIndex);
		m_list.SetItemText(index, 2, buffer);
		buffer.Format(_T("%08X"), pTLS->AddressOfCallBacks);
		m_list.SetItemText(index, 3, buffer);
		index++;
		// 下一个导入表
		pTLS++;
	}



	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}
