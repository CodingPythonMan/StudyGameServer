#pragma once
#include <WS2tcpip.h>
#include <windows.h>
#include <iostream>
using namespace std;

#pragma comment(lib, "ws2_32.lib")

BOOL DomainToIP(const WCHAR* domain, IN_ADDR* addr);

void PrintIP();