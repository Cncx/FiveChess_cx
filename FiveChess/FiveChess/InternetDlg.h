#pragma once


// CInternetDlg 对话框

class CInternetDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CInternetDlg)

public:
	CInternetDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CInternetDlg();

// 对话框数据
	enum { IDD = IDD_INTERNET };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	BOOL isHost;
	BOOL isBlack;
	bool isLink;
	int netPort;
	CString ipAddress;
	afx_msg void OnBnClickedRadioClient();
	afx_msg void OnBnClickedRadioHost();
	afx_msg void OnChangeEditPort();
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
};
