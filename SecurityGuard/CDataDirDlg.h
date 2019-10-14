#pragma once


// CDataDirDlg 对话框

class CDataDirDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CDataDirDlg)

public:
	CDataDirDlg(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CDataDirDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CDatadirDlg };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CMenu m_menu;
	CListCtrl m_list;
	virtual BOOL OnInitDialog();
	afx_msg void OnRclickList1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnMenuDetailInfo();
};
