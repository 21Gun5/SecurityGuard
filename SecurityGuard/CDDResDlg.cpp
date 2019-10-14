// CDDResDlg.cpp: 实现文件
//

#include "stdafx.h"
#include "SecurityGuard.h"
#include "CDDResDlg.h"
#include "afxdialogex.h"


// CDDResDlg 对话框

IMPLEMENT_DYNAMIC(CDDResDlg, CDialogEx)

CDDResDlg::CDDResDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_CDDResDlg, pParent)
{

}

CDDResDlg::~CDDResDlg()
{
}

void CDDResDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDDResDlg, CDialogEx)
END_MESSAGE_MAP()


// CDDResDlg 消息处理程序
