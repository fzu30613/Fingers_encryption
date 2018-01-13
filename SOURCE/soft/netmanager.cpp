#include "netmanager.h"
#include<iostream>
#include <thread>
#include <mutex>
#include <stdlib.h>
#include <winsock2.h>
#include <Windows.h>
#include <string.h>
#include<sstream>
#pragma comment(lib, "ws2_32.lib")
void IPListManager::addIP(unsigned long add, std::string DName)
{//����µ�ip�����еĽ��޸��豸�� 
	bool flag_find = false;
	for (int i = 0; i<num_ipL; i++)
	{
		if (add == ipL[i].addr)
		{
			flag_find = true;
			ipL[i].Device_name = DName;
		}
	}
	if (!flag_find)
	{
		ipL[num_ipL].addr = add;
		ipL[num_ipL].Device_name = DName;
		num_ipL++;
	}
}
void IPListManager::clearipL()
{//��������������б�
	num_ipL = 0;
}
IPList* IPListManager::getipL(int &n_ipL)
{//����һ�ڴ�ռ��Է�������ipL�������ǵ�delete 
	n_ipL = num_ipL;
	if (n_ipL != 0)
	{
		IPList *p = new IPList[num_ipL];
		for (int i = 0; i<num_ipL; i++)
		{
			p[i].addr = ipL[i].addr;
			p[i].Device_name = ipL[i].Device_name;
		}
		return p;
	}
	else return NULL;
}
std::string IPtoString(unsigned long addr) {//ipתaddrΪstring 
									   //-ip[0]-ip[1]*256-ip[2]*256*256
	int ip[4];
	ip[0] = addr % 256;
	ip[1] = ((addr) / 256) % 256;
	ip[2] = ((addr) / 256 / 256) % 256;
	ip[3] = ((addr) / 256 / 256 / 256) % 256;
	std::stringstream ss;
	ss << ip[0] << "." << ip[1] << "." << ip[2] << "." << ip[3];
	std::string ipaddress = ss.str();
	ss.str("");
	return ipaddress;
}

bool IPManager::getlocalipall() {
	WORD wVersionRequested = MAKEWORD(2, 2);

	WSADATA wsaData;
	if (WSAStartup(wVersionRequested, &wsaData) != 0)
		return false;

	char local[255] = { 0 };
	gethostname(local, sizeof(local));
	hostent* ph = gethostbyname(local);
	if (ph == NULL)
		return false;

	localipnum = 0;
	for (int i = 0;; i++)
	{
		if (ph->h_addr_list[i] == NULL) break;
		in_addr addr;
		memcpy(&addr, ph->h_addr_list[i], sizeof(in_addr)); // �������ȡ��һ��ip  

		localip[localipnum] = addr.s_addr;
		localipnum++;
	}
	WSACleanup();
	return true;
}

