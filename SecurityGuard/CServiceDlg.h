#pragma once
#include <winsvc.h>

// CServiceDlg 对话框

class CServiceDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CServiceDlg)

public:
	CServiceDlg(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CServiceDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CServiceDlg };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CListCtrl m_list;
	virtual BOOL OnInitDialog();
//	afx_msg void OnClickList1(NMHDR *pNMHDR, LRESULT *pResult);
	CMenu m_menu;
	afx_msg void OnRclickList1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void Onstartservice();
	afx_msg void Onstopservice();
	void UpdateList();
};
