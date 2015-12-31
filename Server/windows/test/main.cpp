#include <cstdio>
#include <string>
#include "DealRecv.h"
#include <winsock.h>

#pragma comment(lib,"ws2_32.lib")

const int MAXREV = 100000;

using std::string;

int main(int argc, char* argv[])
{
	//��ʼ��WSA
	WORD sockVersion = MAKEWORD(2, 2);
	WSADATA wsaData;
	if (WSAStartup(sockVersion, &wsaData) != 0)
	{
		return 0;
	}

	//�����׽���
	SOCKET slisten = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (slisten == INVALID_SOCKET)
	{
		printf("socket error !");
		return 0;
	}

	//��IP�Ͷ˿�
	sockaddr_in sin;
	sin.sin_family = AF_INET;
	sin.sin_port = htons(25252);
	sin.sin_addr.S_un.S_addr = INADDR_ANY;
	if (bind(slisten, (LPSOCKADDR)&sin, sizeof(sin)) == SOCKET_ERROR)
	{
		printf("bind error !");
	}

	//��ʼ����
	if (listen(slisten, 5) == SOCKET_ERROR)
	{
		printf("listen error !");
		return 0;
	}

	//ѭ����������
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
		//��������
		int ret = recv(sClient, revData, sizeof(revData) - 1, 0);
		revData[ret] = 0x00;		//\0
		rev = revData;

		//������������
		sen = deal->deal(rev);

		//��������
		send(sClient, sen.c_str(), sen.size(), 0);
		closesocket(sClient);
		delete deal;
	}

	closesocket(slisten);
	WSACleanup();
	return 0;
}