unsigned long IPManager::GetLocalIpAddress() { //��ñ���ip 
	WORD wVersionRequested = MAKEWORD(2, 2);
	WSADATA wsaData;
	in_addr addr;
	if (WSAStartup(wVersionRequested, &wsaData) != 0)
		return 0;
	char local[255] = { 0 };
	gethostname(local, sizeof(local));
	hostent* ph = gethostbyname(local);
	if (ph == NULL)
		return 0;
	memcpy(&addr, ph->h_addr_list[0], sizeof(in_addr));//�������ȡ��һ��ip   
	WSACleanup();
	return addr.s_addr;
}
void IPManager::refresh() {//ˢ�º���
	getlocalipall();
	if (flag_refresh == false) {
		//std::cout << "begin refresh" << std::endl;
		mu_refresh.lock();
		flag_refresh = true;
		mu_refresh.unlock();
		ipLM.clearipL();
		//std::cout<<"which client:"<<<<std::endl;
		initUDPClient();
		startUDP();
		Sleep(1000);
		stopUDP();
		Sleep(1000);
		mu_refresh.lock();
		flag_refresh = false;
		mu_refresh.unlock();
	}
	//else std::cout << "ddd" << std::endl;
}
std::string* IPManager::getIPList(int &len) {
	IPList* p = ipLM.getipL(len);
	std::string* s = new std::string[len];
	for (int i = 0; i < len; i++)
	{
		std::stringstream ss;
		ss<<IPtoString(p[i].addr)<<" "<<p[i].Device_name;
		s[i] = ss.str();
	}
	delete[]p;
	return s;
}
bool IPManager::initWinSock() {//��ʼ��winsock�⣬ʹ��socket��ǰ��
	WORD verision = MAKEWORD(2, 2);
	WSADATA lpData;
	int intEr = WSAStartup(verision, &lpData); // ָ��winsock�汾����ʼ��
	if (intEr != 0) {
		//std::cout << "WinSock��ʼ��ʧ�ܣ�" << std::endl;
		return false;
	}
	/*ȷ��֧��winsocket�汾2.2*/
	if (LOBYTE(lpData.wVersion) != 2 || HIBYTE(lpData.wVersion) != 2) {
		WSACleanup();
		//std::cout << "WinSock��ʼ��ʧ�ܣ�" << std::endl;
		return false;
	}
	//std::cout << "WinSock��ʼ���ɹ���" << std::endl;
	return true;
}
bool IPManager::initUDPClient() {//��ʼ��UDP�㲥�ͻ���
	int error=0;							 /*����socket*/
	for (int i = 0; i < localipnum; i++) {
			if (client_socket[i] != NULL)
			{
				closesocket(client_socket[i]);
				client_socket[i] = NULL;
			}
			client_socket[i] = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
			if (INVALID_SOCKET == client_socket[i]) {
				int err = WSAGetLastError();
				//printf("\"socket\" error! error code is %d\n", err);
				error = err;
				continue;
				//return false;
			}
			/*�������׽���*/
			SOCKADDR_IN sin;
			sin.sin_family = AF_INET;
			sin.sin_port = htons(CLIENT_PORT);
			//sin.sin_addr.s_addr = htonl(INADDR_BROADCAST);//�׽��ֵ�ַΪ�㲥��ַ
			sin.sin_addr.s_addr = localip[i];
			//std::cout << "client:"<<i<<","<<IPtoString(localip[i]) << std::endl;

			//std::cout << IPtoString(sin.sin_addr.s_addr) << std::endl;
			/*���ø��׽���Ϊ�㲥����*/
			bool bOpt = true;
			setsockopt(client_socket[i], SOL_SOCKET, SO_BROADCAST, (char*)&bOpt, sizeof(bOpt));
			/*���׽���*/
			int err = bind(client_socket[i], (SOCKADDR*)&sin, sizeof(SOCKADDR));
			if (SOCKET_ERROR == err) {
				err = WSAGetLastError();
				//printf("\"bind\" error! error code is %d\n", err);
				error = err;
				continue;
				//return false;
			}
	}
	if (error) return false;
	return true;
}

