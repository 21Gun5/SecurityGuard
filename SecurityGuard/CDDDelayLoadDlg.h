#pragma once


// CDDDelayLoadDlg 对话框

class CDDDelayLoadDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CDDDelayLoadDlg)

public:
	CDDDelayLoadDlg(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CDDDelayLoadDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CDDDelayLoadDlg };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
};
