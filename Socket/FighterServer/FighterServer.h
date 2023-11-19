#pragma once

#include "Protocol.h"
#include "MyList.h"
#include <WS2tcpip.h>
#include "RingBuffer.h"
#include "Player.h"

#pragma comment(lib, "ws2_32.lib")

#define SERVER_PORT 5000
#define MAX_CLIENT 64
#define MAX_PACKET_SIZE 50

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
	void ReadProc(Session* session);
	void WriteProc(Session* session);

	void SendUnicast(Session* session, char* message, int size);
	void SendBroadcast(Session* session, char* message, int size);
	void Disconnect(Session* session);

	SOCKET listenSock;
	MyList<Session*> clientSocks;
	MyList<Session*> deleteClients;

	int TotalClientSocks;
	int UniqueID;
};