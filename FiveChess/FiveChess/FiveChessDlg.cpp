
// FiveChessDlg.cpp : ʵ���ļ�
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


// CFiveChessDlg �Ի���


CFiveChessDlg::CFiveChessDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CFiveChessDlg::IDD, pParent)
{
	gameMode = 1; //��ʼ��Ϸģʽ
	isBlack = true;
	isTurn = true;
	isHost = true;
	isLink = false;
	isFirst = true;
	CBitmap White; //��ʼ�����ͼƬ
	CBitmap Black;
	Black.LoadBitmap(IDB_BLACK);
	White.LoadBitmap(IDB_WHITE);
	imgList.Create(24,24,ILC_COLOR24|ILC_MASK,0,2);
	imgList.Add(&White,0xff00ff); //ʹ������ȥ��λͼ����Ҫ͸���Ĳ���
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

// CFiveChessDlg ��Ϣ�������

BOOL CFiveChessDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	RestartGame(false); // ��ʼ��Ϸ����ʾ
	AfxSocketInit(); //��ʼ���׽��ֿ�

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ

void CFiveChessDlg::OnPaint()
{
	CPaintDC dc(this);
	// ��������ͼƬ
	CBitmap board;
	board.LoadBitmap(IDB_BOARD);
    CDC memdc;
	memdc.CreateCompatibleDC(&dc);
	memdc.SelectObject(&board);
	dc.BitBlt(0,10,526,727,&memdc,1,0,SRCCOPY);
	ReleaseDC(&memdc);

	for(int i=0;i<20;i++) // ����������Ϣ
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

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// �ػ�WM_ERASEBKGND��Ϣ�Է���˸
BOOL CFiveChessDlg::OnEraseBkgnd(CDC* pDC)
{	// ���������������ػ�ʱ��ԭ�����ݸ���
	return TRUE;
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù����ʾ
HCURSOR CFiveChessDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

// ��ʼ��Ϸ��ť
void CFiveChessDlg::OnBnClickedStartGame()
{
	gameMode = 1;
	RestartGame(true);
}

// �˻���ս��ť
void CFiveChessDlg::OnBnClickedStartComputer()
{
	gameMode = 2;
	RestartGame(true);
}

// ���¿�ʼ��ť
void CFiveChessDlg::OnBnClickedRestartGame()
{	
	RestartGame(true);
}

// ���¿�ʼ��Ϸ
void CFiveChessDlg::RestartGame(bool isWarm)
{
	for(int i=0;i<20;i++)
		for(int j=0;j<20;j++)
		{	// ��������������������isWarm������ʾ
			if(isWarm && chessData[i][j]!=0)
			{
				if(MessageBox("��Ϸ�Ѿ���ʼ��\n�Ƿ����¿�ʼ��Ϸ��","��ܰ��ʾ",
					MB_YESNO|MB_ICONQUESTION)==IDYES)
				{
					isWarm = false; //�����ظ���ʾ
					if(gameMode==3 && isLink)
					{
						int send[3] = { 5, 0, 0 }; //5��ʾ����
						sckClient.Send((LPVOID)send,sizeof(send));
					}
				}
				else	return; // ������񡱽�������

			}
			chessData[i][j] = 0;
		}
	for(int k=0;k<451;k++) // ���û�����Ϣ
		goBack[k] = 0;

	if(gameMode==3 && isLink && isHost)
	{		//���߶Է������෴������״̬
		int send[3] = { 3, !isFirst, 0 }; // 3��ʾ��ʼ������Ϣ
		sckClient.Send((LPVOID)send,sizeof(send));
	}
	isTurn = isFirst;
	isBlack = true; // Ĭ�Ϻ���Ϊ����
	Invalidate(); // ���»��ƴ�����Ϣ
}

// ʹ���������
void CFiveChessDlg::OnLButtonUp(UINT nFlags, CPoint point)
{
	// �������λ��ת�����������ӵ�λ��
	int cx = (point.x-13)/25;
	int cy = (point.y-198)/25;
	 
	if(point.x>13 && point.x<513 && point.y>198 && point.y<698 && chessData[cx][cy]==0)
	{
		if(gameMode == 1) // ˫�˱�����Ϸ
		{
			PutDown(cx, cy);
		}

		if(gameMode == 2) // �����˻���Ϸ
		{	// Ĭ�����ʹ�ú���
			if(isBlack) PutDown(cx, cy);
			if(!isBlack) ComputerDown();
		}
		if(gameMode == 3 && isTurn) // ˫��������Ϸ
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
	else MessageBeep(MB_OK); // ����������
	CDialogEx::OnLButtonUp(nFlags, point);
}

// �������庯��
void CFiveChessDlg::ComputerDown()
{
	int mode=0,cur=0,curX=0,curY=0,sum1=0,sum2=0;
	int x=0,y=0,cx=0,cy=0,cPrior=0,prior=0,flgL=0,flgR=0;
	for(x=0;x<20;x++)
	{
		for(y=0;y<20;y++)
		{	// ɨ��ȫ���հ׵�
			if(chessData[x][y]==0)
			{
				prior = 0;
				for(mode=1;mode<5;mode++)	// ģʽ1Ϊ �� ���жϣ�ģʽ2Ϊ | ���ж�
				{							// ģʽ3Ϊ \ ���жϣ�ģʽ4Ϊ / ���ж�
					sum1 = sum2 = 0;
					flgL = flgR = 0;
					curX = x;
					curY = y; //�ı�ģʽ�����ñ���
					for(cur=-4;cur<=4;cur++)
					{
						if(mode == 1) //������
						{
							curX = x+cur;
							if(curX<0) continue; //��Խ�����
							if(curX>19) break;   //��Խ��ֹͣ
						}
						if(mode == 2) //������
						{
							curY = y+cur;
							if(curY<0) continue; //��Խ�����
							if(curY>19) break;   //��Խ��ֹͣ
						}
						if(mode == 3) //��K���
						{
							curX = x+cur;
							curY = y+cur;
							if(curX<0 || curY<0) continue; // �����Խ�����
							if(curX>19 || curY>19) break;  // �һ���Խ��ֹͣ
						}
						if(mode == 4) //��J���
						{
							curX = x+cur;
							curY = y-cur;
							if(curX<0 || curY>19) continue; // �����Խ�����
							if(curX>19 || curY<0) break;  // �һ���Խ��ֹͣ
						}
						
						// ��ʼ�����ӱ�־�������жϷ�ɫ
						if(cur<0 && flgL==0 && chessData[curX][curY]!=0)
							flgL = chessData[curX][curY];

						if(cur>0 && flgR==0 && chessData[curX][curY]!=0)
							flgR = chessData[curX][curY];

						if(cur<0 && chessData[curX][curY] == (-flgL))
						{
							sum1 = -1;		// �����������ɫ���൱�ڱ���ס����
							flgL = -flgL;	// ͬʱ���Ҳȡ��
						}

						if(cur<0 && chessData[curX][curY] == flgL)
						{
							sum1 ++; // ���������ͬɫ
						}

						if(cur==-1 && sum1>0 && chessData[curX][curY] == 0)
						{
							sum1 --; // ��һλ����������
						}
						if(cur==1 && sum2>0 && chessData[curX][curY] == 0)
						{
							sum2 --; // ��һλ����������
						}

						if(cur>0 && flgL == flgR) // �����ߵ����Ӻ��ұߵ�ͬɫ
						{
							sum2 += sum1; // ��sum2����sum1��������
							sum1 = 0;     // sum1��0�Է��ظ���Ӻ�Ӱ������cPrior
						}

						if(cur>0 && chessData[curX][curY] == (-flgR))
						{
							sum2 --; // �ұ߳��ַ�ɫ���൱�ڱ���ס����
							break;   // ����Ҫ����������
						}

						if(cur>0 && chessData[curX][curY] == flgR)
						{
							sum2 ++;// �Ҳ�������ͬɫ
						}
						
					}
					// ������Զ���ӵ�о��Ե�����
					if(sum1==1) prior = prior + 1;
					if(sum1==2) prior = prior + 10 - 2*flgL; // ��Ŀ��ͬ��������
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
				{	// ����һ��������������ȼ���ȵĵ�
					cPrior = prior;
					cx = x;
					cy = y;
				}
			}
		}
	}
	PutDown(cx,cy); // �ڵõ��ĵ�����
}

// �������庯��2
void CFiveChessDlg::ComputerDown2(int *tx, int *ty)
{
	int mode = 0, cur = 0, curX = 0, curY = 0, sum1 = 0, sum2 = 0;
	int x = 0, y = 0, cx = 0, cy = 0, cPrior = 0, prior = 0, flgL = 0, flgR = 0;
	for (x = 0;x<20;x++)
	{
		for (y = 0;y<20;y++)
		{	// ɨ��ȫ���հ׵�
			if (chessData[x][y] == 0)
			{
				prior = 0;
				for (mode = 1;mode<5;mode++)	// ģʽ1Ϊ �� ���жϣ�ģʽ2Ϊ | ���ж�
				{							// ģʽ3Ϊ \ ���жϣ�ģʽ4Ϊ / ���ж�
					sum1 = sum2 = 0;
					flgL = flgR = 0;
					curX = x;
					curY = y; //�ı�ģʽ�����ñ���
					for (cur = -4;cur <= 4;cur++)
					{
						if (mode == 1) //������
						{
							curX = x + cur;
							if (curX<0) continue; //��Խ�����
							if (curX>19) break;   //��Խ��ֹͣ
						}
						if (mode == 2) //������
						{
							curY = y + cur;
							if (curY<0) continue; //��Խ�����
							if (curY>19) break;   //��Խ��ֹͣ
						}
						if (mode == 3) //��K���
						{
							curX = x + cur;
							curY = y + cur;
							if (curX<0 || curY<0) continue; // �����Խ�����
							if (curX>19 || curY>19) break;  // �һ���Խ��ֹͣ
						}
						if (mode == 4) //��J���
						{
							curX = x + cur;
							curY = y - cur;
							if (curX<0 || curY>19) continue; // �����Խ�����
							if (curX>19 || curY<0) break;  // �һ���Խ��ֹͣ
						}

						// ��ʼ�����ӱ�־�������жϷ�ɫ
						if (cur<0 && flgL == 0 && chessData[curX][curY] != 0)
							flgL = chessData[curX][curY];

						if (cur>0 && flgR == 0 && chessData[curX][curY] != 0)
							flgR = chessData[curX][curY];

						if (cur<0 && chessData[curX][curY] == (-flgL))
						{
							sum1 = -1;		// �����������ɫ���൱�ڱ���ס����
							flgL = -flgL;	// ͬʱ���Ҳȡ��
						}

						if (cur<0 && chessData[curX][curY] == flgL)
						{
							sum1++; // ���������ͬɫ
						}

						if (cur == -1 && sum1>0 && chessData[curX][curY] == 0)
						{
							sum1--; // ��һλ����������
						}
						if (cur == 1 && sum2>0 && chessData[curX][curY] == 0)
						{
							sum2--; // ��һλ����������
						}

						if (cur>0 && flgL == flgR) // �����ߵ����Ӻ��ұߵ�ͬɫ
						{
							sum2 += sum1; // ��sum2����sum1��������
							sum1 = 0;     // sum1��0�Է��ظ���Ӻ�Ӱ������cPrior
						}

						if (cur>0 && chessData[curX][curY] == (-flgR))
						{
							sum2--; // �ұ߳��ַ�ɫ���൱�ڱ���ס����
							break;   // ����Ҫ����������
						}

						if (cur>0 && chessData[curX][curY] == flgR)
						{
							sum2++;// �Ҳ�������ͬɫ
						}

					}
					// ������Զ���ӵ�о��Ե�����
					if (sum1 == 1) prior = prior + 1;
					if (sum1 == 2) prior = prior + 10 - 2 * flgL; // ��Ŀ��ͬ��������
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
				{	// ����һ��������������ȼ���ȵĵ�
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
// ��/��������Ϸ��ť
void CFiveChessDlg::OnBnClickedInternetGame()
{
	CInternetDlg intDlg;
	intDlg.isHost = isHost;
	intDlg.isBlack = isFirst;
	intDlg.isLink = isLink;
	if(intDlg.DoModal()==IDOK)
	{
		if(intDlg.isHost) //ѡ������
		{
			if(!isLink) //δ���Ӳż���
			{	sckHost.Create(intDlg.netPort);
				sckHost.Listen();
				isLink = true;  
			}
			isFirst = intDlg.isBlack; //�������Ծ�������
		}
		if(!intDlg.isHost && !isLink) //ѡ��ͻ�
		{	
			sckClient.Create();
			sckClient.Connect(intDlg.ipAddress,intDlg.netPort);
			isLink = true;
			isFirst = false; //����ͻ�������ǰ��������
		}
		isHost = intDlg.isHost;
		gameMode = 3;
	}
}


// ��/��������Ϸ��ť
void CFiveChessDlg::OnBnClickedInternetGame2()
{
	CInternetDlg intDlg;
	intDlg.isHost = isHost;
	intDlg.isBlack = isFirst;
	intDlg.isLink = isLink;
	if (intDlg.DoModal() == IDOK)
	{
		if (intDlg.isHost) //ѡ������
		{
			if (!isLink) //δ���Ӳż���
			{
				sckHost.Create(intDlg.netPort);
				sckHost.Listen();
				isLink = true;
			}
			isFirst = intDlg.isBlack; //�������Ծ�������
		}
		if (!intDlg.isHost && !isLink) //ѡ��ͻ�
		{
			sckClient.Create();
			sckClient.Connect(intDlg.ipAddress, intDlg.netPort);
			isLink = true;
			isFirst = false; //����ͻ�������ǰ��������
		}
		isHost = intDlg.isHost;
		gameMode = 4;
	}

}

// ���尴ť����
void CFiveChessDlg::OnBnClickedGoBack()
{
	if(goBack[0]==0)
	{
		MessageBox("û�пɻڵ��岽�ˡ�","��ܰ��ʾ",MB_OK|MB_ICONINFORMATION);
		return;
	}

	if(gameMode==3 && isLink)
	{
		int send[3] = { 4, 0, 0 }; //4,0��ʾ������� 4,1Ϊͬ�� 4,0,1Ϊ�ܾ�
		sckClient.Send((LPVOID)send,sizeof(send));
		GetDlgItem(IDC_GO_BACK)->EnableWindow(FALSE);
		return;
	}
	GoBack();
}

// �˳���Ϸ��ť
void CFiveChessDlg::OnBnClickedExitGame()
{
	bool close = true;
	for(int i=0;i<20;i++)
		for(int j=0;j<20;j++)
		{	// ����������������������ʾ
			if(close && chessData[i][j]!=0)
				if(MessageBox("��Ϸ�Ѿ���ʼ��\n�Ƿ������ǰ��Ϸ��", 
					"��ܰ��ʾ",MB_YESNO|MB_ICONQUESTION)==IDYES)
					close = false;
				else return; // ������񡱽�������
		}
	OnOK();
}

// �ػ�WM_CLOSE��Ϣ
void CFiveChessDlg::OnClose()
{	// ��Ϸ�Ѿ���ʼ����ȷ�Ϻ���˳�
	OnBnClickedExitGame();
}

// �ж���Ϸ�Ƿ����
void CFiveChessDlg::IsOver(int x, int y)
{	
	if(chessData[x][y]==1) //���Ϊ��������н����ж�
	{
		CString str;
		int fbd = IsForbid(x, y);
		if(fbd > 0 )
		{
			if(fbd == 33) str = "����";
			if(fbd == 44) str = "����";
			if(fbd == 6 ) str = "����";
			if(MessageBox("����"+str+"�������䣡\n�Ƿ�Ҫ���¿�ʼ��Ϸ��", 
				"��Ϸ����",MB_YESNO|MB_ICONINFORMATION)==IDYES)
				RestartGame(false); //false��ʾ��RestartGame()�в�����ʾ
		}
	}
	int cur,res,curX,curY,mode;
	//���ĸ�����ӵ�ǰ���ӵ�ǰ�ĸ���鵽��ǰ���ӵĺ��ĸ�
	for(mode=1;mode<=4;mode++) //���θı���ģʽ
	{
		res = 0;
		curX = x;
		curY = y; //�ı�ģʽ�����ñ���
		for(cur=-4;cur<=4;cur++)
		{
			if(mode == 1) //������
			{
				curX = x+cur;
				if(curX<0) continue; //��Խ�����
				if(curX>19) break;   //��Խ��ֹͣ
			}
			if(mode == 2) //������
			{
				curY = y+cur;
				if(curY<0) continue; //��Խ�����
				if(curY>19) break;   //��Խ��ֹͣ
			}
			if(mode == 3) //��K���
			{
				curX = x+cur;
				curY = y+cur;
				if(curX<0 || curY<0) continue; //�����Խ�����
				if(curX>19 || curY>19) break;  //�һ���Խ��ֹͣ
			}
			if(mode == 4) //��J���
			{
				curX = x+cur;
				curY = y-cur;
				if(curX<0 || curY>19) continue; //�����Խ�����
				if(curX>19 || curY<0) break;  //�һ���Խ��ֹͣ
			}
			//����������ͬ����������ۼ�
			if(chessData[curX][curY]==chessData[x][y])
			{
				res += chessData[curX][curY];
				if(res == 5)
				{
					if(MessageBox("��ϲ����ʤ����\n�Ƿ����¿�ʼ��Ϸ��", 
						"��Ϸ����",MB_YESNO|MB_ICONINFORMATION)==IDYES)
					RestartGame(false); //false��ʾ��RestartGame()�в�����ʾ
					break;
				}
				if(res == -5)
				{
					if(MessageBox("��ϲ����ʤ����\n�Ƿ����¿�ʼ��Ϸ��", 
						"��Ϸ����",MB_YESNO|MB_ICONINFORMATION)==IDYES)
					RestartGame(false);
					break;
				}
			} //�ڵ�ǰ������������ͬ���������¼���
			else res = 0;
		}
	}
}

// �����жϺ���
int CFiveChessDlg::IsForbid(int x, int y)
{	//���ִ�����������ƶ��ĺ�����֣������޽���

	int mode=0,f3=0,flg3=0,f4=0,flg4=0,fline=0;
	int cur=0,curX=x,curY=y,i=0,j=0,line[11]={0};

	for(mode=1;mode<5;mode++)	//ģʽ1Ϊ �� ���жϣ�ģʽ2Ϊ | ���ж�
	{							//ģʽ3Ϊ \ ���жϣ�ģʽ4Ϊ / ���ж�
		flg3 = 0;
		flg4 = 0;
		curX = x;
		curY = y;	//�ı�ģʽ������
		for(i=0;i<11;i++)
			line[i] = -10; // �����ֵδ���ı䣬���Ӧ��Խ��
		for(cur=-5;cur<=5;cur++)
		{
			if(mode == 1) //������
			{
				curX = x+cur;
				if(curX<0) continue; //��Խ�����
				if(curX>19) break;   //��Խ��ֹͣ
			}
			if(mode == 2) //������
			{
				curY = y+cur;
				if(curY<0) continue; //��Խ�����
				if(curY>19) break;   //��Խ��ֹͣ
			}
			if(mode == 3) //��K���
			{
				curX = x+cur;
				curY = y+cur;
				if(curX<0 || curY<0) continue; // �����Խ�����
				if(curX>19 || curY>19) break;  // �һ���Խ��ֹͣ
			}
			if(mode == 4) //��J���
			{
				curX = x+cur;
				curY = y-cur;
				if(curX<0 || curY>19) continue; // �����Խ�����
				if(curX>19 || curY<0) break;  // �һ���Խ��ֹͣ
			}
			line[cur+5] = chessData[curX][curY]; //����������ϵ�����
		}

		for(i=0;i<4;i++) //�ж���������
		{
			for(j=i;j<i+8;j++)
			{	 //���ӷֲ��ж�
				if(line[j]<1 && line[j+1]==0 && line[j+6]==0 && line[j+7]<1
					&& line[j+2]+line[j+3]+line[j+4]+line[j+5]==3)
					flg3 = 1; //�����������ֵ�����֮һ,ʹ�ñ�־��ֹ�Գ����ظ�
			}
		}

		for(i=0;i<5;i++) //�ж����Ľ���
		{
			for(j=i;j<i+7;j++)
			{
				if(line[j]<1 && line[j+6]<1 //���ӷֲ��ж�
					&& line[j+1]+line[j+2]+line[j+3]+line[j+4]+line[j+5]==4)
				{
					f3 = -5; //�������Ľ��ֵ�����֮һ,ȡ��������������
					flg4 = 1;
				}
			}
		}

		line[5]=-1; //�������ӵ�Ϊ����
		for(i=0;i<11;i++) //�ж��Ƿ�Ϊ������
		{
			if(fline==5) return -5;
			if(line[i]==-1) fline++;
			else fline = 0;
		}

		line[5]=1; //���ӵ�ָ�����
		for(i=0;i<11;i++) //�ж��Ƿ���
		{
			if(fline>5) return 6;
			if(line[i]==1) fline++;
			else fline = 0;
		}

		f3 += flg3; //���Ͻ��ֱ�־
		f4 += flg4;
	}

	if(f4>1) return 44;
	if(f3>1) return 33;
	return 0;
}

// �ڸ���λ������
void CFiveChessDlg::PutDown(int x, int y)
{	
	//����isBlack�����������ݲ���������
	chessData[x][y] = isBlack * 2 - 1;
	imgList.Draw(GetDC(),isBlack,CPoint(x*25+13,y*25+198),ILD_TRANSPARENT);

	goBack[0]++; //������������Ϣ
	goBack[goBack[0]*2-1] = x;
	goBack[goBack[0]*2] = y;

	MessageBeep(MB_ICONASTERISK); //�ɹ���ʾ��
	isBlack = ! isBlack; //�ı��´η����ӵ���ɫ
	IsOver(x,y); //ÿ�������Ҫ�ж��Ƿ�ʤ��
}

// ������Ϸ��Ϣ��Ӧ
LRESULT CFiveChessDlg::OnMySocket(WPARAM wParam,LPARAM lParam)
{
	switch(wParam)
	{
	case MS_ACCEPT: //�������н������ӵ���Ϣ
		{
			int send[3]={2,0,0};
			sckHost.Accept(sckClient); //��������
			sckHost.Close(); //��ʼ���岻�ټ���
			sckClient.Send((LPVOID)send,sizeof(send));
			MessageBox("�ͻ����Ѿ����룡\n���Կ�ʼ�����ˡ�", 
					"��ܰ��ʾ",MB_ICONINFORMATION);
			RestartGame(false);
		} break;
	case MS_RECEIVE: //˫�����н������ݵ���Ϣ
		{
			int recv[3]={0}; //��Ϣ����
			sckClient.Receive((LPVOID)recv,sizeof(recv)); //�����ӵ�SOCKET����
			if(recv[0] < 2) //С��2��ʾ������Ϣ
			{
				PutDown(recv[1],recv[2]);
				isTurn = true;
			}
			if(recv[0] == 2) //2��ʾ���ӳɹ�
			{
				MessageBox("���ӷ������ɹ���\n���Կ�ʼ�����ˡ�", 
						"��ܰ��ʾ",MB_ICONINFORMATION);
				RestartGame(false);
			}

			if(recv[0] == 3) //3��ʾ��ʼ������Ϣ
			{	isFirst = recv[1];	}
			if(recv[0] == 4) //4��ʾ�������
			{
				if(recv[1]==1) //�Է�ͬ�����
				{
					GoBack(); //ִ�л���
					GetDlgItem(IDC_GO_BACK)->EnableWindow(TRUE);
				}
				if(recv[2]==1) //�Է��ܾ�����
				{
					GetDlgItem(IDC_GO_BACK)->EnableWindow(TRUE);
					MessageBox("�Է��ܾ��˻��壡", 
						"��ܰ��ʾ",MB_ICONINFORMATION);
				}
				if(recv[1]==0 && recv[2]==0) //�����������
				{
					if(MessageBox("�Է�������壡\n�Ƿ�ͬ����壿", 
						"��ܰ��ʾ",MB_YESNO|MB_ICONQUESTION)==IDYES)
					{
						recv[1] = 1; //ͬ�����
						GoBack(); //ִ�л���
					}
					else recv[2] = 1; //�ܾ�����
					sckClient.Send((LPVOID)recv,sizeof(recv));
				}
			}
			if(recv[0] == 5) //5��ʾ�Է��Ѿ�����
			{
				MessageBox("�Է����¿�ʼ����Ϸ��","��ܰ��ʾ",MB_ICONINFORMATION);
			}
		} break;
	case MS_CLOSE: //�ر����ӵ���Ϣ
		{
			sckClient.Close();
			MessageBox("�Է��뿪����Ϸ��\n�Ǿ��´�����ɡ�", 
				"��ܰ��ʾ",MB_ICONINFORMATION);
		} break;
	}
	return 0;
}

// ִ�л��庯��
void CFiveChessDlg::GoBack()
{
	MessageBeep(MB_ICONASTERISK);
	chessData[goBack[goBack[0]*2-1]][goBack[goBack[0]*2]] = 0;
	isBlack = ! isBlack;
	if(gameMode==3 && isLink) isTurn = !isTurn;
	goBack[0]--; // ������岽����
	Invalidate();// �������Ҫ�ػ洰��
}

//���ڰ�ť
void CFiveChessDlg::OnBnClickedAbout()
{
	About about;
	about.DoModal();
}
