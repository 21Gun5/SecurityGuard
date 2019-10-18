#pragma once

#include "CTools.h"

// CProcessDlg 对话框

class CProcessDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CProcessDlg)

public:
	CProcessDlg(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CProcessDlg();

	// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum {
		IDD = IDD_CProcessDlg
};
#endif

protected:
	CMenu m_menu;

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	void updateProcessList();
	std::vector<PROCESSINFO> m_procList;

	afx_msg void OnSize(UINT nType, int cx, int cy);
	CListCtrl m_list;
	virtual BOOL OnInitDialog();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnRclickList1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnMenuKillproc();
	afx_msg void OnMenuListmodule();
	afx_msg void OnMenuListThread();

	afx_msg void OnBnClickedButtonMemstatus();
	afx_msg void OnBnClickedButtonMemclean();
	afx_msg void OnProtectproc();
	afx_msg void Ondeleteprotect();
};
