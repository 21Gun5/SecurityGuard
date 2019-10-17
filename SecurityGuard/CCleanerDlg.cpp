// CCleanerDlg.cpp: 实现文件
//

#include "stdafx.h"
#include "SecurityGuard.h"
#include "CCleanerDlg.h"
#include "afxdialogex.h"
#include "CTools.h"


// CCleanerDlg 对话框

#define MY_MSG_SCANFILE (WM_USER+100)

IMPLEMENT_DYNAMIC(CCleanerDlg, CDialogEx)

CCleanerDlg::CCleanerDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_CCleanerDlg, pParent)
	, m_editDir(_T(""))
	, m_editFilter(_T(""))
{

}

CCleanerDlg::~CCleanerDlg()
{
}

void CCleanerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_DIR, m_editDir);
	DDX_Text(pDX, IDC_EDIT_FILTER, m_editFilter);
	DDX_Control(pDX, IDC_LIST1, m_list);
}


BEGIN_MESSAGE_MAP(CCleanerDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_CHOOSEDIR, &CCleanerDlg::OnBnClickedButtonChoosedir)
	ON_BN_CLICKED(IDC_BUTTON_SCAN, &CCleanerDlg::OnBnClickedButtonScan)
	ON_MESSAGE(MY_MSG_SCANFILE, &CCleanerDlg::OnScanFile)

	ON_NOTIFY(NM_RCLICK, IDC_LIST1, &CCleanerDlg::OnRclickList1)
	ON_COMMAND(ID_32775, &CCleanerDlg::OnFileSelectall)
	ON_COMMAND(ID_32776, &CCleanerDlg::OnFileUnselectAll)
	ON_COMMAND(ID_32777, &CCleanerDlg::OnFileDeleteall)
	ON_COMMAND(ID_32778, &CCleanerDlg::OnFileDeleteone)
	ON_WM_DROPFILES()
	ON_BN_CLICKED(IDC_BUTTON_RBINFO, &CCleanerDlg::OnBnClickedButtonRbinfo)
	ON_BN_CLICKED(IDC_BUTTON_CLEANRB, &CCleanerDlg::OnBnClickedButtonCleanrb)
END_MESSAGE_MAP()


// CCleanerDlg 消息处理程序

BOOL CCleanerDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化

	m_list.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES | LVS_EX_CHECKBOXES);
	m_list.InsertColumn(0, L"Path", 0, 500);
	m_list.InsertColumn(1, L"Size", 0, 100);

	UpdateData(TRUE);
	m_editFilter = L".ilk .pdb .obj .log .pch .tlog .lastbuildstate .sdf .idb .ipch .res .o .lst .knl .img .bin .db";
	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}

void CCleanerDlg::OnBnClickedButtonChoosedir()
{
	// TODO: 在此添加控件通知处理程序代码
	TCHAR szFolderPath[MAX_PATH] = { 0 };
	BROWSEINFO	sInfo;
	::ZeroMemory(&sInfo, sizeof(BROWSEINFO));
	sInfo.pidlRoot = 0;
	sInfo.lpszTitle = _T("请选择一个文件夹：");
	sInfo.ulFlags = BIF_DONTGOBELOWDOMAIN | BIF_RETURNONLYFSDIRS | BIF_NEWDIALOGSTYLE | BIF_EDITBOX | BIF_USENEWUI | BIF_BROWSEINCLUDEFILES;
	sInfo.lpfn = NULL;
	// 显示文件夹选择对话框
	LPITEMIDLIST lpidlBrowse = ::SHBrowseForFolder(&sInfo);
	if (lpidlBrowse != NULL)
	{
		if (::SHGetPathFromIDList(lpidlBrowse, szFolderPath))
		{
			UpdateData(TRUE);
			m_editDir = szFolderPath;
			UpdateData(FALSE);
		}
	}
	return;
}

DWORD CALLBACK ScanFileProc(VOID* arg)
{
	// 在创建线程时:
	//  CreateThread(0, 0, ScanFileProc, this, 0, 0);
	//  已将this指针通过附加参数传递过来了, 因此,可以将
	//  参数强转成this指针来使用.
	CCleanerDlg* _this = (CCleanerDlg*)arg;
	findFiles(_this->m_editDir,
		_this->m_editFilter,
		NULL,
		_this->m_hWnd,/*接收消息的窗口*/
		MY_MSG_SCANFILE /*接收的消息,这个宏是自定义的,
						该消息会在CVsCleanDlg类中有一
						个消息响应函数:OnScanFile,这
						个函数在消息映射中和
						MY_MSG_SCANFILE绑定*/);
	_this->MessageBox(L"扫描完毕");
	//_this->m_isScanBegin = false;
	return 0;
}

