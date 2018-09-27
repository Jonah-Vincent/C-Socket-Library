//Networks Program 1 Jonah Vincent 
#include <sys/socket.h>
#include <arpa/inet.h>
#include "pingpong.h"
#include <stdio.h>
#include <string.h>
#include <netdb.h>
#include <iostream>
#include <sys/types.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <unistd.h>

using namespace std;

void ConnectionInfo::setSocket(int sock)
{
	socket = sock;
}

void ConnectionInfo::setFlag(int flag)
{
	flags = flag;
}

int ConnectionInfo::getSocket()
{
	return socket;
}

int ConnectionInfo::getFlag()
{
	return flags;
}

void ConnectionInfo::setBufferSize(int size)
{
	bufferSize = size;
}

int ConnectionInfo::getBufferSize()
{
	return bufferSize;
}

ConnectionInfo::ConnectionInfo()
{

}
int sendMessage(ConnectionInfo *con, char *msg)
{
	int cflag = con->getFlag();
	int csocket = con->getSocket();

	int msgLen = strlen(msg);
	int sendCheck = send(csocket,msg,msgLen,cflag);
	con->setBufferSize(sendCheck);
	if(sendCheck != -1)
	{
		return 0;
	}
	else
	{
		return sendCheck;
	}
}

char* recieveMessage(ConnectionInfo* con)
{
	int cflag = con->getFlag();
	int csocket = con->getSocket();

	int bufferLen = con->getBufferSize();

	char* szBuffer;
	int recvCheck = recv(csocket,szBuffer,bufferLen,cflag);
	cout << "The server sent back: " << szBuffer << endl;
	return szBuffer;
}

void dealocate_message(char* mem)
{
	mem = 0;
	delete mem;
}

int connect_to_server(char* who, int port, ConnectionInfo* con)
{
	int sockfd;
	struct sockaddr_in server_addr;
	struct hostent* hent;
	if((hent=gethostbyname(who)) == NULL)
	{
		cerr << "Invalid host name." << endl;
		return 1;
	}

	if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{
		cerr << "Socket error." << endl;
		return 1;
	}

	memset((void *) &server_addr, 0, sizeof(server_addr));

	server_addr.sin_family = AF_INET;
	server_addr.sin_addr = *((struct in_addr *)hent->h_addr);
	server_addr.sin_port = htons(port);

	if(connect(sockfd, (struct sockaddr *) &server_addr, sizeof(server_addr)) < 0)
	{
		cerr << "Connect error." << endl;
		return 1;
	}
	con->setSocket(sockfd);
	con->setFlag(0);
	return 0;
}

int run_server(int port)
{
	int sockfd, newsockfd;
	unsigned int clilen;
	struct sockaddr_in server_addr, cli_addr;

	if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{
		cerr << "Socket error." << endl;
		return 1;
	}

	memset((void *) &server_addr, 0, sizeof(server_addr));

	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	server_addr.sin_port = htons(port);

	if(bind(sockfd, (struct sockaddr *) &server_addr, sizeof(server_addr)) < 0)
	{
		cerr << "Bind error";
		return 1;
	}

	cout << "Server running on port: " << port << endl;

	listen(sockfd, 10);

	while(1)
	{
			clilen = sizeof(cli_addr);
			newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);
			if(newsockfd < 0)
			{
				cerr << "Accept error." << endl;
				return 1;
			}
			int msgSize;
			char buffer[1016];
			memset(buffer, '\0', 1016);

			if((msgSize = recv(newsockfd, buffer, 1015, 0)) < 0)
			{
				cerr << "Receive error." << endl;
			}

			cout << "Message received from client: " << buffer<<endl;
			char *response;
			response = buffer;
			if(strcmp(buffer,"PING") == 0)
			{
				response = "PONG";
			}

			if((msgSize = send(newsockfd, response, strlen(response), 0)) < 0)
			{
				cerr << "Send error." << endl;
			}

			close(newsockfd);
		}
}
