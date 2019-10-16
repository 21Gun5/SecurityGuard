#pragma once
#include <vector>
#include "CTools.h"


// CThreadDlg 对话框

class CThreadDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CThreadDlg)

public:
	CThreadDlg(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CThreadDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CThreadDlg };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	DWORD m_pID;
	CString m_pName;
	void SetProcessID(DWORD pID);
	void SetProcessName(CString pName);
	CListCtrl m_list;
};
