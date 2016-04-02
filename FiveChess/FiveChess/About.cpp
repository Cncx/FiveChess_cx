// About.cpp : 实现文件
//

#include "stdafx.h"
#include "FiveChess.h"
#include "FiveChessDlg.h"
#include "About.h"
#include "afxdialogex.h"


// About 对话框

IMPLEMENT_DYNAMIC(About, CDialogEx)

About::About(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_ABOUT, pParent)
{

	EnableAutomation();

}

About::~About()
{
}

void About::OnFinalRelease()
{
	// 释放了对自动化对象的最后一个引用后，将调用
	// OnFinalRelease。  基类将自动
	// 删除该对象。  在调用该基类之前，请添加您的
	// 对象所需的附加清理代码。

	CDialogEx::OnFinalRelease();
}

void About::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(About, CDialogEx)
	ON_BN_CLICKED(IDOK, &About::OnBnClickedOk)
END_MESSAGE_MAP()

BEGIN_DISPATCH_MAP(About, CDialogEx)
END_DISPATCH_MAP()

// 注意: 我们添加 IID_IAbout 支持
//  以支持来自 VBA 的类型安全绑定。  此 IID 必须同附加到 .IDL 文件中的
//  调度接口的 GUID 匹配。

// {C7EF48FC-13D5-44D1-AFC5-A08C37F27AE6}
static const IID IID_IAbout =
{ 0xC7EF48FC, 0x13D5, 0x44D1, { 0xAF, 0xC5, 0xA0, 0x8C, 0x37, 0xF2, 0x7A, 0xE6 } };

BEGIN_INTERFACE_MAP(About, CDialogEx)
	INTERFACE_PART(About, IID_IAbout, Dispatch)
END_INTERFACE_MAP()


// About 消息处理程序


void About::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	CDialogEx::OnOK();
}
