// MySocket.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "FiveChess.h"
#include "MySocket.h"

// CMySocket ����ͨ����

CMySocket::CMySocket()
{
}

CMySocket::~CMySocket()
{
}

// CMySocket ��Ա����

void CMySocket::OnAccept( int nErrorCode )
{	//����Ϣ���������ߴ���
	::PostMessage(AfxGetMainWnd()->m_hWnd,WM_MYSOCKET,MS_ACCEPT,nErrorCode);
}

void CMySocket::OnConnect( int nErrorCode )
{	//����Ϣ���������ߴ���
	::PostMessage(AfxGetMainWnd()->m_hWnd,WM_MYSOCKET,MS_CONNECT,nErrorCode);
}

void CMySocket::OnReceive( int nErrorCode )
{	//����Ϣ���������ߴ���
	::PostMessage(AfxGetMainWnd()->m_hWnd,WM_MYSOCKET,MS_RECEIVE,nErrorCode);
}

void CMySocket::OnClose( int nErrorCode )
{	//����Ϣ���������ߴ���
	::PostMessage(AfxGetMainWnd()->m_hWnd,WM_MYSOCKET,MS_CLOSE,nErrorCode);
}
