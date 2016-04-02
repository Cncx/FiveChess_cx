
// FiveChessDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "FiveChess.h"
#include "FiveChessDlg.h"
#include "InternetDlg.h"
#include "afxdialogex.h"
#include "About.h"
#include <windows.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

int *tx = new int;
int *ty = new int;


// CFiveChessDlg 对话框


CFiveChessDlg::CFiveChessDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CFiveChessDlg::IDD, pParent)
{
	gameMode = 1; //初始游戏模式
	isBlack = true;
	isTurn = true;
	isHost = true;
	isLink = false;
	isFirst = true;
	CBitmap White; //初始化相关图片
	CBitmap Black;
	Black.LoadBitmap(IDB_BLACK);
	White.LoadBitmap(IDB_WHITE);
	imgList.Create(24,24,ILC_COLOR24|ILC_MASK,0,2);
	imgList.Add(&White,0xff00ff); //使用掩码去除位图中需要透明的部分
	imgList.Add(&Black,0xff00ff);

	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CFiveChessDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CFiveChessDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_ERASEBKGND()
	ON_BN_CLICKED(IDC_START_GAME, &CFiveChessDlg::OnBnClickedStartGame)
	ON_BN_CLICKED(IDC_GO_BACK, &CFiveChessDlg::OnBnClickedGoBack)
	ON_BN_CLICKED(IDC_RESTART_GAME, &CFiveChessDlg::OnBnClickedRestartGame)
	ON_BN_CLICKED(IDC_EXIT_GAME, &CFiveChessDlg::OnBnClickedExitGame)
	ON_WM_LBUTTONUP()
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_START_COMPUTER, &CFiveChessDlg::OnBnClickedStartComputer)
	ON_BN_CLICKED(IDC_INTERNET_GAME, &CFiveChessDlg::OnBnClickedInternetGame)
	ON_MESSAGE(WM_MYSOCKET, &CFiveChessDlg::OnMySocket)
	ON_BN_CLICKED(IDC_INTERNET_GAME2, &CFiveChessDlg::OnBnClickedInternetGame2)
	ON_BN_CLICKED(IDC_ABOUT, &CFiveChessDlg::OnBnClickedAbout)
END_MESSAGE_MAP()

// CFiveChessDlg 消息处理程序

BOOL CFiveChessDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	RestartGame(false); // 开始游戏不提示
	AfxSocketInit(); //初始化套接字库

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。

