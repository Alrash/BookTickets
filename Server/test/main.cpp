#include <cstdio>
#include <string>
#include "DealRecv.h"
#include <winsock.h>

#pragma comment(lib,"ws2_32.lib")

const int MAXREV = 100000;

using std::string;

int main(int argc, char* argv[])
{
	//初始化WSA
	WORD sockVersion = MAKEWORD(2, 2);
	WSADATA wsaData;
	if (WSAStartup(sockVersion, &wsaData) != 0)
	{
		return 0;
	}

	//创建套接字
	SOCKET slisten = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (slisten == INVALID_SOCKET)
	{
		printf("socket error !");
		return 0;
	}

	//绑定IP和端口
	sockaddr_in sin;
	sin.sin_family = AF_INET;
	sin.sin_port = htons(25252);
	sin.sin_addr.S_un.S_addr = INADDR_ANY;
	if (bind(slisten, (LPSOCKADDR)&sin, sizeof(sin)) == SOCKET_ERROR)
	{
		printf("bind error !");
	}

	//开始监听
	if (listen(slisten, 5) == SOCKET_ERROR)
	{
		printf("listen error !");
		return 0;
	}

	//循环接收数据
	SOCKET sClient;
	sockaddr_in remoteAddr;
	int nAddrlen = sizeof(remoteAddr);
	char revData[MAXREV];
	string rev, sen;		//receive send

	while (true)
	{
		memset(revData, 0, sizeof(revData));
		rev = "";
		sClient = accept(slisten, (SOCKADDR *)&remoteAddr, &nAddrlen);
		if (sClient == INVALID_SOCKET)
		{
			printf("accept error !");
			continue;
		}

		printf("OK\t");

		DealRecv *deal = new DealRecv();
		//接收数据
		int ret = recv(sClient, revData, sizeof(revData) - 1, 0);
		revData[ret] = 0x00;		//\0
		rev = revData;

		//处理并返回数据
		sen = deal->deal(rev);

		//发送数据
		send(sClient, sen.c_str(), sen.size(), 0);
		closesocket(sClient);
		delete deal;
	}

	closesocket(slisten);
	WSACleanup();
	return 0;
}