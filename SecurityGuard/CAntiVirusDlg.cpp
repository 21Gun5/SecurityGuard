// CAntiVirus.cpp: 实现文件
//

#include "stdafx.h"
#include "SecurityGuard.h"
#include "CAntiVirusDlg.h"
#include "afxdialogex.h"
#include "CTools.h"

#include <locale.h>


// 宽字符转换为多字符(Unicode --> ASCII)
#define  WCHAR_TO_CHAR(lpW_Char, lpChar) \
     WideCharToMultiByte(CP_ACP, NULL, lpW_Char, -1, \
 lpChar, _countof(lpChar), NULL, FALSE);


// 多字符转换为宽字符(ASCII --> Unicode)
#define  CHAR_TO_WCHAR(lpChar, lpW_Char) \
     MultiByteToWideChar(CP_ACP, NULL, lpChar, -1, \
 lpW_Char, _countof(lpW_Char));

// CAntiVirus 对话框

#define MY_MSG_SCANFILE2 (WM_USER+101)

IMPLEMENT_DYNAMIC(CAntiVirusDlg, CDialogEx)

CAntiVirusDlg::CAntiVirusDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_CAntiVirusDlg, pParent)
	, m_editDir(_T(""))

{

}

CAntiVirusDlg::~CAntiVirusDlg()
{
}

void CAntiVirusDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	//  DDX_Control(pDX, IDC_TREE1, m_tree);
	DDX_Text(pDX, IDC_EDIT1, m_editDir);
	DDX_Control(pDX, IDC_LIST1, m_list);
	//  DDX_Text(pDX, IDC_EDIT_TEST, m_editTest);
}


BEGIN_MESSAGE_MAP(CAntiVirusDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_CHOOSEDIR, &CAntiVirusDlg::OnBnClickedButtonChoosedir)
	ON_BN_CLICKED(IDC_BUTTON_SCAN, &CAntiVirusDlg::OnBnClickedButtonScan)
	ON_MESSAGE(MY_MSG_SCANFILE2, &CAntiVirusDlg::OnScanFile)
	ON_NOTIFY(NM_RCLICK, IDC_LIST1, &CAntiVirusDlg::OnRclickList1)
	ON_COMMAND(ID_MD5Anti, &CAntiVirusDlg::OnMd5anti)
END_MESSAGE_MAP()


// CAntiVirus 消息处理程序


BOOL CAntiVirusDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化

	m_list.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES | LVS_EX_CHECKBOXES);
	m_list.InsertColumn(0, L"Index", 0, 100);
	m_list.InsertColumn(1, L"路径", 0, 700);
	//m_list.InsertColumn(1, L"Size", 0, 100);



	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}


//void CAntiVirusDlg::InitFileTree(CString str_Dir, HTREEITEM tree_Root)
//{
//	CFileFind FileFind;
//	//临时变量，用以记录返回的树节点
//	HTREEITEM tree_Temp;
//	//判断输入目录最后是否存在'\'，不存在则补充
//	if (str_Dir.Right(1) != "\\")
//		str_Dir += "\\";
//	str_Dir += "*.*";
//	BOOL res = FileFind.FindFile(str_Dir);
//	while (res)
//	{
//		tree_Temp = tree_Root;
//		res = FileFind.FindNextFileW();
//		if (FileFind.IsDirectory() && !FileFind.IsDots())//目录是文件夹
//		{
//			CString strPath = FileFind.GetFilePath(); //得到路径，做为递归调用的开始
//			CString strTitle = FileFind.GetFileName();//得到目录名，做为树控的结点
//			tree_Temp = m_tree.InsertItem(strTitle, 0, 0, tree_Root);
//			InitFileTree(strPath, tree_Temp);
//		}
//		else if (!FileFind.IsDirectory() && !FileFind.IsDots())//如果是文件
//		{
//			CString strPath = FileFind.GetFilePath(); //得到路径，做为递归调用的开始
//			CString strTitle = FileFind.GetFileName();//得到文件名，做为树控的结点
//			m_tree.InsertItem(strTitle, 0, 0, tree_Temp);
//		}
//	}
//	FileFind.Close();
//}

void CAntiVirusDlg::OnBnClickedButtonChoosedir()
{
	// TODO: 在此添加控件通知处理程序代码

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


DWORD CALLBACK ScanFileProc2(VOID* arg)
{
	// 在创建线程时:
	//  CreateThread(0, 0, ScanFileProc, this, 0, 0);
	//  已将this指针通过附加参数传递过来了, 因此,可以将
	//  参数强转成this指针来使用.
	CAntiVirusDlg* _this = (CAntiVirusDlg*)arg;

	findFiles(_this->m_editDir,
		_this->m_filter,
		NULL,
		_this->m_hWnd,/*接收消息的窗口*/
		MY_MSG_SCANFILE2 /*接收的消息,这个宏是自定义的,
						该消息会在CVsCleanDlg类中有一
						个消息响应函数:OnScanFile,这
						个函数在消息映射中和
						MY_MSG_SCANFILE绑定*/);
						//_this->MessageBox(L"扫描完毕");
	return 0;
}

void CAntiVirusDlg::OnBnClickedButtonScan()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_editDir.IsEmpty())
	{
		MessageBox(L"请选择目录", L"Tips", 0);
		return;
	}
	m_list.DeleteAllItems();
	m_totalSize = 0;

	// 每次扫描文件列表前，即写入md5值到文件前，先将旧的删除，防止文件过大且内容重复
	DeleteFile(L".\\res\\AntiVirus\\MD5\\md5.txt");

	// 遍历文件, 添加到列表控件
	// 开启线程扫描垃圾文件
	m_hScanThread = CreateThread(0, 0, ScanFileProc2, this, 0, 0);
}

