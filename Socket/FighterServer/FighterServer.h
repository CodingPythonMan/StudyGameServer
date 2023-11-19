#pragma once

#include "Protocol.h"
#include "MyList.h"
#include <WS2tcpip.h>
#include "RingBuffer.h"
#include "Player.h"

#pragma comment(lib, "ws2_32.lib")

#define SERVER_PORT 5000
#define MAX_CLIENT 64

struct Session {
	SOCKET Sock;
	int ID;
	Player* _Player;
	RingBuffer SendBuffer;
	RingBuffer RecvBuffer;
};

class FighterServer {
public:
	FighterServer();
	~FighterServer();

	bool Init();
	bool SelectLoop();
	void Close();

private:
	void AcceptProc();

	SOCKET listenSock;
	MyList<Session*> clientSocks;

	int TotalClientSocks;
	int UniqueID;
};