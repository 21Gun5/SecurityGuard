// CPeInfoDlg.cpp: 实现文件
//

#include "stdafx.h"
#include "SecurityGuard.h"
#include "CPeInfoDlg.h"
#include "afxdialogex.h"
#include "CPe.h"
#include "CSectionDlg.h"
#include "CDataDirDlg.h"
wchar_t * g_PePath ;


// 宽字符转换为多字符(Unicode --> ASCII)
#define  WCHAR_TO_CHAR(lpW_Char, lpChar) \
     WideCharToMultiByte(CP_ACP, NULL, lpW_Char, -1, \
 lpChar, _countof(lpChar), NULL, FALSE);


// 多字符转换为宽字符(ASCII --> Unicode)
#define  CHAR_TO_WCHAR(lpChar, lpW_Char) \
     MultiByteToWideChar(CP_ACP, NULL, lpChar, -1, \
 lpW_Char, _countof(lpW_Char));


// CPeInfoDlg 对话框

IMPLEMENT_DYNAMIC(CPeInfoDlg, CDialogEx)

CPeInfoDlg::CPeInfoDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_CPeInfoDlg, pParent)
	, m_file(_T(""))
	, m_entryPoint(_T(""))
	, m_imageBase(_T(""))
	, m_imageSize(_T(""))
	, m_codeBase(_T(""))
	, m_dataBase(_T(""))
	, m_sectionAlign(_T(""))
	, m_fileAlign(_T(""))
	, m_sectionNum(_T(""))
	, m_headersSize(_T(""))
	, m_optHeaderSize(_T(""))
{

}

CPeInfoDlg::~CPeInfoDlg()
{
}

void CPeInfoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_file);
	DDX_Text(pDX, IDC_EDIT4, m_entryPoint);
	//  DDX_Control(pDX, IDC_EDIT2, m_imageBase);
	DDX_Text(pDX, IDC_EDIT2, m_imageBase);
	DDX_Text(pDX, IDC_EDIT3, m_imageSize);
	DDX_Text(pDX, IDC_EDIT5, m_codeBase);
	DDX_Text(pDX, IDC_EDIT6, m_dataBase);
	DDX_Text(pDX, IDC_EDIT7, m_sectionAlign);
	DDX_Text(pDX, IDC_EDIT8, m_fileAlign);
	DDX_Text(pDX, IDC_EDIT9, m_sectionNum);
	DDX_Text(pDX, IDC_EDIT10, m_headersSize);
	DDX_Text(pDX, IDC_EDIT11, m_optHeaderSize);
}


BEGIN_MESSAGE_MAP(CPeInfoDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_CHOOSE, &CPeInfoDlg::OnBnClickedButtonChoose)
	ON_BN_CLICKED(IDC_BUTTON_ANALYSE, &CPeInfoDlg::OnBnClickedButtonAnalyse)
	ON_BN_CLICKED(IDC_BUTTON_SECINFO, &CPeInfoDlg::OnBnClickedButtonSecInfo)
	ON_BN_CLICKED(IDC_BUTTON_DIRINFO, &CPeInfoDlg::OnBnClickedButtonDirInfo)
	ON_BN_CLICKED(IDC_BUTTON_SAVE, &CPeInfoDlg::OnBnClickedButtonSave)
END_MESSAGE_MAP()


// CPeInfoDlg 消息处理程序


void CPeInfoDlg::OnBnClickedButtonChoose()
{
	// TODO: 在此添加控件通知处理程序代码

	// 获取控件的数据
	UpdateData(TRUE);
	// 如果目录为空,则弹出文件夹选择对话框
	if (m_file.IsEmpty()) 
	{

		TCHAR szFolderPath[MAX_PATH] = { 0 };
		BROWSEINFO sInfo;
		::ZeroMemory(&sInfo, sizeof(BROWSEINFO));
		sInfo.pidlRoot = 0;
		sInfo.lpszTitle = _T("请选择文件：");
		sInfo.ulFlags = BIF_DONTGOBELOWDOMAIN | BIF_RETURNONLYFSDIRS | BIF_NEWDIALOGSTYLE | BIF_EDITBOX | BIF_USENEWUI | BIF_BROWSEINCLUDEFILES;
		sInfo.lpfn = NULL;

		// 显示文件夹选择对话框
		LPITEMIDLIST lpidlBrowse = ::SHBrowseForFolder(&sInfo);
		if (lpidlBrowse != NULL)
		{
			// 取得文件夹名
			if (::SHGetPathFromIDList(lpidlBrowse, szFolderPath))
			{
				m_file = szFolderPath;
				UpdateData(FALSE);
			}
		}
		return;
	}
}

