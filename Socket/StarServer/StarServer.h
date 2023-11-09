#pragma once
#include <WS2tcpip.h>
#include <windows.h>
#include "Protocol.h"
#include "Console.h"
#include "MyList.h"

#pragma comment(lib, "ws2_32.lib")

#define SERVER_PORT 3000

#define MAX_STARS 30

// Init Socket
bool SetServerSocket();

// Connect
bool ListenSocket();

// Select Loop
bool SelectLoop();

// Fin Socket
bool EndSocket();

void AcceptProc(SOCKET* clientSock, SOCKADDR_IN* clientAddr);

void Flip();

void Clear();

void SpriteDraw(int X, int Y, char Sprite);