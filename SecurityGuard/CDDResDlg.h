#pragma once


// CDDResDlg 对话框

class CDDResDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CDDResDlg)

public:
	CDDResDlg(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CDDResDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CDDResDlg };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
//	CTreeCtrl m_tree;
	CString m_editRVA;
//	CEdit m_editSize;
	CString m_editSize;
	CString m_editCurNum;
	CString m_editTotalNum;
	CString m_editType;
	int m_arr[20][200] = { 0 };
	CListCtrl m_listType;
	CListCtrl m_listRes;
	afx_msg void OnClickListType(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnClickListRes(NMHDR *pNMHDR, LRESULT *pResult);
};
