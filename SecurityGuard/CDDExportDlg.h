#pragma once


// CExportDlg 对话框

class CDDExportDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CDDExportDlg)

public:
	CDDExportDlg(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CDDExportDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CDDExportDlg };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	CString m_editName;
	CString m_editBase;
	CString m_editFuncNum;
	CString m_editNameNum;
	CString m_editNameAddr;
	CString m_editEAT;
	CString m_editENT;
	CString m_editEOT;
	CListCtrl m_list;
};
