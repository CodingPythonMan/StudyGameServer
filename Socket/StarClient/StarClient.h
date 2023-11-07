#pragma once
#include <iostream>
#include <WS2tcpip.h>
#include <windows.h>
#include "Protocol.h"
#include "Console.h"

#pragma comment(lib, "ws2_32.lib")

#define SERVER_PORT 3000

#define MAX_STARS 30

// Init Socket
bool SetClientSocket();

// Connect
bool ConnectClientSocket(WCHAR IP[]);

// Trans Non-Block
bool TransSocket();

// Select Loop
bool SelectLoop();

// Fin Socket
bool EndSocket();

void Flip();

void Clear();

void SpriteDraw(int X, int Y, char Sprite);