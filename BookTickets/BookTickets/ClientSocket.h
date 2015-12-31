#ifndef _CLIENTSOCKET_H

#define _CLIENTSOCKET_H

#include <string>
#include <winsock2.h>
#include <windows.h>
#include <climits>

#pragma comment(lib,"ws2_32.lib")

using std::string;

const string ip("127.0.0.1");
const short port = 25252;
const int MAX = SHRT_MAX - 1;

class ClientSocket
{
public:
	ClientSocket();
	~ClientSocket();

	int init();
	int Connect();
	int Send(string str);
	string Recv();
	int closeclient();

private:
	WSADATA wsa;
	SOCKET sock;
	sockaddr_in serv_addr;
};

#endif
