#pragma once


// CPeInfoDlg 对话框

extern wchar_t * g_PePath;

class CPeInfoDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CPeInfoDlg)

public:
	CPeInfoDlg(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CPeInfoDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CPeInfoDlg };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:

	afx_msg void OnBnClickedButtonChoose();
	CString m_file;

	afx_msg void OnBnClickedButtonAnalyse();
	CString m_entryPoint;
	CString m_imageBase;
	CString m_imageSize;
	CString m_codeBase;
	CString m_dataBase;
	CString m_sectionAlign;
	CString m_fileAlign;
	CString m_sectionNum;
	CString m_headersSize;
	CString m_optHeaderSize;
	afx_msg void OnBnClickedButtonSecInfo();
	afx_msg void OnBnClickedButtonDirInfo();
	afx_msg void OnBnClickedButtonSave();
};
