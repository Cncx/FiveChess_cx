// About.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "FiveChess.h"
#include "FiveChessDlg.h"
#include "About.h"
#include "afxdialogex.h"


// About �Ի���

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
	// �ͷ��˶��Զ�����������һ�����ú󣬽�����
	// OnFinalRelease��  ���ཫ�Զ�
	// ɾ���ö���  �ڵ��øû���֮ǰ�����������
	// ��������ĸ���������롣

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

// ע��: ������� IID_IAbout ֧��
//  ��֧������ VBA �����Ͱ�ȫ�󶨡�  �� IID ����ͬ���ӵ� .IDL �ļ��е�
//  ���Ƚӿڵ� GUID ƥ�䡣

// {C7EF48FC-13D5-44D1-AFC5-A08C37F27AE6}
static const IID IID_IAbout =
{ 0xC7EF48FC, 0x13D5, 0x44D1, { 0xAF, 0xC5, 0xA0, 0x8C, 0x37, 0xF2, 0x7A, 0xE6 } };

BEGIN_INTERFACE_MAP(About, CDialogEx)
	INTERFACE_PART(About, IID_IAbout, Dispatch)
END_INTERFACE_MAP()


// About ��Ϣ�������


void About::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CDialogEx::OnOK();
}
