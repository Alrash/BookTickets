#include "stdafx.h"
#include "ClientSocket.h"
#include <cstring>

ClientSocket::ClientSocket()
{
	if (WSAStartup(MAKEWORD(2, 2), &wsa))
		throw InitDllFail;
}

ClientSocket::~ClientSocket()
{
	WSACleanup();
}

int ClientSocket::init()
{
	//´´½¨Ì×½Ó×Ö
	if ((sock = (socket(AF_INET,SOCK_STREAM,IPPROTO_TCP)))==INVALID_SOCKET)
	{
		closesocket(sock);
		throw CreateSocketFail;
	}

	memset(&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.S_un.S_addr = inet_addr(ip.c_str());
	serv_addr.sin_port = htons(port);

	return 0;
}

int ClientSocket::Connect()
{
	if (connect(sock, (sockaddr *)&serv_addr, sizeof(serv_addr)) == SOCKET_ERROR)
	{
		closesocket(sock);
		throw ConnectFail;
	}

	return 0;
}

int ClientSocket::Send(string str)
{
	if (send(sock, str.c_str(), str.size(), 0) == SOCKET_ERROR)
	{
		closesocket(sock);
		throw SendFail;
	}

	return 0;
}

string ClientSocket::Recv()
{
	char *s = new char[MAX];
	int byte;

	if ((byte = recv(sock, s, sizeof(char) * MAX - 1, 0)) == SOCKET_ERROR)
	{
		closesocket(sock);
		throw ReceiveFail;
	}
	s[byte] = '\0';

	string str;
	str = s;
	delete[] s;
	return str;
}

int ClientSocket::closeclient()
{
	closesocket(sock);
	return 0;
}