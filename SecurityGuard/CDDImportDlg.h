#pragma once


// CImportDlg 对话框

class CDDImportDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CDDImportDlg)

public:
	CDDImportDlg(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CDDImportDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CDDImportDlg };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CListCtrl m_listDll;
	CListCtrl m_listFunc;
	virtual BOOL OnInitDialog();
	afx_msg void OnClickListDll(NMHDR *pNMHDR, LRESULT *pResult);
};
