#ifndef SERVER_H
#define SERVER_H
#include <iostream>
#include <cstdlib> 
#include <cstring>
#include <WinSock2.h> // WinSocket   
#include <WS2tcpip.h> // IP地址转换用到inet_pton 
using namespace std;
class TCPServer
{
	private:
		SOCKET listenScok;  // 服务端Socket  
    	SOCKET clientSock;  // 客户端Socket
				  
		bool initWinSock();// 【1】初始化WinSock
		bool createSocket();// 【2】创建socket
		bool bindIPandPort(const unsigned short port);// 【3】socket绑定本机地址信息 
		bool listenSocket();// 【4】侦听socket，接收客户端请求
		bool waitClientConnect();// 【5】等待客户端连接-阻塞    
		bool receiveData(string &data);// 【6】接收数据-阻塞     
		bool shutdownSocket();// 【7】停止套接字的接收、发送  	  
		bool sendData(const string &data);// 【8】发送信息 
	public:
		bool Initserver();//初始化函数 
		/* 
		发送一个消息收到一个消息，失败返回false 
		const string &msg 用于输入发送的消息
		string &recvmsg 用于存放接收的消息*/
		bool req_one_one(const string &msg,string &recvmsg);
		/* 
		发送一个消息收到两个消息，失败返回false
		const string &msg 用于输入发送的消息
		string &recvmsg_one,string &recvmsg_two 用于存放接收的消息*/
		bool req_one_two(const string &msg,string &recvmsg_one,string &recvmsg_two);//
		bool Closeserver();//关闭监听以及套接字	
};
#endif
