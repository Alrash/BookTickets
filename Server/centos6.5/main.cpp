#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/shm.h>
#include "DealRecv.h"

#define MYPORT 		25252 
#define QUEUE   	20
#define BUFFER_SIZE 50000

using std::string;

int main()
{
    ///定义sockfd
    int server_sockfd = socket(AF_INET,SOCK_STREAM, 0);

    ///定义sockaddr_in
    struct sockaddr_in server_sockaddr;
    server_sockaddr.sin_family = AF_INET;
    server_sockaddr.sin_port = htons(MYPORT);
    server_sockaddr.sin_addr.s_addr = htonl(INADDR_ANY);

    ///bind，成功返回0，出错返回-1
    if(bind(server_sockfd,(struct sockaddr *)&server_sockaddr,sizeof(server_sockaddr))==-1)
    {
        perror("bind");
        exit(1);
    }

    ///listen，成功返回0，出错返回-1
    if(listen(server_sockfd,QUEUE) == -1)
    {
        perror("listen");
        exit(1);
    }

    ///客户端套接字
    char buffer[BUFFER_SIZE];
	int len;
    struct sockaddr_in client_addr;
    socklen_t length = sizeof(client_addr);
	string rev, sen;						//reveice send

    while(true)
	{
        memset(buffer,0,sizeof(buffer));
		rev = "";
		///成功返回非负描述字，出错返回-1
		int conn = accept(server_sockfd, (struct sockaddr*)&client_addr, &length);
		if(conn<0)
		{
			printf("connect");
			continue;
		}

		DealRecv *deal = new DealRecv();
		//接收数据
        len = recv(conn, buffer, sizeof(buffer),0);
		buffer[len] = 0x00;
		rev = buffer;

		//处理数据
		sen = deal->deal(rev);

		//发送处理完成的数据
        send(conn, sen.c_str(), sen.size(), 0);
    	close(conn);
		delete deal;
    }

    close(server_sockfd);
    return 0;
}
