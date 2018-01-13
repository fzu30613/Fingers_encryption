#ifndef SERVER_H
#define SERVER_H
#include<iostream>
#include <thread>
#include <mutex>
#include <winsock2.h>
#include <Windows.h>
/*
����WSAStartup������WSACleanup�������ó���Ľ��:
����
�����ʱ�����
undefined reference to `__imp_WSAStartup'
undefined reference to `__imp_WSACleanup'
��һ��������в���ֱ�����ӱ���wsock��̬�⣬��Ҫ�ֶ���ӿ�
���ȿ��Գ��Դ�����ֱ�����붯̬�⣬#pragam comment(lib,"ws2_32");
����������ֶ���ӿ�
��vc�У�����Ŀ(project-->project option)��makefile������-L"ws2_32"
��dvc C++�е������(Tool)-->����ѡ�Compile Option)�ڱ��������������-static-libgcc -lwsock32
*/
#include <string.h>
#include<sstream>
#pragma comment(lib, "ws2_32.lib")
#define CLIENT_PORT 5000
#define SERVER_PORT 4999
#define TCP_PORT 2228
#define MAXIPNUM 255
const int MAX_BUF_LEN = 255;
bool GetLocalIP(char* ip);
std::string IPtoString(unsigned long addr);//��unsigned long ��ip��ַתλstring
unsigned long StringtoIP(std::string ip);
struct IPList
{//ip���豸���Ľṹ�� 
	unsigned long addr;//ip��unsigned long��ʾ����ֱ������s_addr
	std::string Device_name;//�豸�� 
};
class IPListManager
{//����IP���豸��,���ù������
private:
	IPList ipL[MAXIPNUM];
	int num_ipL;
public:
	IPListManager() { num_ipL = 0; }
	void addIP(unsigned long add, std::string DName);//���ip�����ù�
	void clearipL();//����б�
	IPList* getipL(int &n_ipL);
	unsigned long getIP(int row);
	int getnum_ipL();
};
namespace tcpM
{
	class TCPManager
	{//tcp���ӵĹ�������ʽʹ��
	private:
		bool flag_connect;
		SOCKET client_socket;

		bool initWinSock();
		bool initTCPClient(unsigned long addr);
	public:
		bool isConnect() { return flag_connect; }//�����ж��Ƿ�����
		TCPManager() {
			initWinSock();
		}
		~TCPManager() {
			if (flag_connect = true) {
				TCPdisconnect();
			}
			WSACleanup();
		}
		void TCPconnect(unsigned long addr);//����ָ��ip��Ҫ��getIPbyRow()���ص�ip,����isConnect���������ж��Ƿ�ɹ�������
		void TCPdisconnect();//�ر�tcp����
		/*�������ڽ�����Ϣʱ�Ͽ�������յ�����ϢΪ�գ�����isConnect�����᷵��false*/
		void requestEncrypt(std::string &recvmsg_one, std::string &recvmsg_two);//������ܣ��ظ�����Ϣ�����recvmsg��
		void requestDecrypt(std::string &recvmsg);//������ܣ��ظ�����Ϣ�����recvmsg��
	};
}
class IPManager {//��ȡ�������ڿ���ip 
private:
	int RefreshTime;
	SOCKET client_socket[32];
	bool flag_UDPBroadCast;
	bool flag_UDPResponse;
	std::mutex mu_flag;
	std::mutex mu_ip;
	bool flag_refresh;
	std::mutex mu_refresh;

	unsigned long localip[32];
	int localipnum;
	bool getlocalipall();
	unsigned long GetLocalIpAddress();//��ñ���ip 
	bool initWinSock();//��ʼ��winsock�⣬ʹ��socket��ǰ��
	bool initUDPClient();//��ʼ��UDP�ͻ���
	void sendUDPBroadCast(int which);//����UDP�㲥���߳�����
	void recvUDPResponse(int which);//����UDP��Ӧ���߳����壬������IPList��num_IPList
public:
	IPListManager ipLM;
	void refresh();//ˢ�º�����ˢ��ipList
	IPManager();
	~IPManager() {
		flag_UDPResponse = false;
		flag_UDPBroadCast = false;
		for (int i = 0; i < localipnum; i++) {
			if(client_socket[i] != NULL)
						closesocket(client_socket[i]);
		}
		
		WSACleanup();
	}
	void startUDP();//���ù�
	void stopUDP();//���ù�
	std::string* getIPList(int &len);/*�õ�ipList���ǵ������delete��*/
	unsigned long getIPbyRow(int row);//���������õ�unsigned long ��ip,�����ڷ���0
	int getipLnum();//�õ���
};

#endif
