#include "Protocol.h"
#include "MyList.h"
#include <WS2tcpip.h>
#include "RingBuffer.h"

#pragma comment(lib, "ws2_32.lib")

#define SERVER_PORT 5000
#define MAX_CLIENT 64

struct Session {
	SOCKET Sock;
	int ID;
	int X;
	int Y;
	RingBuffer sendBuffer;
	RingBuffer recvBuffer;
};

class FighterServer {
public:
	FighterServer();
	~FighterServer();

	bool Init();
	bool SelectLoop();
	void Close();

private:
	SOCKET listenSock;
	MyList<Session*> clientSocks;

	int totalClientSocks;
};