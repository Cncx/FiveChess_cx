#pragma once


// About 对话框

class About : public CDialogEx
{
	DECLARE_DYNAMIC(About)

public:
	About(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~About();

	virtual void OnFinalRelease();

// 对话框数据          
	enum { IDD = IDD_ABOUT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP() 
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()
public:
	afx_msg void OnBnClickedOk();           
};
