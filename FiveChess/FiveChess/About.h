#pragma once


// About �Ի���

class About : public CDialogEx
{
	DECLARE_DYNAMIC(About)

public:
	About(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~About();

	virtual void OnFinalRelease();

// �Ի�������          
	enum { IDD = IDD_ABOUT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP() 
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()
public:
	afx_msg void OnBnClickedOk();           
};
