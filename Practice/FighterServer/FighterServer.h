#pragma once

#include "Protocol.h"
#include "MyList.h"
#include <WS2tcpip.h>
#include "Session.h"

#pragma comment(lib, "ws2_32.lib")
#pragma comment(lib, "winmm.lib")

#define SERVER_PORT 5000
#define MAX_CLIENT 64
#define WAIT 20

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

	void CheckDamage(Session* session, ATTACK_TYPE attackType);

	void SendUnicast(Session* session, char* message, int size);
	void SendBroadcast(Session* session, char* message, int size);

	void Disconnect(Session* session);
	void DeleteSessions();

	SOCKET listenSock;
	MyList<Session*> clientSocks;
	MyList<Session*> deleteClients;

	int TotalClientSocks;
	int UniqueID;
};