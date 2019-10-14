#pragma once

#include <vector>
#include "CTools.h"
#include "CModuleDlg.h"

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
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CListCtrl m_list;
	//CMenu m_Menu;
	virtual BOOL OnInitDialog();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	void UpdateProcessList();
	std::vector<PROCESSINFO> m_procList;
	bool IsFindItemInList(std::vector<PROCESSINFO> list, DWORD pid);
	afx_msg void OnRclickList1(NMHDR *pNMHDR, LRESULT *pResult);
	
	CMenu m_menu;

	afx_msg void OnMenuKillproc();
	afx_msg void OnMenuListmodule();
};
