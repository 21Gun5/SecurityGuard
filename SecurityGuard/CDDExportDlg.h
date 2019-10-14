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
	enum { IDD = IDD_CExportDlg };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
};
