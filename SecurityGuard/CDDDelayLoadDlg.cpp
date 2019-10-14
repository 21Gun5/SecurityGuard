// CDDDelayLoadDlg.cpp: 实现文件
//

#include "stdafx.h"
#include "SecurityGuard.h"
#include "CDDDelayLoadDlg.h"
#include "afxdialogex.h"


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
}


BEGIN_MESSAGE_MAP(CDDDelayLoadDlg, CDialogEx)
END_MESSAGE_MAP()


// CDDDelayLoadDlg 消息处理程序