void CCleanerDlg::OnBnClickedButtonScan()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_editDir.IsEmpty())
	{
		MessageBox(L"请选择目录", L"Tips", 0);
		return;
	}
	m_list.DeleteAllItems();
	m_totalSize = 0;

	// 遍历文件, 添加到列表控件
	// 开启线程扫描垃圾文件
	m_hScanThread = CreateThread(0, 0, ScanFileProc, this, 0, 0);
}

LRESULT CCleanerDlg::OnScanFile(WPARAM w, LPARAM l)
{
	FILEINFO* fInfo = (FILEINFO*)w;
	m_totalSize += (fInfo->nFileSizeHigh << 32 | fInfo->nFileSizeLow);
	int row = m_list.GetItemCount();
	m_list.InsertItem(row, fInfo->path);
	CString buffer;
	buffer.Format(_T("%dKb"), (fInfo->nFileSizeHigh << 32 | fInfo->nFileSizeLow) / 1024);
	m_list.SetItemText(row, 1, buffer);
	//滚动到最后一行
	m_list.Scroll(CSize(0, 100000));
	return 1;
}





void CCleanerDlg::OnRclickList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;

	CMenu menu;
	menu.LoadMenu(IDR_MENU3);
	CMenu* pSubMenu = menu.GetSubMenu(0);
	CPoint pos;
	GetCursorPos(&pos);
	pSubMenu->TrackPopupMenu(0, pos.x, pos.y, this);
}


void CCleanerDlg::OnFileSelectall()
{
	// TODO: 在此添加命令处理程序代码
	int size = m_list.GetItemCount();
	for (int i = 0; i < size; ++i) 
	{
		m_list.SetCheck(i, TRUE);
	}
}


void CCleanerDlg::OnFileUnselectAll()
{
	// TODO: 在此添加命令处理程序代码
	int size = m_list.GetItemCount();
	for (int i = 0; i < size; ++i) {
		m_list.SetCheck(i, !m_list.GetCheck(i));
	}
}


void CCleanerDlg::OnFileDeleteall()
{
	// TODO: 在此添加命令处理程序代码
	for (int i = 0; i < m_list.GetItemCount(); ) {
		if (m_list.GetCheck(i)) {

			CString path = m_list.GetItemText(i, 0);
			OutputDebugString(L"删除文件: " + path);
			// 从磁盘上删除文件
			if (DeleteFile(path)) {
				m_list.DeleteItem(i);
				continue;
			}

			m_list.DeleteItem(i);
			continue;
		}
		++i;
	}
}


void CCleanerDlg::OnFileDeleteone()
{
	// TODO: 在此添加命令处理程序代码
	POSITION pos = m_list.GetFirstSelectedItemPosition();
	if (pos == NULL) {
		return;
	}
	int nPos = (int)pos - 1;
	CString path = m_list.GetItemText(nPos, 0);
	OutputDebugString(L"删除文件: " + path);
	// 从磁盘上删除文件
	if (DeleteFile(path))
	{
		m_list.DeleteItem(nPos);
	}
	//m_list.DeleteItem(nPos);
}


void CCleanerDlg::OnBnClickedButtonRbinfo()
{
	// TODO: 在此添加控件通知处理程序代码
	
	// 初始化回收站结构体
	SHQUERYRBINFO RecycleBinInfo = { };
	RecycleBinInfo.cbSize = sizeof(RecycleBinInfo);
	// 查询信息
	SHQueryRecycleBin(NULL, &RecycleBinInfo);
	CString buff;
	buff.Format(L"大小:%lldKB\n数量:%lld个", RecycleBinInfo.i64Size, RecycleBinInfo.i64NumItems);
	MessageBox(buff);
}


void CCleanerDlg::OnBnClickedButtonCleanrb()
{
	// TODO: 在此添加控件通知处理程序代码
	// 初始化回收站结构体
	SHQUERYRBINFO RecycleBinInfo = { };
	RecycleBinInfo.cbSize = sizeof(RecycleBinInfo);
	//清空回收站
	SHEmptyRecycleBin(NULL, NULL, NULL);
}
