#include"server.h"
#include <iostream>
#include <cstdlib> 
#include<cstdio> 
#include <cstring>  
#include <WinSock2.h>// WinSocket
#include <WS2tcpip.h> // IP地址转换用到inet_pton 
#pragma comment(lib,"ws2_32.lib")
using namespace std;

bool TCPServer::initWinSock()  
{// 【1】初始化WinSock  
    WORD verision = MAKEWORD(2, 2);  
    WSADATA lpData;  
    int intEr = WSAStartup(verision, &lpData); // 指定winsock版本并初始化  
    if (intEr != 0)  
    {  
        //cout << "WinSock初始化失败！" << endl;  
        return false;  
    }  
   // cout << "WinSock初始化成功！" << endl;  
    return true;  
}  
bool TCPServer::createSocket()  
{// 【2】创建socket  
    // 创建侦听socket    
    listenScok = socket(AF_INET, SOCK_STREAM, 0);  
    if (listenScok == INVALID_SOCKET)  
    {  
        //cout << "socket创建失败！" << endl;  
        return false;  
    }  
    //cout << "socket创建成功！" << endl;  
    return true;  
}   
bool TCPServer::bindIPandPort(const unsigned short port)  
{// 【3】socket绑定本机地址信息   
    // 制作sockaddr_in结构体  
    // 在bind函数，connect函数里提到了套接字编程网络地址信息结构体const struct sockaddr和const struct sockaddr_i  
    sockaddr_in hostAddr;  
    hostAddr.sin_family = AF_INET;  
    hostAddr.sin_port = htons(port);  
  	hostAddr.sin_addr.S_un.S_addr = htonl(INADDR_ANY);//监听所有ip 
    // 侦听套接字listenSock绑定本机地址信息    
    int err = bind(listenScok, (struct sockaddr*)&hostAddr, sizeof(sockaddr));  
    if (err != 0)  
    {  
        //cout << "本地IP绑定失败！" << endl;  
        return false;  
    }  
    return true;  
}    
bool TCPServer::listenSocket()  
{// 【4】侦听socket，接收客户端请求  
    // 设定套接字为侦听状态，准备接收客户机进程发送来的连接请求  
    int err = listen(listenScok, 3);  
    if (err != 0)  
    {  
        //cout << "socket监听失败！" << endl;  
        return false;  
    }  
    //cout << "监听客户端连接中……" << endl;  
    return true;  
}    
bool TCPServer::waitClientConnect()  
{// 【5】等待客户端连接-阻塞  
    sockaddr_in clientAddr;  
    int len = sizeof(struct sockaddr); // 必须指定长度，否则会导致accept返回10014错误  
                                       // accept会循环等待客户端连接  
    clientSock = accept(listenScok, (struct sockaddr*)&clientAddr, &len);  
    ////cout << "客户端Socket编号:" << clientSock << endl;  
    if (clientSock == INVALID_SOCKET)  
    {  
        //cout << "客户端连接失败！" << endl;  
        //cout << WSAGetLastError() << endl;  
        return false;  
    }  
    return true;  
}   
bool TCPServer::receiveData(string &data)  
{// 【6】接收数据-阻塞  
    // 通过已建立连接的套接字，接收数据 设定缓冲1024字节  
    char buf[1024] = "\0";  
    // flags操作方式（0正常数据，MSG_PEED系统缓冲区的数据复制到所提供的接收缓冲区内，系统缓冲区数据未删除，MSG_OOB处理带外数据，通常用参数0即可）  
    int buflen = recv(clientSock, buf, 1024, 0);  
    if (buflen == SOCKET_ERROR)  
    {  
        //cout << "接收失败！" << endl;  
        return false;  
    }  
    // 一切正常则显示接收数据  
    data = string(buf);  
    return true;  
} 
bool TCPServer::shutdownSocket()  
{// 【7】停止套接字的接收、发送  
    //（收完就关）停止套接字的接收、发送功能（0禁止接收，1禁止发送，2禁制接收发送）  
    int err = shutdown(clientSock, 2);  
    if (err == SOCKET_ERROR)  
    {  
        //cout << "关闭Socket失败！" << endl;  
        return false;  
    }  
    return true;  
}   
bool TCPServer::sendData(const string &data)  
{// 【8】发送信息 
    int err = send(clientSock, data.c_str(), data.size(), 0);  
    if (err == SOCKET_ERROR)  
    {  
        //cout << "发送失败！" << endl;  
        return false;  
    }  
    //cout << "发送数据为:\n" << data << endl;  
    return true;  
}
bool TCPServer::Initserver()
{
	if(!initWinSock()) return false;
	if(!createSocket())return false; 
	if(!bindIPandPort(2228))return false;
	if(!listenSocket())return false;
	return true;
}
bool TCPServer::req_one_one(const string &msg,string &recvmsg)
{
	if(!waitClientConnect()){//cout<<"client connect error"<<endl;
		return false;}
	if(!sendData(msg)){//cout<<"faile to send msg"<<endl;
		return false;}
	if(!receiveData(recvmsg)){//cout<<"faile to recerver msg"<<endl;
		return false;}
	if(!shutdownSocket()) return false;
	return true;
}
bool TCPServer::req_one_two(const string &msg,string &recvmsg_one,string &recvmsg_two)
{
	if(!waitClientConnect()){//cout<<"client connect error"<<endl;
		return false;}
	if(!sendData(msg)){//cout<<"faile to send msg"<<endl;
		return false;}
	if(!receiveData(recvmsg_one)){//cout<<"faile to recerver msg"<<endl;
		return false;}
	if(!receiveData(recvmsg_two)){//cout<<"faile to recerver msg"<<endl;
		return false;}
	if(!shutdownSocket()) return false;
	return true;
}
bool TCPServer::Closeserver()
{
   	if(!shutdownSocket()) return false;
	/*if (closesocket(listenScok)== SOCKET_ERROR)  
	{  
        cout << "关闭socket失败！" << endl;
		return false;  
    } */ 
    // 停止使用WinSock库，释放对应资源  
    if (WSACleanup() != 0)  
    {  
        //cout << "WSA清空失败！" << endl;
		return false; 
    }
    return true;
}


