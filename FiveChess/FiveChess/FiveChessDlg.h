
// FiveChessDlg.h : ͷ�ļ�
//

#pragma once
#include "MySocket.h"

// CFiveChessDlg �Ի���
class CFiveChessDlg : public CDialogEx
{
// ����
public:
	CFiveChessDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_FIVECHESS_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	CMySocket sckHost;
	CMySocket sckClient;
	int gameMode;
	void RestartGame(bool isWarm);
	void IsOver(int x,int y);
	int IsForbid(int x, int y);
	void PutDown(int x, int y);
	void OnAutoUp();
	void GoBack();
	void ComputerDown();
	void ComputerDown2(int *tx, int *ty);
	int goBack[801]; //������Ϣ,��goBack[0]��ʾ����Ĳ���,ÿ������2��ֵ,��������ܷ�400������,���������Ҫ801����ֵ.
	int chessData[20][20];
	CImageList imgList;
	bool isBlack;
	bool isTurn; //��ʶ������Ϸ�Ƿ��ֵ���������
	bool isLink;
	bool isHost;
	bool isFirst;
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnBnClickedStartGame();
	afx_msg void OnBnClickedGoBack();
	afx_msg void OnBnClickedRestartGame();
	afx_msg void OnBnClickedExitGame();
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnClose();
	afx_msg void OnBnClickedStartComputer();
	afx_msg void OnBnClickedInternetGame();
	afx_msg void OnBnClickedInternetGame2();
	afx_msg LRESULT OnMySocket(WPARAM wParam,LPARAM lParam);
	afx_msg void OnBnClickedAbout();
};
