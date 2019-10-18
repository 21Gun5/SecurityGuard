// CWindowDlg.cpp: 实现文件
//

#include "stdafx.h"
#include "SecurityGuard.h"
#include "CWindowDlg.h"
#include "afxdialogex.h"


// CWindowDlg 对话框

IMPLEMENT_DYNAMIC(CWindowDlg, CDialogEx)

CWindowDlg::CWindowDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_CWindowDlg, pParent)
{

}

CWindowDlg::~CWindowDlg()
{
}

void CWindowDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_list);
}


BEGIN_MESSAGE_MAP(CWindowDlg, CDialogEx)
END_MESSAGE_MAP()


// CWindowDlg 消息处理程序


BOOL CWindowDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化


	// 初始化图标列表
	//m_imgList.Create(16, 16, ILC_MASK | ILC_COLOR16, 0, 1);
	// 初始化列表控件
	m_list.InsertColumn(0, L"窗口名称", 0, 500);
	m_list.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_SUBITEMIMAGES);
	//m_list.SetImageList(&m_imgList, LVSIL_SMALL);// 设置图标列表
	// 查找窗口
	std::list<HWND> wndList;
	getWindows(&wndList);
	// 窗口信息插入列表
	CWnd* pWnd = NULL;
	CString text;
	int imgIndex = 0, i = 0;
	for (auto&wnd : wndList)
	{
		imgIndex = -1;
		pWnd = FromHandle(wnd);
		pWnd->GetWindowText(text);// 获取窗口标题
		if (pWnd->IsWindowVisible() && !text.IsEmpty())
		{
			// 获取窗口标题
			HICON hIcon = (HICON)::GetClassLong(wnd, -14);//GCL_HICON报错，故-14
			// 获取图标
			if (hIcon)
			{
				//imgIndex = m_imgList.Add(hIcon);
			}
			// 插入一行
			m_list.InsertItem(i, text, imgIndex);
		}
	}

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}
