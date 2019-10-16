// CAntiVirus.cpp: 实现文件
//

#include "stdafx.h"
#include "SecurityGuard.h"
#include "CAntiVirusDlg.h"
#include "afxdialogex.h"


// CAntiVirus 对话框

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
	DDX_Control(pDX, IDC_TREE1, m_tree);
	DDX_Text(pDX, IDC_EDIT1, m_editDir);
}


BEGIN_MESSAGE_MAP(CAntiVirusDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_CHOOSEDIR, &CAntiVirusDlg::OnBnClickedButtonChoosedir)
END_MESSAGE_MAP()


// CAntiVirus 消息处理程序


BOOL CAntiVirusDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化

	////FILE * fprawFile;
	//FILE* fpmd5file;
	//FILE* fpvirusfile;
	////char * rawFile = ".\\AntiVirus\\MD5\\RawFiles\\1.txt";
	//char * md5file = ".\\AntiVirus\\MD5\\md5.txt";
	//char * virusfile = ".\\AntiVirus\\MD5\\VirusFiles\\1.txt";
	////fopen_s(&fprawFile, rawFile, "rb");
	//fopen_s(&fpmd5file, md5file, "rb");
	//fopen_s(&fpvirusfile, virusfile, "rb");
	//MD5VAL md5Value = md5File(fpvirusfile);
	//MD5VAL md5Value2;
	////CString buff;
	////buff.Format(_T("%d %d %d %d"), md5Value.a, md5Value.b, md5Value.c, md5Value.d);
	////MessageBox(0,buff , 0);
	////fwrite(&md5Value, sizeof(MD5VAL),1, fpmd5file);
	//fread(&md5Value2, sizeof(MD5VAL), 1, fpmd5file);
	//if (!(md5Value.a == md5Value2.a &&
	//	md5Value.b == md5Value2.b &&
	//	md5Value.c == md5Value2.c &&
	//	md5Value.d == md5Value2.d))
	//{
	//	MessageBox(0, L"hhHhh", 0);
	//}
	////fclose(fprawFile);
	//fclose(fpmd5file);
	//fclose(fpvirusfile);

	// 初始化文件列表
	//InitFileTree(L".\\AntiVirus\\MD5\\Files", m_tree.GetRootItem());
	// 将原文件的md5写入文件
	//HTREEITEM rootNode = m_tree.GetRootItem();
	//HTREEITEM childNode = m_tree.GetChildItem(rootNode);
	//for (int i = 0; m_tree.GetCount(); i++)
	//{
	//	HTREEITEM childNode2 = m_tree.GetNextItem(childNode);
	//	CString name = m_tree.GetItemText(childNode);
	//	MessageBox(0, name, 0);
	//}



	//FILE * fprawFile;
	//FILE* fpmd5file;
	//char * rawFile = ".\\AntiVirus\\MD5\\RawFiles\\1.txt";
	//char * md5file = ".\\AntiVirus\\MD5\\md5.txt";
	//fopen_s(&fprawFile, rawFile, "rb");
	//fopen_s(&fpmd5file, md5file, "wb");
	//MD5VAL md5Value = md5File(fprawFile);
	//fwrite(&md5Value, sizeof(MD5VAL),1, fpmd5file);
	//fclose(fprawFile);
	//fclose(fpmd5file);








	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}


void CAntiVirusDlg::InitFileTree(CString str_Dir, HTREEITEM tree_Root)
{
	CFileFind FileFind;
	//临时变量，用以记录返回的树节点
	HTREEITEM tree_Temp;
	//判断输入目录最后是否存在'\'，不存在则补充
	if (str_Dir.Right(1) != "\\")
		str_Dir += "\\";
	str_Dir += "*.*";
	BOOL res = FileFind.FindFile(str_Dir);
	while (res)
	{
		tree_Temp = tree_Root;
		res = FileFind.FindNextFileW();
		if (FileFind.IsDirectory() && !FileFind.IsDots())//目录是文件夹
		{
			CString strPath = FileFind.GetFilePath(); //得到路径，做为递归调用的开始
			CString strTitle = FileFind.GetFileName();//得到目录名，做为树控的结点
			tree_Temp = m_tree.InsertItem(strTitle, 0, 0, tree_Root);
			InitFileTree(strPath, tree_Temp);
		}
		else if (!FileFind.IsDirectory() && !FileFind.IsDots())//如果是文件
		{
			CString strPath = FileFind.GetFilePath(); //得到路径，做为递归调用的开始
			CString strTitle = FileFind.GetFileName();//得到文件名，做为树控的结点
			m_tree.InsertItem(strTitle, 0, 0, tree_Temp);
		}
	}
	FileFind.Close();
}

void CAntiVirusDlg::OnBnClickedButtonChoosedir()
{
	// TODO: 在此添加控件通知处理程序代码

	
	// 如果目录为空,则弹出文件夹选择对话框
	if (m_editDir.IsEmpty())
	{
		TCHAR szFolderPath[MAX_PATH] = { 0 };
		BROWSEINFO sInfo;
		::ZeroMemory(&sInfo, sizeof(BROWSEINFO));
		sInfo.pidlRoot = 0;
		sInfo.lpszTitle = _T("请选择一个文件夹：");
		sInfo.ulFlags = BIF_DONTGOBELOWDOMAIN | BIF_RETURNONLYFSDIRS | BIF_NEWDIALOGSTYLE | BIF_EDITBOX | BIF_USENEWUI | BIF_BROWSEINCLUDEFILES;
		sInfo.lpfn = NULL;

		// 显示文件夹选择对话框
		LPITEMIDLIST lpidlBrowse = ::SHBrowseForFolder(&sInfo);
		if (lpidlBrowse != NULL)
		{
			// 取得文件夹名
			if (::SHGetPathFromIDList(lpidlBrowse, szFolderPath))
			{
				UpdateData(TRUE);
				m_editDir = szFolderPath;
				UpdateData(FALSE);
			}
		}
		if (lpidlBrowse != NULL)
		{
			::CoTaskMemFree(lpidlBrowse);
		}
		else
		{
			return;
		};
		return;
	}




}
