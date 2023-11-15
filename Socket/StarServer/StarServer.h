#pragma once
#include <WS2tcpip.h>
#include <windows.h>
#include "Protocol.h"
#include "Console.h"
#include "MyList.h"
#include "RingBuffer.h"

#pragma comment(lib, "ws2_32.lib")

#define SERVER_PORT 3000

#define MAX_STARS 30
#define RINGBUFFER_MAX 1000

struct Session {
	SOCKET Sock;
	int ID;
	int X;
	int Y;
	RingBuffer sendBuffer;
	RingBuffer recvBuffer;
	WCHAR IP[16];
	int Port;

	Session()
	{
		Sock = NULL;
		ID = 0;
		X = 0;
		Y = 0;
		memset(IP, 0, 16);
		Port = 0;
	}
};

// Init Socket
bool SetServerSocket();

// Connect
bool ListenSocket();

// Select Loop
bool SelectLoop();

// Fin Socket
bool EndSocket();

void AcceptProc();

void ReadProc(Session* session);

void WriteProc(Session* session);

// Send, Disconnect
void SendUnicast(Session* session, char* buf);

void SendBroadcast(Session* session, char* buf);

void Disconnect(Session* session);

void DeleteExecute();

// Rendering
void Rendering();

void Flip();

void Clear();

void SpriteDraw(int X, int Y, char Sprite);