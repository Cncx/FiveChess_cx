// InternetDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "FiveChess.h"
#include "InternetDlg.h"
#include "afxdialogex.h"


// CInternetDlg 对话框

IMPLEMENT_DYNAMIC(CInternetDlg, CDialogEx)

CInternetDlg::CInternetDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CInternetDlg::IDD, pParent)
{
	isHost = TRUE;
	isBlack = TRUE;
	netPort = 999;
	ipAddress = "127.0.0.1";
}

CInternetDlg::~CInternetDlg()
{
}

void CInternetDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_PORT, netPort);
	DDX_Radio(pDX, IDC_RADIO_CLIENT, isHost);
	DDX_Radio(pDX, IDC_RADIO_WHITE, isBlack);

	GetDlgItem(IDC_IPADDRESS)->GetWindowText(ipAddress);

	if(isLink)
	{
		GetDlgItem(IDC_RADIO_CLIENT)->EnableWindow(FALSE);
		GetDlgItem(IDC_RADIO_HOST)->EnableWindow(FALSE);
		GetDlgItem(IDC_IPADDRESS)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_PORT)->EnableWindow(FALSE);
	}
	if(!isHost)
	{
		GetDlgItem(IDC_RADIO_BLACK)->EnableWindow(FALSE);
		GetDlgItem(IDC_RADIO_WHITE)->EnableWindow(FALSE);
	}
}


BEGIN_MESSAGE_MAP(CInternetDlg, CDialogEx)
	ON_BN_CLICKED(IDC_RADIO_CLIENT, &CInternetDlg::OnBnClickedRadioClient)
	ON_BN_CLICKED(IDC_RADIO_HOST, &CInternetDlg::OnBnClickedRadioHost)
	ON_EN_CHANGE(IDC_EDIT_PORT, &CInternetDlg::OnChangeEditPort)
	ON_BN_CLICKED(IDOK, &CInternetDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CInternetDlg 消息处理程序

BOOL CInternetDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	GetDlgItem(IDC_IPADDRESS)->SetWindowText("127.0.0.1");
	GetDlgItem(IDC_IPADDRESS)->EnableWindow(FALSE);
	((CEdit*)GetDlgItem(IDC_EDIT_PORT))->LimitText(8);

	return TRUE;
}

void CInternetDlg::OnBnClickedRadioClient()
{
	GetDlgItem(IDC_IPADDRESS)->EnableWindow(TRUE);
	GetDlgItem(IDC_RADIO_BLACK)->EnableWindow(FALSE);
	GetDlgItem(IDC_RADIO_WHITE)->EnableWindow(FALSE);
}


void CInternetDlg::OnBnClickedRadioHost()
{
	GetDlgItem(IDC_IPADDRESS)->EnableWindow(FALSE);
	GetDlgItem(IDC_RADIO_BLACK)->EnableWindow(TRUE);
	GetDlgItem(IDC_RADIO_WHITE)->EnableWindow(TRUE);
}


void CInternetDlg::OnChangeEditPort()
{
	char cPort[8];
	((CEdit*)GetDlgItem(IDC_EDIT_PORT))->GetWindowText(cPort,8);
	if(atoi(cPort) > 65535) //限制输入范围，超限发出提示音不弹窗
	{
		MessageBeep(0);
		((CEdit*)GetDlgItem(IDC_EDIT_PORT))->SetWindowText("65535");
		((CEdit*)GetDlgItem(IDC_EDIT_PORT))->SetSel(0,-1);
	}
}



void CInternetDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	CDialogEx::OnOK();
}
