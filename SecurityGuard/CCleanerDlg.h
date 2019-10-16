#pragma once


// CCleanerDlg 对话框

class CCleanerDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CCleanerDlg)

public:
	CCleanerDlg(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CCleanerDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CCleanerDlg };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CString m_editDir;
	CString m_editFilter;
	afx_msg void OnBnClickedButtonChoosedir();
	afx_msg void OnBnClickedButtonScan();
	CListCtrl m_list;
	virtual BOOL OnInitDialog();
	//bool m_isScanBegin = false;
	DWORD64 m_totalSize;
	HANDLE m_hScanThread;
	//DWORD CALLBACK ScanFileProc(VOID* arg);
	LRESULT OnScanFile(WPARAM w, LPARAM l);

	afx_msg void OnRclickList1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnFileSelectall();
	afx_msg void OnFileUnselectAll();
	afx_msg void OnFileDeleteall();
	afx_msg void OnFileDeleteone();
};
