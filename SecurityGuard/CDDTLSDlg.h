#pragma once


// CDDTLSDlg 对话框

class CDDTLSDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CDDTLSDlg)

public:
	CDDTLSDlg(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CDDTLSDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CDDTLSDlg };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
};
