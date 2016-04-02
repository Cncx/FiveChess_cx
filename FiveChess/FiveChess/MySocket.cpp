// MySocket.cpp : 实现文件
//

#include "stdafx.h"
#include "FiveChess.h"
#include "MySocket.h"

// CMySocket 网络通信类

CMySocket::CMySocket()
{
}

CMySocket::~CMySocket()
{
}

// CMySocket 成员函数

void CMySocket::OnAccept( int nErrorCode )
{	//将消息交给调用者处理
	::PostMessage(AfxGetMainWnd()->m_hWnd,WM_MYSOCKET,MS_ACCEPT,nErrorCode);
}

void CMySocket::OnConnect( int nErrorCode )
{	//将消息交给调用者处理
	::PostMessage(AfxGetMainWnd()->m_hWnd,WM_MYSOCKET,MS_CONNECT,nErrorCode);
}

void CMySocket::OnReceive( int nErrorCode )
{	//将消息交给调用者处理
	::PostMessage(AfxGetMainWnd()->m_hWnd,WM_MYSOCKET,MS_RECEIVE,nErrorCode);
}

void CMySocket::OnClose( int nErrorCode )
{	//将消息交给调用者处理
	::PostMessage(AfxGetMainWnd()->m_hWnd,WM_MYSOCKET,MS_CLOSE,nErrorCode);
}