IPManager::IPManager() {
	getlocalipall();
	RefreshTime = 1000;
	initWinSock();
	for (int i = 0; i < localipnum; i++) {
		client_socket[i] = NULL;
	}
	flag_UDPResponse = false;
	flag_UDPBroadCast = false;
	flag_refresh = false;
}
void IPManager::sendUDPBroadCast(int which) {//����UDP�㲥���߳�����
									//�����㲥�ĵ�ַ
	SOCKADDR_IN sin_from;
	sin_from.sin_family = AF_INET;
	sin_from.sin_port = htons(SERVER_PORT);
	sin_from.sin_addr.s_addr = INADDR_BROADCAST;

	char    szMsg[] = "getIP";
	int nLen = sizeof(sin_from);
	while (flag_UDPBroadCast)
	{
		//std::lock_guard<std::mutex> lockGuard(mu_ip);
		if (SOCKET_ERROR == sendto(client_socket[which], szMsg, strlen(szMsg), 0, (sockaddr*)&sin_from, nLen)) {
			// AfxMessageBox(L"Send UDP Failed");
			break;
		}
		//printf("send broadcast data:%s\n", szMsg);
		Sleep(RefreshTime);
	}
	std::lock_guard<std::mutex> lockGuard(mu_flag);
	flag_UDPBroadCast = false;
	//std::cout << "BroadCoat end" << std::endl;
}
void IPManager::startUDP() {//�ñ�־λ�������㲥�ͽ����߳�
	if (flag_UDPResponse != false ||
		flag_UDPBroadCast != false)
	{
		flag_UDPResponse = false;
		flag_UDPBroadCast = false;
		Sleep(1000);
	}
	flag_UDPResponse = true;
	flag_UDPBroadCast = true;
	for (int i = 0; i < localipnum; i++) {
		//std::cout <<"ip:"<< i << ","<<IPtoString (localip[i])<< std::endl;
		std::thread sendbroad(&IPManager::sendUDPBroadCast, this,i);
		sendbroad.detach();
		std::thread response(&IPManager::recvUDPResponse, this,i);
		response.detach();
	}
	
	Sleep(1000);
}
void IPManager::stopUDP() {//�ñ�־λ��ֹͣ�㲥�ͽ����߳� 
	std::lock_guard<std::mutex> lockGuard(mu_flag);
	Sleep(100);
	flag_UDPResponse = false;
	Sleep(100);
	flag_UDPBroadCast = false;
}
unsigned long IPListManager::getIP(int row) {
	if(row<num_ipL)
	return ipL[row].addr;
	else return 0;
}
unsigned long IPManager::getIPbyRow(int row) {
	return ipLM.getIP(row);
}
int IPListManager::getnum_ipL() {
	return num_ipL;
}
int IPManager::getipLnum() {
	return ipLM.getnum_ipL();
}
void IPManager::recvUDPResponse(int which) {//����UDP��Ӧ���߳����壬������IP���豸�� 
	SOCKADDR_IN sin_from;
	int nAddrLen = sizeof(SOCKADDR);
	char buff[MAX_BUF_LEN] = "";
	while (flag_UDPResponse) {
		// ��������
		int nSendSize = recvfrom(client_socket[which], buff, MAX_BUF_LEN, 0, (SOCKADDR*)&sin_from, &nAddrLen);
		if (SOCKET_ERROR == nSendSize) {
			int err = WSAGetLastError();
			//printf("\"recvfrom\" error! error code is %d\n", err);
			break;
		}
		std::lock_guard<std::mutex> lockGuard(mu_ip);
		buff[nSendSize] = '\0';
		//printf("received: %s\n", buff);
		//printf("ip:%d\n", sin_from.sin_addr.s_addr);
		//std::cout << IPtoString(sin_from.sin_addr.s_addr) << std::endl;
		ipLM.addIP(sin_from.sin_addr.s_addr, buff);
	}
	std::lock_guard<std::mutex> lockGuard(mu_flag);
	flag_UDPResponse = false;
	//std::cout << "stop response" << std::endl;
}


