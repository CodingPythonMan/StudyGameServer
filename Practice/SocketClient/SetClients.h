#pragma once

#include <iostream>
#include <WS2tcpip.h>
#include <windows.h>
using namespace std;

#pragma comment(lib, "ws2_32.lib")

void SetClients();

void SetCloseClient();