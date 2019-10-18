
// SecurityGuardDlg.h: 头文件
//

#pragma once


// CSecurityGuardDlg 对话框
class CSecurityGuardDlg : public CDialogEx
{
// 构造
public:
	CSecurityGuardDlg(CWnd* pParent = nullptr);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SECURITYGUARD_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
//	CTabCtrl m_tab;
	afx_msg void OnBnClickedButtonClean();
	afx_msg void OnBnClickedButtonProcess();
	afx_msg void OnBnClickedButtonWindow();
	afx_msg void OnBnClickedButtonPE();
//	afx_msg void OnBnClickedButtonThread();
	afx_msg void OnBnClickedButtonAntivirus();
	afx_msg void OnBnClickedButtonService();
};
