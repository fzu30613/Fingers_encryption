#ifndef SERVER_H
#define SERVER_H
#include<iostream>
#include <thread>
#include <mutex>
#include <winsock2.h>
#include <Windows.h>
/*
关于WSAStartup方法和WSACleanup方法引用出错的解决:
错误：
编译的时候出现
undefined reference to `__imp_WSAStartup'
undefined reference to `__imp_WSACleanup'
在一般编译器中不会直接连接编译wsock动态库，需要手动添加库
首先可以尝试代码中直接引入动态库，#pragam comment(lib,"ws2_32");
如果不行再手动添加库
在vc中，点项目(project-->project option)在makefile中输入-L"ws2_32"
在dvc C++中点击工具(Tool)-->编译选项（Compile Option)在编译命令框中输入-static-libgcc -lwsock32
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
std::string IPtoString(unsigned long addr);//将unsigned long 的ip地址转位string
unsigned long StringtoIP(std::string ip);
struct IPList
{//ip和设备名的结构体 
	unsigned long addr;//ip的unsigned long表示，可直接用于s_addr
	std::string Device_name;//设备名 
};
class IPListManager
{//管理IP和设备名,不用管这个类
private:
	IPList ipL[MAXIPNUM];
	int num_ipL;
public:
	IPListManager() { num_ipL = 0; }
	void addIP(unsigned long add, std::string DName);//添加ip，不用管
	void clearipL();//清空列表
	IPList* getipL(int &n_ipL);
	unsigned long getIP(int row);
	int getnum_ipL();
};
namespace tcpM
{
	class TCPManager
	{//tcp连接的管理，阻塞式使用
	private:
		bool flag_connect;
		SOCKET client_socket;

		bool initWinSock();
		bool initTCPClient(unsigned long addr);
	public:
		bool isConnect() { return flag_connect; }//用于判断是否连接
		TCPManager() {
			initWinSock();
		}
		~TCPManager() {
			if (flag_connect = true) {
				TCPdisconnect();
			}
			WSACleanup();
		}
		void TCPconnect(unsigned long addr);//连接指定ip，要用getIPbyRow()返回的ip,请用isConnect（）函数判断是否成功连接了
		void TCPdisconnect();//关闭tcp连接
		/*若连接在接收消息时断开，则接收到的信息为空，调用isConnect（）会返回false*/
		void requestEncrypt(std::string &recvmsg_one, std::string &recvmsg_two);//请求加密，回复的信息存放在recvmsg中
		void requestDecrypt(std::string &recvmsg);//请求解密，回复的信息存放再recvmsg中
	};
}
class IPManager {//获取局域网内可用ip 
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
	unsigned long GetLocalIpAddress();//获得本地ip 
	bool initWinSock();//初始化winsock库，使用socket的前提
	bool initUDPClient();//初始化UDP客户端
	void sendUDPBroadCast(int which);//发送UDP广播的线程主体
	void recvUDPResponse(int which);//接受UDP回应的线程主体，并更新IPList和num_IPList
public:
	IPListManager ipLM;
	void refresh();//刷新函数，刷新ipList
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
	void startUDP();//不用管
	void stopUDP();//不用管
	std::string* getIPList(int &len);/*得到ipList，记得用完后delete掉*/
	unsigned long getIPbyRow(int row);//利用行数得到unsigned long 的ip,不存在返回0
	int getipLnum();//得到表长
};

#endif