void CPeInfoDlg::OnBnClickedButtonAnalyse()
{
	// TODO: 在此添加控件通知处理程序代码

	if (m_file.IsEmpty())
	{
		MessageBox(L"先选择文件", 0, 0);
		return;
	}


	// 将所选择的文件路径，赋值为全局变量
	WCHAR wstr[100] = { 0 };
	CHAR str[100] = { 0 };
	wcscpy_s(wstr, 100, m_file);
	//WCHAR_TO_CHAR(wstr, str);
	g_PePath = wstr;



	CPe pe;
	// 加载文件
	bool isPe = pe.InitPe((TCHAR*)g_PePath);
	//检测是否是PE文件
	if (!isPe)
	{
		MessageBox(L"不是PE文件,请重新选择", L"Tips", 0);
		return;
	}
	else
		MessageBox(L"是PE文件", L"Tips", 0);

	PIMAGE_NT_HEADERS pNt = pe.GetNtHeader();

	UpdateData(TRUE);
	m_entryPoint.Format(_T("%08X"), pNt->OptionalHeader.AddressOfEntryPoint);
	m_imageBase.Format(_T("%08X"), pNt->OptionalHeader.ImageBase);
	m_imageSize.Format(_T("%08X"), pNt->OptionalHeader.SizeOfImage);
	m_codeBase.Format(_T("%08X"), pNt->OptionalHeader.BaseOfCode);
	m_dataBase.Format(_T("%08X"), pNt->OptionalHeader.BaseOfData);
	m_sectionAlign.Format(_T("%08X"), pNt->OptionalHeader.SectionAlignment);
	m_fileAlign.Format(_T("%08X"), pNt->OptionalHeader.FileAlignment);
	m_sectionNum.Format(_T("%08X"), pNt->FileHeader.NumberOfSections);
	m_headersSize.Format(_T("%08X"), pNt->OptionalHeader.SizeOfHeaders);
	m_optHeaderSize.Format(_T("%08X"), pNt->FileHeader.SizeOfOptionalHeader);
	UpdateData(FALSE);

		

}


void CPeInfoDlg::OnBnClickedButtonSecInfo()
{
	// TODO: 在此添加控件通知处理程序代码

	if (m_file.IsEmpty())
	{
		MessageBox(L"先选择文件", 0, 0);
		return;
	}

		// 将所选择的文件路径，赋值为全局变量
	WCHAR wstr[100] = { 0 };
	CHAR str[100] = { 0 };
	wcscpy_s(wstr, 100, m_file);
	//WCHAR_TO_CHAR(wstr, str);
	g_PePath = wstr;

	CSectionDlg dlg;
	dlg.DoModal();
}


void CPeInfoDlg::OnBnClickedButtonDirInfo()
{
	// TODO: 在此添加控件通知处理程序代码

	if (m_file.IsEmpty())
	{
		MessageBox(L"先选择文件", 0, 0);
		return ;
	}

	// 将所选择的文件路径，赋值为全局变量
	WCHAR wstr[100] = { 0 };
	CHAR str[100] = { 0 };
	wcscpy_s(wstr, 100, m_file);
	//WCHAR_TO_CHAR(wstr, str);
	g_PePath = wstr;

	CDataDirDlg dlg;
	dlg.DoModal();
}


void CPeInfoDlg::OnBnClickedButtonSave()
{
	// TODO: 在此添加控件通知处理程序代码
	MessageBox(L"敬请期待", L"Tips", 0);
}
