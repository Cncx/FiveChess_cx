// MySocket.h : header file
//
//该类可通用，通过消息调用其他类的函数，没有直接关联

#pragma once

//由调用者处理该自定义消息
#define WM_MYSOCKET	(WM_USER+10)
#define MS_ACCEPT	0
#define MS_CONNECT	1
#define MS_RECEIVE	2
#define MS_CLOSE	3

// CMySocket 网络通信类

class CMySocket : public CAsyncSocket
{
// Attributes
public:

// Operations
public:
	CMySocket();
	virtual ~CMySocket();

// Implementation
protected: //重写由框架调用的消息响应函数
	virtual void OnAccept( int nErrorCode );
	virtual void OnConnect( int nErrorCode );
	virtual void OnReceive( int nErrorCode );
	virtual void OnClose( int nErrorCode );
};