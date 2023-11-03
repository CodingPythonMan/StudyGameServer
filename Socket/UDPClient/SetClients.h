#pragma once
#include <iostream>
#include <WS2tcpip.h>
#include <locale>
using namespace std;

#pragma comment(lib, "ws2_32.lib")

#define PORT_SEARCH_MAX 99
#define SEARCH_PORT 10001
#define SPECIFIC_PORT 10050

int SetUDPBroadcastClient();

int SendDefaultUDP();