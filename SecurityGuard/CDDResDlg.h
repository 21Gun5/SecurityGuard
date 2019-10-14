#pragma once


// CDDResDlg 对话框

class CDDResDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CDDResDlg)

public:
	CDDResDlg(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CDDResDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CDDResDlg };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
};
