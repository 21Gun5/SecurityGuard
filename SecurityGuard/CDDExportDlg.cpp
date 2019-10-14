// CExportDlg.cpp: 实现文件
//

#include "stdafx.h"
#include "SecurityGuard.h"
#include "CDDExportDlg.h"
#include "afxdialogex.h"


// CExportDlg 对话框

IMPLEMENT_DYNAMIC(CDDExportDlg, CDialogEx)

CDDExportDlg::CDDExportDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_CDDExportDlg, pParent)
{

}

CDDExportDlg::~CDDExportDlg()
{
}

void CDDExportDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDDExportDlg, CDialogEx)
END_MESSAGE_MAP()


// CExportDlg 消息处理程序


BOOL CDDExportDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化




	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}
