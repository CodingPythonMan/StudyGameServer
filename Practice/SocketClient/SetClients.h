#pragma once

#include <iostream>
#include <WS2tcpip.h>
#include <windows.h>
using namespace std;

#pragma comment(lib, "ws2_32.lib")

void SetClients();

void SetCloseClient();

// EchoServer 용으로 만든 기본 함수
int SetDefaultEchoClient();