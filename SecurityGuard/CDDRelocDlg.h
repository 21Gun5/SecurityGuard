#pragma once


// CDDRelocDlg 对话框

class CDDRelocDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CDDRelocDlg)

public:
	CDDRelocDlg(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CDDRelocDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CDDRelocDlg };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
};