bool tcpM::TCPManager::initWinSock() {//��ʼ��winsock�⣬ʹ��socket��ǰ��
	WORD verision = MAKEWORD(2, 2);
	WSADATA lpData;
	int intEr = WSAStartup(verision, &lpData); // ָ��winsock�汾����ʼ��
	if (intEr != 0) {
		//std::cout << "WinSock��ʼ��ʧ�ܣ�" << std::endl;
		return false;
	}
	/*ȷ��֧��winsocket�汾2.2*/
	if (LOBYTE(lpData.wVersion) != 2 || HIBYTE(lpData.wVersion) != 2) {
		WSACleanup();
		//std::cout << "WinSock��ʼ��ʧ�ܣ�" << std::endl;
		return false;
	}
	//std::cout << "WinSock��ʼ���ɹ���" << std::endl;
	return true;
}
bool tcpM::TCPManager::initTCPClient(unsigned long addr) {//��ʼ��TCP�ͻ���
													/*����socket*/
	client_socket = socket(AF_INET, SOCK_STREAM, 0);
	if (INVALID_SOCKET == client_socket) {
		int err = WSAGetLastError();
		//printf("\"socket\" error! error code is %d\n", err);
		return false;
	}
	/*�������׽���*/
	SOCKADDR_IN sin;
	memset(&sin, 0x00, sizeof(struct sockaddr_in));
	sin.sin_family = AF_INET;
	sin.sin_port = htons(TCP_PORT);
	sin.sin_addr.s_addr = addr;
	if (::connect(client_socket, (sockaddr *)&sin, sizeof(sin)) == SOCKET_ERROR) //��ָ��IP��ַ�Ͷ˿ڵķ��������
	{
		//printf("connect error !");
		closesocket(client_socket);
		flag_connect = false;
		return 0;
	}
	return true;
}
unsigned long StringtoIP(std::string ip) {
	std::stringstream ss;
	int a[4] = { 0 }; int j = 0;
	for (int i = 0; i < ip.length() && ip[i] != '\n'; i++)
	{
		if (ip[i] == '.')
		{
			std::string d = ss.str();
			a[j] = atoi(d.c_str());
			ss.str("");
			continue;
		}
		ss << ip[i];
	}
	unsigned long r=a[0]+a[1]*256+a[2]*256*256+a[3]*256*256*256;
	return r;
}
void tcpM::TCPManager::TCPconnect(unsigned long addr) {
	flag_connect = true;
	initTCPClient(addr);
}
void tcpM::TCPManager::TCPdisconnect() {
	char q[] = "close";
	send(client_socket, q, strlen(q), 0);
	char recvBuf[256] = { 0 };
	int len;
	len = recv(client_socket, recvBuf, 256, 0);
	if (SOCKET_ERROR == len)
	{
		//std::cout << "recv error";
		flag_connect = false;
	}
	else {
		recvBuf[len] = '\0';
		//std::cout << recvBuf << std::endl;
	}
	flag_connect = false;
	closesocket(client_socket);
}
void tcpM::TCPManager::requestEncrypt(std::string &recvmsg_one, std::string &recvmsg_two) {
	char q[] = "encrypt";
	send(client_socket, q, strlen(q), 0);
	Sleep(100);
	//std::cout << "begin recv" << std::endl;
	char recvBuf[256] = { 0 };
	int len = 0;
	len = recv(client_socket, recvBuf, 256, 0);
	if (SOCKET_ERROR == len||len<=0)
	{
		//std::cout << "recv error";
		flag_connect = false;
		recvmsg_one = "";
		recvmsg_two = "";
	}
	else {
		//std::cout << len << std::endl;
		//system("pause");
		recvBuf[len] = '\0';
		char *p;
		p = strtok(recvBuf, "@");
		recvmsg_one = p;
		p = strtok(NULL, "@");
		if (p != NULL) {
			
			recvmsg_two = p;
		}	
		else recvmsg_two = "";
	}

}
void tcpM::TCPManager::requestDecrypt(std::string &recvmsg) {
	char q[] = "decrypt";
	send(client_socket, q, strlen(q), 0);
	Sleep(100);
	//std::cout << "begin recv" << std::endl;
	char recvBuf[256] = { 0 };
	int len = 0;

	len = recv(client_socket, recvBuf, 256, 0);
	if (SOCKET_ERROR == len||len<=0)
	{
		//std::cout << "recv error";
		recvmsg = "";
		flag_connect = false;
	}
	else {
		recvBuf[len] = '\0';
		recvmsg = recvBuf;
	}

}