void CFiveChessDlg::OnPaint()
{
	CPaintDC dc(this);
	// 加载棋盘图片
	CBitmap board;
	board.LoadBitmap(IDB_BOARD);
    CDC memdc;
	memdc.CreateCompatibleDC(&dc);
	memdc.SelectObject(&board);
	dc.BitBlt(0,10,526,727,&memdc,1,0,SRCCOPY);
	ReleaseDC(&memdc);

	for(int i=0;i<20;i++) // 绘制棋盘信息
		for(int j=0;j<20;j++)
		{
			if(chessData[i][j]==1)
				imgList.Draw(&dc,1,CPoint(i*25+13,j*25+198),ILD_TRANSPARENT);
			if(chessData[i][j]==-1)
				imgList.Draw(&dc,0,CPoint(i*25+13,j*25+198),ILD_TRANSPARENT);
		}

	if (IsIconic())
	{
		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// 截获WM_ERASEBKGND消息以防闪烁
BOOL CFiveChessDlg::OnEraseBkgnd(CDC* pDC)
{	// 背景不做擦除，重绘时将原有内容覆盖
	return TRUE;
}

//当用户拖动最小化窗口时系统调用此函数取得光标显示
HCURSOR CFiveChessDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

// 开始游戏按钮
void CFiveChessDlg::OnBnClickedStartGame()
{
	gameMode = 1;
	RestartGame(true);
}

// 人机对战按钮
void CFiveChessDlg::OnBnClickedStartComputer()
{
	gameMode = 2;
	RestartGame(true);
}

// 重新开始按钮
void CFiveChessDlg::OnBnClickedRestartGame()
{	
	RestartGame(true);
}

// 重新开始游戏
void CFiveChessDlg::RestartGame(bool isWarm)
{
	for(int i=0;i<20;i++)
		for(int j=0;j<20;j++)
		{	// 如果棋盘上有棋子则根据isWarm进行提示
			if(isWarm && chessData[i][j]!=0)
			{
				if(MessageBox("游戏已经开始！\n是否重新开始游戏？","温馨提示",
					MB_YESNO|MB_ICONQUESTION)==IDYES)
				{
					isWarm = false; //不再重复提示
					if(gameMode==3 && isLink)
					{
						int send[3] = { 5, 0, 0 }; //5表示重玩
						sckClient.Send((LPVOID)send,sizeof(send));
					}
				}
				else	return; // 点击“否”结束函数

			}
			chessData[i][j] = 0;
		}
	for(int k=0;k<451;k++) // 重置悔棋信息
		goBack[k] = 0;

	if(gameMode==3 && isLink && isHost)
	{		//告诉对方具有相反的先手状态
		int send[3] = { 3, !isFirst, 0 }; // 3表示初始先手信息
		sckClient.Send((LPVOID)send,sizeof(send));
	}
	isTurn = isFirst;
	isBlack = true; // 默认黑棋为先手
	Invalidate(); // 重新绘制窗口信息
}

// 使用鼠标下棋
void CFiveChessDlg::OnLButtonUp(UINT nFlags, CPoint point)
{
	// 将点击的位置转化成棋盘棋子的位置
	int cx = (point.x-13)/25;
	int cy = (point.y-198)/25;
	 
	if(point.x>13 && point.x<513 && point.y>198 && point.y<698 && chessData[cx][cy]==0)
	{
		if(gameMode == 1) // 双人本地游戏
		{
			PutDown(cx, cy);
		}

		if(gameMode == 2) // 本地人机游戏
		{	// 默认玩家使用黑棋
			if(isBlack) PutDown(cx, cy);
			if(!isBlack) ComputerDown();
		}
		if(gameMode == 3 && isTurn) // 双人网络游戏
		{
			int send[3] = {isBlack,cx,cy};
			sckClient.Send((LPVOID)send,sizeof(send));
			PutDown(cx,cy);
			isTurn = false;
		}
		if (gameMode == 4 && isTurn)
		{
			ComputerDown2(tx, ty);
			int send[3] = { isBlack,*tx,*ty };
			sckClient.Send((LPVOID)send, sizeof(send));
			PutDown(*tx, *ty);
			isTurn = false;
		}
	}
	else MessageBeep(MB_OK); // 发出警告音
	CDialogEx::OnLButtonUp(nFlags, point);
}

// 电脑下棋函数
void CFiveChessDlg::ComputerDown()
{
	int mode=0,cur=0,curX=0,curY=0,sum1=0,sum2=0;
	int x=0,y=0,cx=0,cy=0,cPrior=0,prior=0,flgL=0,flgR=0;
	for(x=0;x<20;x++)
	{
		for(y=0;y<20;y++)
		{	// 扫描全部空白点
			if(chessData[x][y]==0)
			{
				prior = 0;
				for(mode=1;mode<5;mode++)	// 模式1为 — 向判断，模式2为 | 向判断
				{							// 模式3为 \ 向判断，模式4为 / 向判断
					sum1 = sum2 = 0;
					flgL = flgR = 0;
					curX = x;
					curY = y; //改变模式后重置变量
					for(cur=-4;cur<=4;cur++)
					{
						if(mode == 1) //向→检查
						{
							curX = x+cur;
							if(curX<0) continue; //左越界继续
							if(curX>19) break;   //右越界停止
						}
						if(mode == 2) //向↓检查
						{
							curY = y+cur;
							if(curY<0) continue; //上越界继续
							if(curY>19) break;   //下越界停止
						}
						if(mode == 3) //向↘检查
						{
							curX = x+cur;
							curY = y+cur;
							if(curX<0 || curY<0) continue; // 左或上越界继续
							if(curX>19 || curY>19) break;  // 右或下越界停止
						}
						if(mode == 4) //向↗检查
						{
							curX = x+cur;
							curY = y-cur;
							if(curX<0 || curY>19) continue; // 左或下越界继续
							if(curX>19 || curY<0) break;  // 右或上越界停止
						}
						
						// 初始化棋子标志，用于判断反色
						if(cur<0 && flgL==0 && chessData[curX][curY]!=0)
							flgL = chessData[curX][curY];

						if(cur>0 && flgR==0 && chessData[curX][curY]!=0)
							flgR = chessData[curX][curY];

						if(cur<0 && chessData[curX][curY] == (-flgL))
						{
							sum1 = -1;		// 左侧搜索到反色，相当于被堵住的棋
							flgL = -flgL;	// 同时标记也取反
						}

						if(cur<0 && chessData[curX][curY] == flgL)
						{
							sum1 ++; // 左侧搜索到同色
						}

						if(cur==-1 && sum1>0 && chessData[curX][curY] == 0)
						{
							sum1 --; // 左一位搜索到空棋
						}
						if(cur==1 && sum2>0 && chessData[curX][curY] == 0)
						{
							sum2 --; // 右一位搜索到空棋
						}

						if(cur>0 && flgL == flgR) // 如果左边的棋子和右边的同色
						{
							sum2 += sum1; // 用sum2代替sum1继续搜索
							sum1 = 0;     // sum1置0以防重复相加和影响后面的cPrior
						}

						if(cur>0 && chessData[curX][curY] == (-flgR))
						{
							sum2 --; // 右边出现反色则，相当于被堵住的棋
							break;   // 不需要继续搜索了
						}

						if(cur>0 && chessData[curX][curY] == flgR)
						{
							sum2 ++;// 右侧搜索到同色
						}
						
					}
					// 长连相对短连拥有绝对的优势
					if(sum1==1) prior = prior + 1;
					if(sum1==2) prior = prior + 10 - 2*flgL; // 数目相同白棋优先
					if(sum1==3) prior = prior + 100 - 20*flgL;
					if(sum1==4) prior = prior + 1000 - 200*flgL;
					if(sum2==1) prior = prior + 1;
					if(sum2==2) prior = prior + 10 - 2*flgR;
					if(sum2==3) prior = prior + 100 - 20*flgR;
					if(sum2==4) prior = prior + 1000 - 200*flgR;
				}

				if(prior>cPrior)
				{
					cPrior = prior;
					cx = x;
					cy = y;
				}

				if(prior==cPrior && rand() < RAND_MAX/5)
				{	// 除第一个棋外很少有优先级相等的点
					cPrior = prior;
					cx = x;
					cy = y;
				}
			}
		}
	}
	PutDown(cx,cy); // 在得到的点下棋
}

// 电脑下棋函数2
void CFiveChessDlg::ComputerDown2(int *tx, int *ty)
{
	int mode = 0, cur = 0, curX = 0, curY = 0, sum1 = 0, sum2 = 0;
	int x = 0, y = 0, cx = 0, cy = 0, cPrior = 0, prior = 0, flgL = 0, flgR = 0;
	for (x = 0;x<20;x++)
	{
		for (y = 0;y<20;y++)
		{	// 扫描全部空白点
			if (chessData[x][y] == 0)
			{
				prior = 0;
				for (mode = 1;mode<5;mode++)	// 模式1为 — 向判断，模式2为 | 向判断
				{							// 模式3为 \ 向判断，模式4为 / 向判断
					sum1 = sum2 = 0;
					flgL = flgR = 0;
					curX = x;
					curY = y; //改变模式后重置变量
					for (cur = -4;cur <= 4;cur++)
					{
						if (mode == 1) //向→检查
						{
							curX = x + cur;
							if (curX<0) continue; //左越界继续
							if (curX>19) break;   //右越界停止
						}
						if (mode == 2) //向↓检查
						{
							curY = y + cur;
							if (curY<0) continue; //上越界继续
							if (curY>19) break;   //下越界停止
						}
						if (mode == 3) //向↘检查
						{
							curX = x + cur;
							curY = y + cur;
							if (curX<0 || curY<0) continue; // 左或上越界继续
							if (curX>19 || curY>19) break;  // 右或下越界停止
						}
						if (mode == 4) //向↗检查
						{
							curX = x + cur;
							curY = y - cur;
							if (curX<0 || curY>19) continue; // 左或下越界继续
							if (curX>19 || curY<0) break;  // 右或上越界停止
						}

						// 初始化棋子标志，用于判断反色
						if (cur<0 && flgL == 0 && chessData[curX][curY] != 0)
							flgL = chessData[curX][curY];

						if (cur>0 && flgR == 0 && chessData[curX][curY] != 0)
							flgR = chessData[curX][curY];

						if (cur<0 && chessData[curX][curY] == (-flgL))
						{
							sum1 = -1;		// 左侧搜索到反色，相当于被堵住的棋
							flgL = -flgL;	// 同时标记也取反
						}

						if (cur<0 && chessData[curX][curY] == flgL)
						{
							sum1++; // 左侧搜索到同色
						}

						if (cur == -1 && sum1>0 && chessData[curX][curY] == 0)
						{
							sum1--; // 左一位搜索到空棋
						}
						if (cur == 1 && sum2>0 && chessData[curX][curY] == 0)
						{
							sum2--; // 右一位搜索到空棋
						}

						if (cur>0 && flgL == flgR) // 如果左边的棋子和右边的同色
						{
							sum2 += sum1; // 用sum2代替sum1继续搜索
							sum1 = 0;     // sum1置0以防重复相加和影响后面的cPrior
						}

						if (cur>0 && chessData[curX][curY] == (-flgR))
						{
							sum2--; // 右边出现反色则，相当于被堵住的棋
							break;   // 不需要继续搜索了
						}

						if (cur>0 && chessData[curX][curY] == flgR)
						{
							sum2++;// 右侧搜索到同色
						}

					}
					// 长连相对短连拥有绝对的优势
					if (sum1 == 1) prior = prior + 1;
					if (sum1 == 2) prior = prior + 10 - 2 * flgL; // 数目相同白棋优先
					if (sum1 == 3) prior = prior + 100 - 20 * flgL;
					if (sum1 == 4) prior = prior + 1000 - 200 * flgL;
					if (sum2 == 1) prior = prior + 1;
					if (sum2 == 2) prior = prior + 10 - 2 * flgR;
					if (sum2 == 3) prior = prior + 100 - 20 * flgR;
					if (sum2 == 4) prior = prior + 1000 - 200 * flgR;
				}

				if (prior>cPrior)
				{
					cPrior = prior;
					cx = x;
					cy = y;
				}

				if (prior == cPrior && rand() < RAND_MAX / 5)
				{	// 除第一个棋外很少有优先级相等的点
					cPrior = prior;
					cx = x;
					cy = y;
				}
			}
		}
	}
	*tx = cx;
	*ty = cy;
}
// 人/人联网游戏按钮
void CFiveChessDlg::OnBnClickedInternetGame()
{
	CInternetDlg intDlg;
	intDlg.isHost = isHost;
	intDlg.isBlack = isFirst;
	intDlg.isLink = isLink;
	if(intDlg.DoModal()==IDOK)
	{
		if(intDlg.isHost) //选择主机
		{
			if(!isLink) //未连接才监听
			{	sckHost.Create(intDlg.netPort);
				sckHost.Listen();
				isLink = true;  
			}
			isFirst = intDlg.isBlack; //主机可以决定先手
		}
		if(!intDlg.isHost && !isLink) //选择客户
		{	
			sckClient.Create();
			sckClient.Connect(intDlg.ipAddress,intDlg.netPort);
			isLink = true;
			isFirst = false; //避免客户端连接前出现先手
		}
		isHost = intDlg.isHost;
		gameMode = 3;
	}
}


// 机/机联网游戏按钮
void CFiveChessDlg::OnBnClickedInternetGame2()
{
	CInternetDlg intDlg;
	intDlg.isHost = isHost;
	intDlg.isBlack = isFirst;
	intDlg.isLink = isLink;
	if (intDlg.DoModal() == IDOK)
	{
		if (intDlg.isHost) //选择主机
		{
			if (!isLink) //未连接才监听
			{
				sckHost.Create(intDlg.netPort);
				sckHost.Listen();
				isLink = true;
			}
			isFirst = intDlg.isBlack; //主机可以决定先手
		}
		if (!intDlg.isHost && !isLink) //选择客户
		{
			sckClient.Create();
			sckClient.Connect(intDlg.ipAddress, intDlg.netPort);
			isLink = true;
			isFirst = false; //避免客户端连接前出现先手
		}
		isHost = intDlg.isHost;
		gameMode = 4;
	}

}

// 悔棋按钮函数
void CFiveChessDlg::OnBnClickedGoBack()
{
	if(goBack[0]==0)
	{
		MessageBox("没有可悔的棋步了。","温馨提示",MB_OK|MB_ICONINFORMATION);
		return;
	}

	if(gameMode==3 && isLink)
	{
		int send[3] = { 4, 0, 0 }; //4,0表示发起悔棋 4,1为同意 4,0,1为拒绝
		sckClient.Send((LPVOID)send,sizeof(send));
		GetDlgItem(IDC_GO_BACK)->EnableWindow(FALSE);
		return;
	}
	GoBack();
}

// 退出游戏按钮
void CFiveChessDlg::OnBnClickedExitGame()
{
	bool close = true;
	for(int i=0;i<20;i++)
		for(int j=0;j<20;j++)
		{	// 如果棋盘上有棋子则进行提示
			if(close && chessData[i][j]!=0)
				if(MessageBox("游戏已经开始！\n是否结束当前游戏？", 
					"温馨提示",MB_YESNO|MB_ICONQUESTION)==IDYES)
					close = false;
				else return; // 点击“否”结束函数
		}
	OnOK();
}

// 截获WM_CLOSE消息
void CFiveChessDlg::OnClose()
{	// 游戏已经开始则需确认后才退出
	OnBnClickedExitGame();
}

// 判断游戏是否结束
void CFiveChessDlg::IsOver(int x, int y)
{	
	if(chessData[x][y]==1) //如果为黑棋则进行禁手判断
	{
		CString str;
		int fbd = IsForbid(x, y);
		if(fbd > 0 )
		{
			if(fbd == 33) str = "三三";
			if(fbd == 44) str = "四四";
			if(fbd == 6 ) str = "长连";
			if(MessageBox("黑棋"+str+"禁手判输！\n是否要重新开始游戏？", 
				"游戏结束",MB_YESNO|MB_ICONINFORMATION)==IDYES)
				RestartGame(false); //false表示在RestartGame()中不再提示
		}
	}
	int cur,res,curX,curY,mode;
	//分四个方向从当前棋子的前四个检查到当前棋子的后四个
	for(mode=1;mode<=4;mode++) //依次改变检查模式
	{
		res = 0;
		curX = x;
		curY = y; //改变模式后重置变量
		for(cur=-4;cur<=4;cur++)
		{
			if(mode == 1) //向→检查
			{
				curX = x+cur;
				if(curX<0) continue; //左越界继续
				if(curX>19) break;   //右越界停止
			}
			if(mode == 2) //向↓检查
			{
				curY = y+cur;
				if(curY<0) continue; //上越界继续
				if(curY>19) break;   //下越界停止
			}
			if(mode == 3) //向↘检查
			{
				curX = x+cur;
				curY = y+cur;
				if(curX<0 || curY<0) continue; //左或上越界继续
				if(curX>19 || curY>19) break;  //右或下越界停止
			}
			if(mode == 4) //向↗检查
			{
				curX = x+cur;
				curY = y-cur;
				if(curX<0 || curY>19) continue; //左或下越界继续
				if(curX>19 || curY<0) break;  //右或上越界停止
			}
			//依次遇到相同棋子则进行累加
			if(chessData[curX][curY]==chessData[x][y])
			{
				res += chessData[curX][curY];
				if(res == 5)
				{
					if(MessageBox("恭喜黑棋胜利！\n是否重新开始游戏？", 
						"游戏结束",MB_YESNO|MB_ICONINFORMATION)==IDYES)
					RestartGame(false); //false表示在RestartGame()中不再提示
					break;
				}
				if(res == -5)
				{
					if(MessageBox("恭喜白棋胜利！\n是否重新开始游戏？", 
						"游戏结束",MB_YESNO|MB_ICONINFORMATION)==IDYES)
					RestartGame(false);
					break;
				}
			} //在当前方向上遇到不同棋子则重新计数
			else res = 0;
		}
	}
}

// 禁手判断函数
int CFiveChessDlg::IsForbid(int x, int y)
{	//针对执黑先手优势制定的黑棋禁手，白棋无禁手

	int mode=0,f3=0,flg3=0,f4=0,flg4=0,fline=0;
	int cur=0,curX=x,curY=y,i=0,j=0,line[11]={0};

	for(mode=1;mode<5;mode++)	//模式1为 — 向判断，模式2为 | 向判断
	{							//模式3为 \ 向判断，模式4为 / 向判断
		flg3 = 0;
		flg4 = 0;
		curX = x;
		curY = y;	//改变模式后重置
		for(i=0;i<11;i++)
			line[i] = -10; // 如果该值未被改变，则对应点越界
		for(cur=-5;cur<=5;cur++)
		{
			if(mode == 1) //向→检查
			{
				curX = x+cur;
				if(curX<0) continue; //左越界继续
				if(curX>19) break;   //右越界停止
			}
			if(mode == 2) //向↓检查
			{
				curY = y+cur;
				if(curY<0) continue; //上越界继续
				if(curY>19) break;   //下越界停止
			}
			if(mode == 3) //向↘检查
			{
				curX = x+cur;
				curY = y+cur;
				if(curX<0 || curY<0) continue; // 左或上越界继续
				if(curX>19 || curY>19) break;  // 右或下越界停止
			}
			if(mode == 4) //向↗检查
			{
				curX = x+cur;
				curY = y-cur;
				if(curX<0 || curY>19) continue; // 左或下越界继续
				if(curX>19 || curY<0) break;  // 右或上越界停止
			}
			line[cur+5] = chessData[curX][curY]; //保存该条线上的棋子
		}

		for(i=0;i<4;i++) //判断三三禁手
		{
			for(j=i;j<i+8;j++)
			{	 //棋子分布判断
				if(line[j]<1 && line[j+1]==0 && line[j+6]==0 && line[j+7]<1
					&& line[j+2]+line[j+3]+line[j+4]+line[j+5]==3)
					flg3 = 1; //符合三三禁手的条件之一,使用标志防止对称性重复
			}
		}

		for(i=0;i<5;i++) //判断四四禁手
		{
			for(j=i;j<i+7;j++)
			{
				if(line[j]<1 && line[j+6]<1 //棋子分布判断
					&& line[j+1]+line[j+2]+line[j+3]+line[j+4]+line[j+5]==4)
				{
					f3 = -5; //符合四四禁手的条件之一,取消所有三三禁手
					flg4 = 1;
				}
			}
		}

		line[5]=-1; //假设落子点为白棋
		for(i=0;i<11;i++) //判断是否为堵五连
		{
			if(fline==5) return -5;
			if(line[i]==-1) fline++;
			else fline = 0;
		}

		line[5]=1; //落子点恢复黑棋
		for(i=0;i<11;i++) //判断是否长连
		{
			if(fline>5) return 6;
			if(line[i]==1) fline++;
			else fline = 0;
		}

		f3 += flg3; //加上禁手标志
		f4 += flg4;
	}

	if(f4>1) return 44;
	if(f3>1) return 33;
	return 0;
}

// 在给定位置下棋
void CFiveChessDlg::PutDown(int x, int y)
{	
	//根据isBlack保存棋盘数据并绘制棋子
	chessData[x][y] = isBlack * 2 - 1;
	imgList.Draw(GetDC(),isBlack,CPoint(x*25+13,y*25+198),ILD_TRANSPARENT);

	goBack[0]++; //保存悔棋相关信息
	goBack[goBack[0]*2-1] = x;
	goBack[goBack[0]*2] = y;

	MessageBeep(MB_ICONASTERISK); //成功提示音
	isBlack = ! isBlack; //改变下次放棋子的颜色
	IsOver(x,y); //每次下棋后都要判断是否胜利
}

// 联网游戏消息响应
LRESULT CFiveChessDlg::OnMySocket(WPARAM wParam,LPARAM lParam)
{
	switch(wParam)
	{
	case MS_ACCEPT: //服务器有接受连接的消息
		{
			int send[3]={2,0,0};
			sckHost.Accept(sckClient); //接受连接
			sckHost.Close(); //开始下棋不再监听
			sckClient.Send((LPVOID)send,sizeof(send));
			MessageBox("客户端已经接入！\n可以开始下棋了。", 
					"温馨提示",MB_ICONINFORMATION);
			RestartGame(false);
		} break;
	case MS_RECEIVE: //双方都有接受数据的消息
		{
			int recv[3]={0}; //消息数据
			sckClient.Receive((LPVOID)recv,sizeof(recv)); //用连接的SOCKET接收
			if(recv[0] < 2) //小于2表示落子信息
			{
				PutDown(recv[1],recv[2]);
				isTurn = true;
			}
			if(recv[0] == 2) //2表示连接成功
			{
				MessageBox("连接服务器成功！\n可以开始下棋了。", 
						"温馨提示",MB_ICONINFORMATION);
				RestartGame(false);
			}

			if(recv[0] == 3) //3表示初始先手信息
			{	isFirst = recv[1];	}
			if(recv[0] == 4) //4表示请求悔棋
			{
				if(recv[1]==1) //对方同意悔棋
				{
					GoBack(); //执行悔棋
					GetDlgItem(IDC_GO_BACK)->EnableWindow(TRUE);
				}
				if(recv[2]==1) //对方拒绝悔棋
				{
					GetDlgItem(IDC_GO_BACK)->EnableWindow(TRUE);
					MessageBox("对方拒绝了悔棋！", 
						"温馨提示",MB_ICONINFORMATION);
				}
				if(recv[1]==0 && recv[2]==0) //处理悔棋请求
				{
					if(MessageBox("对方请求悔棋！\n是否同意悔棋？", 
						"温馨提示",MB_YESNO|MB_ICONQUESTION)==IDYES)
					{
						recv[1] = 1; //同意悔棋
						GoBack(); //执行悔棋
					}
					else recv[2] = 1; //拒绝悔棋
					sckClient.Send((LPVOID)recv,sizeof(recv));
				}
			}
			if(recv[0] == 5) //5表示对方已经重玩
			{
				MessageBox("对方重新开始了游戏！","温馨提示",MB_ICONINFORMATION);
			}
		} break;
	case MS_CLOSE: //关闭连接的消息
		{
			sckClient.Close();
			MessageBox("对方离开了游戏！\n那就下次再玩吧。", 
				"温馨提示",MB_ICONINFORMATION);
		} break;
	}
	return 0;
}

// 执行悔棋函数
void CFiveChessDlg::GoBack()
{
	MessageBeep(MB_ICONASTERISK);
	chessData[goBack[goBack[0]*2-1]][goBack[goBack[0]*2]] = 0;
	isBlack = ! isBlack;
	if(gameMode==3 && isLink) isTurn = !isTurn;
	goBack[0]--; // 保存的棋步减少
	Invalidate();// 悔棋后需要重绘窗口
}

//关于按钮
void CFiveChessDlg::OnBnClickedAbout()
{
	About about;
	about.DoModal();
}
