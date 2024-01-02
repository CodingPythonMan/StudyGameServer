#pragma once
#include "Player.h"
#include "RingBuffer.h"
#include <WS2tcpip.h>

#define HEADER_SIZE 3

struct Session {
	SOCKET Sock;
	int ID;
	Player* _Player;
	RingBuffer SendBuffer;
	RingBuffer RecvBuffer;
	WCHAR IP[16];
	unsigned short Port;

	Session() {
		Sock = 0;
		ID = 0;
		_Player = new Player;
		memset(IP, 0, 16);
		Port = 0;
	}
};

#pragma pack(push, 1)
struct PACKET_HEADER {
	unsigned char ByCode;	// 패킷코드 0x89 고정.
	unsigned char BySize;	// 패킷 사이즈.
	unsigned char ByType;	// 패킷타입.
};

#pragma pack(pop)