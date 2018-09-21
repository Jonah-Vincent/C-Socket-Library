#include <sys/socket.h>
#include <arpa/inet.h>
#include "pingpong.h"
#include <stdio.h>
#include <string.h>
#include <netdb.h>

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
	return szBuffer; 
}

void dealocate_message(char* mem)
{
	delete mem;
}

int connect_to_server(char* who, int port, ConnectionInfo* con)
{
	struct sockaddr_in server_address;
	struct hostent* hent = gethostbyname(who);
	
	server_address.sin_family = AF_INET;
	server_address.sin_port = htons(port);
	//server_address.sin_addr = *(struct in_addr*)hent->h_addr;
	
	
	
	con->setSocket(socket(AF_INET, SOCK_STREAM,0));
	int socket_fd = con->getSocket();
	
	int connectCheck = connect(socket_fd,(struct sockaddr*)&server_address,sizeof(server_address));  
	
	return connectCheck;
	
}

int run_server(int port)
{
	struct sockaddr_in server_address;
	
	server_address.sin_family=AF_INET;
	server_address.sin_port=htons(port);
	server_address.sin_addr.s_addr=INADDR_ANY;
	
	int socket_fd = socket(AF_INET, SOCK_STREAM,0);
	int bindcheck = bind(socket_fd, (struct sockaddr*)&server_address, sizeof(server_address));
	listen(socket_fd,10);
	return bindcheck;
}



