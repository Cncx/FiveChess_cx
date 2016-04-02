
// FiveChessDlg.h : 头文件
//

#pragma once
#include "MySocket.h"

// CFiveChessDlg 对话框
class CFiveChessDlg : public CDialogEx
{
// 构造
public:
	CFiveChessDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_FIVECHESS_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
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
	int goBack[801]; //悔棋信息,用goBack[0]表示保存的步数,每步包含2个值,棋盘最多能放400颗棋子,所以最大需要801个数值.
	int chessData[20][20];
	CImageList imgList;
	bool isBlack;
	bool isTurn; //标识联网游戏是否轮到本机下棋
	bool isLink;
	bool isHost;
	bool isFirst;
	HICON m_hIcon;

	// 生成的消息映射函数
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
