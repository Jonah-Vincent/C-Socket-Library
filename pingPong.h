#ifndef pingPong_H
#define ADD_H
class ConnectionInfo
{
	private:
		int socket;
		int flags;
		int bufferSize;
	public:
		ConnectionInfo();
		void setSocket(int sock);
		void setFlag(int flag);
		int getSocket();
		int getFlag();
		void setBufferSize(int size);
		int getBufferSize();
};

int sendMessage(ConnectionInfo *con, char *msg);

char* recieveMessage( ConnectionInfo* con);

void dealocate_message(char* mem );

int connect_to_server( char* who, int port, ConnectionInfo* con);

int run_server(int port);

#endif