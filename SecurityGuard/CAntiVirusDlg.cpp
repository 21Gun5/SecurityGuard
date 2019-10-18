// CAntiVirus.cpp: 实现文件
//

#include "stdafx.h"
#include "SecurityGuard.h"
#include "CAntiVirusDlg.h"
#include "afxdialogex.h"


#include <locale.h>

#include<WinSock2.h>
#pragma comment(lib,"ws2_32.lib")
#include <Ws2tcpip.h>


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
	DDX_Control(pDX, IDC_LIST2, m_listWhiteProcess);
}


BEGIN_MESSAGE_MAP(CAntiVirusDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_CHOOSEDIR, &CAntiVirusDlg::OnBnClickedButtonChoosedir)
	ON_BN_CLICKED(IDC_BUTTON_SCAN, &CAntiVirusDlg::OnBnClickedButtonScan)
	ON_MESSAGE(MY_MSG_SCANFILE2, &CAntiVirusDlg::OnScanFile)
	ON_NOTIFY(NM_RCLICK, IDC_LIST1, &CAntiVirusDlg::OnRclickList1)
	ON_COMMAND(ID_MD5Anti, &CAntiVirusDlg::OnMd5anti)
	ON_BN_CLICKED(IDC_BUTTON_VIRUSPROCESS, &CAntiVirusDlg::OnBnClickedButtonVirusprocess)
	ON_COMMAND(ID_KillVirusProc, &CAntiVirusDlg::OnKillvirusproc)
	ON_NOTIFY(NM_RCLICK, IDC_LIST2, &CAntiVirusDlg::OnRclickList2)
	ON_COMMAND(ID_antiOnline, &CAntiVirusDlg::Onantionline)
END_MESSAGE_MAP()


// CAntiVirus 消息处理程序


BOOL CAntiVirusDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	// 文件列表
	m_list.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	m_list.InsertColumn(0, L"Index", 0, 50);
	m_list.InsertColumn(1, L"路径", 0, 700);
	//m_list.InsertColumn(1, L"Size", 0, 100);

	// 白名单外进程列表
	m_listWhiteProcess.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	m_listWhiteProcess.InsertColumn(0, L"名称", 0, 250);
	m_listWhiteProcess.InsertColumn(1, L"PID", 0, 150);


	// 初始化白名单（用首次获取的进程来模拟
	getAllProcess(&m_vecWhiteProcessList);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}



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
	FILE * fprawFile = NULL;
	FILE* fpmd5file = NULL;
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
	int index = _ttoi(m_list.GetItemText(nPos, 0));

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


void CAntiVirusDlg::OnBnClickedButtonVirusprocess()
{
	// TODO: 在此添加控件通知处理程序代码

	// 清空旧列表
	m_listWhiteProcess.DeleteAllItems();
	// 获取新的进程列表
	std::vector<PROCESSINFO> newProcList;
	getAllProcess(&newProcList);
	// 将不在白名单中的列举出来
	int index = 0;
	for (auto&i : newProcList)
	{
		if (-1 == IndexOfProcessList(m_vecWhiteProcessList, i.th32ProcessID))
		{
			// 插入到列表控件中
			CString buffer;
			m_listWhiteProcess.InsertItem(index, _T(""));
			m_listWhiteProcess.UpdateWindow();
			m_listWhiteProcess.SetItemText(index, 0, i.szExeFile);
			buffer.Format(_T("%d"), i.th32ProcessID);
			m_listWhiteProcess.SetItemText(index, 1, buffer);
			++index;
		}
	}

}


void CAntiVirusDlg::OnRclickList2(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;

	CMenu menu;
	menu.LoadMenu(IDR_MENU6);
	CMenu* pSubMenu = menu.GetSubMenu(0);
	CPoint pos;
	GetCursorPos(&pos);
	pSubMenu->TrackPopupMenu(0, pos.x, pos.y, this);
}


void CAntiVirusDlg::OnKillvirusproc()
{
	// TODO: 在此添加命令处理程序代码

	// 获取待结束进程（通过光标选择序号，序号从1开始，故-1
	int index = (int)m_listWhiteProcess.GetFirstSelectedItemPosition() - 1;
	// 获取进程id（字符串转整形
	CString strPid = m_listWhiteProcess.GetItemText(index, 1);// 第1列是pid
	DWORD dwPid = _wtoi(strPid);
	// 获取进程句柄（要通过句柄来结束进程
	HANDLE hProc = OpenProcess(PROCESS_TERMINATE, FALSE, dwPid);

	TerminateProcess(hProc, 0);// 结束进程
	m_listWhiteProcess.DeleteItem(index);//从列表中删除
	// 关闭句柄
	CloseHandle(hProc);
}





void CAntiVirusDlg::Onantionline()
{
	// TODO: 在此添加命令处理程序代码

	// 获取点击处文件的md5值
	int index = (int)m_list.GetFirstSelectedItemPosition() - 1;
	CString name = m_list.GetItemText(index, 1);
	FILE* fpfile;
	//char * filename = "C:\\Users\\Rylynn\\source\\repos\\SecurityGuard\\SecurityGuard\\res\\AntiVirus\\KillOnline\\virus2.txt";
	char filename[100] = { 0 };
	// 将文件名从cstring转为char *
	WCHAR wstr2[100] = { 0 };
	CHAR str2[100] = { 0 };
	wcscpy_s(wstr2, 100, name);
	WCHAR_TO_CHAR(wstr2, str2);
	memcpy_s(filename, 100, str2, sizeof(str2));
	// 构造md5的字符串
	fopen_s(&fpfile, filename, "rb");
	MD5VAL md5Value = md5File(fpfile);
	CString buffMD5;
	buffMD5.Format(L"%d%d%d%d", md5Value.a, md5Value.b, md5Value.c, md5Value.a);
	//MessageBox(buffMD5);

	//1.初始化环境 必须调用加载socket库
	WSADATA wdata = {};
	WSAStartup(MAKEWORD(2, 2),	&wdata);
	//2.创建套接字
	SOCKET ClientSock =socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
	//3.发起连接
	sockaddr_in Saddr = {};//服务器地址信息
	Saddr.sin_family = AF_INET;//设置协议
	Saddr.sin_port = htons(6666);//设置服务器的端口
	inet_pton(AF_INET, "127.0.0.1", &Saddr.sin_addr);// 配置服务端的地址和端口
	//发起连接
	int conRes = connect(ClientSock, (sockaddr*)&Saddr, sizeof(Saddr));
	if (conRes != 0)
	{
		MessageBox(L"服务器连接失败", L"Tips");
		return;
	}
	
	// 构造发送的数据
	char sendBuff[100];
	WCHAR wstr[100] = { 0 };
	CHAR str[100] = { 0 };
	wcscpy_s(wstr, 100, buffMD5);
	WCHAR_TO_CHAR(wstr, str);
	memcpy_s(sendBuff, 100, str, sizeof(str));

	//发送到服务上
	send(ClientSock, sendBuff, 100, 0);
	char recvBuff[100] = {0};
	recv(ClientSock, recvBuff, 100, 0);
	//显示服务器发送回来的信息
	CString buffer;
	buffer.Format(L"%s", recvBuff);
	//int rescode = _ttoi(buffer);
	if (buffer == L"1")
		MessageBox(L"已知病毒",L"云端响应结果");
	else
		MessageBox(L"正常文件", L"云端响应结果");
	//MessageBox(buffer);

	closesocket(ClientSock);//5.关闭套接字
	WSACleanup();//6.清理环境

	return;
}