LRESULT CAntiVirusDlg::OnScanFile(WPARAM w, LPARAM l)
{
	CString buffer;
	FILEINFO* fInfo = (FILEINFO*)w;
	m_totalSize += (fInfo->nFileSizeHigh << 32 | fInfo->nFileSizeLow);
	int row = m_list.GetItemCount();
	//m_list.InsertItem(row, fInfo->path);
	m_list.InsertItem(row, L"");
	buffer.Format(_T("%d"), row);
	m_list.SetItemText(row, 0, buffer);
	m_list.SetItemText(row, 1, fInfo->path);


	// 将文件的md5值写入文件
	FILE * fprawFile=NULL;
	FILE* fpmd5file=NULL;
	//char * rawfile = ".\\res\\AntiVirus\\MD5\\Files\\TestFiles\\1.txt";
	char * md5file = ".\\res\\AntiVirus\\MD5\\md5.txt";

	// 宽字节转多字节
	WCHAR wstr[100] = { 0 };
	CHAR rawfile[100] = { 0 };
	wcscpy_s(wstr, 100, fInfo->path);
	WCHAR_TO_CHAR(wstr, rawfile);

	fopen_s(&fprawFile, rawfile, "rb");
	fopen_s(&fpmd5file, md5file, "ab");//追加模式
	MD5VAL md5Value = md5File(fprawFile);
	FILEANDMD5 fileAndMd5 = { row ,md5Value };
	fwrite(&fileAndMd5, sizeof(fileAndMd5), 1, fpmd5file);
	//fwrite(&md5Value, sizeof(MD5VAL), 1, fpmd5file);
	fclose(fprawFile);
	fclose(fpmd5file);


	//滚动到最后一行
	m_list.Scroll(CSize(0, 100000));
	return 1;
}

void CAntiVirusDlg::OnRclickList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;

	CMenu menu;
	menu.LoadMenu(IDR_MENU5);
	CMenu* pSubMenu = menu.GetSubMenu(0);
	CPoint pos;
	GetCursorPos(&pos);
	pSubMenu->TrackPopupMenu(0, pos.x, pos.y, this);
}


void CAntiVirusDlg::OnMd5anti()
{
	// TODO: 在此添加命令处理程序代码

	// 获取点击的文件路径及索引
	POSITION pos = m_list.GetFirstSelectedItemPosition();
	if (pos == NULL) {
		return;
	}
	int nPos = (int)pos - 1;
	CString path = m_list.GetItemText(nPos, 1); 
	int index=_ttoi(m_list.GetItemText(nPos, 0));

	// 对比md5值
	FILE* fpmd5file;
	FILE* fpTestFile;
	char * md5file = ".\\res\\AntiVirus\\MD5\\md5.txt";
	// 宽字节转多字节
	WCHAR wstr[100] = { 0 };
	CHAR testfile[100] = { 0 };
	wcscpy_s(wstr, 100, path);
	WCHAR_TO_CHAR(wstr, testfile);
	fopen_s(&fpmd5file, md5file, "rb");
	fopen_s(&fpTestFile, testfile, "rb");

	MD5VAL md5Value = md5File(fpTestFile);
	//MD5VAL md5Value2;
	FILEANDMD5 fileAndMd5 = { index ,md5Value };

	//CString buff;
	//buff.Format(_T("%d %d %d %d"), md5Value.a, md5Value.b, md5Value.c, md5Value.d);
	//MessageBox(0,buff , 0);
	//fwrite(&md5Value, sizeof(MD5VAL),1, fpmd5file);
	while (1)
	{
		fread(&fileAndMd5, sizeof(FILEANDMD5), 1, fpmd5file);
		// 只检查相应的文件
		if (fileAndMd5.index != index)
		{
			//fpmd5file++;
			continue;
		}
		// 对比md5
		if (!(md5Value.a == fileAndMd5.md5.a &&
			md5Value.b == fileAndMd5.md5.b &&
			md5Value.c == fileAndMd5.md5.c &&
			md5Value.d == fileAndMd5.md5.d))
		{
			MessageBox(L"文件已被篡改", 0, 0);
			break;
		}
		else
		{
			MessageBox(L"文件未被篡改", 0, 0);
			break;
		}


	}
	


	//

	fclose(fpmd5file);
	fclose(fpTestFile);

}
