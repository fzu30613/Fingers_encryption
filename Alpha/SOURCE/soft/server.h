#ifndef SERVER_H
#define SERVER_H
#include <iostream>
#include <cstdlib> 
#include <cstring>
#include <WinSock2.h> // WinSocket   
#include <WS2tcpip.h> // IP��ַת���õ�inet_pton 
using namespace std;
class TCPServer
{
	private:
		SOCKET listenScok;  // �����Socket  
    	SOCKET clientSock;  // �ͻ���Socket
				  
		bool initWinSock();// ��1����ʼ��WinSock
		bool createSocket();// ��2������socket
		bool bindIPandPort(const unsigned short port);// ��3��socket�󶨱�����ַ��Ϣ 
		bool listenSocket();// ��4������socket�����տͻ�������
		bool waitClientConnect();// ��5���ȴ��ͻ�������-����    
		bool receiveData(string &data);// ��6����������-����     
		bool shutdownSocket();// ��7��ֹͣ�׽��ֵĽ��ա�����  	  
		bool sendData(const string &data);// ��8��������Ϣ 
	public:
		bool Initserver();//��ʼ������ 
		/* 
		����һ����Ϣ�յ�һ����Ϣ��ʧ�ܷ���false 
		const string &msg �������뷢�͵���Ϣ
		string &recvmsg ���ڴ�Ž��յ���Ϣ*/
		bool req_one_one(const string &msg,string &recvmsg);
		/* 
		����һ����Ϣ�յ�������Ϣ��ʧ�ܷ���false
		const string &msg �������뷢�͵���Ϣ
		string &recvmsg_one,string &recvmsg_two ���ڴ�Ž��յ���Ϣ*/
		bool req_one_two(const string &msg,string &recvmsg_one,string &recvmsg_two);//
		bool Closeserver();//�رռ����Լ��׽���	
};
#endif
