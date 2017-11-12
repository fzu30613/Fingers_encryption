#include"server.h"
#include <iostream>
#include <cstdlib> 
#include<cstdio> 
#include <cstring>  
#include <WinSock2.h>// WinSocket
#include <WS2tcpip.h> // IP��ַת���õ�inet_pton 
#pragma comment(lib,"ws2_32.lib")
using namespace std;

bool TCPServer::initWinSock()  
{// ��1����ʼ��WinSock  
    WORD verision = MAKEWORD(2, 2);  
    WSADATA lpData;  
    int intEr = WSAStartup(verision, &lpData); // ָ��winsock�汾����ʼ��  
    if (intEr != 0)  
    {  
        //cout << "WinSock��ʼ��ʧ�ܣ�" << endl;  
        return false;  
    }  
   // cout << "WinSock��ʼ���ɹ���" << endl;  
    return true;  
}  
bool TCPServer::createSocket()  
{// ��2������socket  
    // ��������socket    
    listenScok = socket(AF_INET, SOCK_STREAM, 0);  
    if (listenScok == INVALID_SOCKET)  
    {  
        //cout << "socket����ʧ�ܣ�" << endl;  
        return false;  
    }  
    //cout << "socket�����ɹ���" << endl;  
    return true;  
}   
bool TCPServer::bindIPandPort(const unsigned short port)  
{// ��3��socket�󶨱�����ַ��Ϣ   
    // ����sockaddr_in�ṹ��  
    // ��bind������connect�������ᵽ���׽��ֱ�������ַ��Ϣ�ṹ��const struct sockaddr��const struct sockaddr_i  
    sockaddr_in hostAddr;  
    hostAddr.sin_family = AF_INET;  
    hostAddr.sin_port = htons(port);  
  	hostAddr.sin_addr.S_un.S_addr = htonl(INADDR_ANY);//��������ip 
    // �����׽���listenSock�󶨱�����ַ��Ϣ    
    int err = bind(listenScok, (struct sockaddr*)&hostAddr, sizeof(sockaddr));  
    if (err != 0)  
    {  
        //cout << "����IP��ʧ�ܣ�" << endl;  
        return false;  
    }  
    return true;  
}    
bool TCPServer::listenSocket()  
{// ��4������socket�����տͻ�������  
    // �趨�׽���Ϊ����״̬��׼�����տͻ������̷���������������  
    int err = listen(listenScok, 3);  
    if (err != 0)  
    {  
        //cout << "socket����ʧ�ܣ�" << endl;  
        return false;  
    }  
    //cout << "�����ͻ��������С���" << endl;  
    return true;  
}    
bool TCPServer::waitClientConnect()  
{// ��5���ȴ��ͻ�������-����  
    sockaddr_in clientAddr;  
    int len = sizeof(struct sockaddr); // ����ָ�����ȣ�����ᵼ��accept����10014����  
                                       // accept��ѭ���ȴ��ͻ�������  
    clientSock = accept(listenScok, (struct sockaddr*)&clientAddr, &len);  
    ////cout << "�ͻ���Socket���:" << clientSock << endl;  
    if (clientSock == INVALID_SOCKET)  
    {  
        //cout << "�ͻ�������ʧ�ܣ�" << endl;  
        //cout << WSAGetLastError() << endl;  
        return false;  
    }  
    return true;  
}   
bool TCPServer::receiveData(string &data)  
{// ��6����������-����  
    // ͨ���ѽ������ӵ��׽��֣��������� �趨����1024�ֽ�  
    char buf[1024] = "\0";  
    // flags������ʽ��0�������ݣ�MSG_PEEDϵͳ�����������ݸ��Ƶ����ṩ�Ľ��ջ������ڣ�ϵͳ����������δɾ����MSG_OOB����������ݣ�ͨ���ò���0���ɣ�  
    int buflen = recv(clientSock, buf, 1024, 0);  
    if (buflen == SOCKET_ERROR)  
    {  
        //cout << "����ʧ�ܣ�" << endl;  
        return false;  
    }  
    // һ����������ʾ��������  
    data = string(buf);  
    return true;  
} 
bool TCPServer::shutdownSocket()  
{// ��7��ֹͣ�׽��ֵĽ��ա�����  
    //������͹أ�ֹͣ�׽��ֵĽ��ա����͹��ܣ�0��ֹ���գ�1��ֹ���ͣ�2���ƽ��շ��ͣ�  
    int err = shutdown(clientSock, 2);  
    if (err == SOCKET_ERROR)  
    {  
        //cout << "�ر�Socketʧ�ܣ�" << endl;  
        return false;  
    }  
    return true;  
}   
bool TCPServer::sendData(const string &data)  
{// ��8��������Ϣ 
    int err = send(clientSock, data.c_str(), data.size(), 0);  
    if (err == SOCKET_ERROR)  
    {  
        //cout << "����ʧ�ܣ�" << endl;  
        return false;  
    }  
    //cout << "��������Ϊ:\n" << data << endl;  
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
        cout << "�ر�socketʧ�ܣ�" << endl;
		return false;  
    } */ 
    // ֹͣʹ��WinSock�⣬�ͷŶ�Ӧ��Դ  
    if (WSACleanup() != 0)  
    {  
        //cout << "WSA���ʧ�ܣ�" << endl;
		return false; 
    }
    return true;
}


