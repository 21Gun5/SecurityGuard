#pragma once
#include "MD5.h"

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
	CTreeCtrl m_tree;
	void InitFileTree(CString str_Dir, HTREEITEM tree_Root);
	afx_msg void OnBnClickedButtonChoosedir();
	CString m_editDir;
};
