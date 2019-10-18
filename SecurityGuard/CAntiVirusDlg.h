#pragma once
#include "MD5.h"
#include "CTools.h"
using namespace std;

// CAntiVirus 对话框

class CAntiVirusDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CAntiVirusDlg)

public:
	CAntiVirusDlg(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CAntiVirusDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CAntiVirusDlg };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
//	CTreeCtrl m_tree;
//	void InitFileTree(CString str_Dir, HTREEITEM tree_Root);
	afx_msg void OnBnClickedButtonChoosedir();
	CString m_editDir;

	LRESULT OnScanFile(WPARAM w, LPARAM l);
	void OnBnClickedButtonScan();
	CListCtrl m_list;

	DWORD64 m_totalSize;
	HANDLE m_hScanThread;

	const TCHAR* m_filter = L".txt .exe .dll";
	afx_msg void OnRclickList1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnMd5anti();
	CListCtrl m_listWhiteProcess;


	std::vector<PROCESSINFO> m_vecWhiteProcessList;

	afx_msg void OnBnClickedButtonVirusprocess();
	afx_msg void OnKillvirusproc();
	afx_msg void OnRclickList2(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void Onantionline();
};

// 文件名和md5的结构体
typedef struct _FILEANDMD5
{
	int index;
	MD5VAL md5;
}FILEANDMD5;