#include <iostream>
#include <WS2tcpip.h>
#include <locale.h>
using namespace std;

#pragma comment(lib, "ws2_32.lib")

#define PORT_SEARCH_MAX 99
#define SEARCH_PORT 10001
#define BUFSIZE 512

int main()
{
	_wsetlocale(LC_ALL, L"korean");

	int retval;

	// 윈속 초기화
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		return 1;

	// socket
	SOCKET socks[PORT_SEARCH_MAX];
	for (INT i = 0; i < PORT_SEARCH_MAX; i++)
	{
		socks[i] = socket(AF_INET, SOCK_DGRAM, 0);
		if (socks[i] == INVALID_SOCKET)
			return -1;
	}

	// bind
	SOCKADDR_IN localAddr[PORT_SEARCH_MAX];
	for (int i = 0; i < PORT_SEARCH_MAX; i++)
	{
		memset(&localAddr[i], 0, sizeof(SOCKADDR_IN));
		localAddr[i].sin_family = AF_INET;
		localAddr[i].sin_addr.s_addr = htonl(INADDR_ANY);
		localAddr[i].sin_port = htons(SEARCH_PORT + i);
		retval = bind(socks[i], (SOCKADDR*)&localAddr[i], sizeof(localAddr[i]));
		if (retval == SOCKET_ERROR)
			return -1;
	}

	// set up select parameters
	SOCKADDR_IN peerAddr;
	int addrLen;
	WCHAR buf[BUFSIZE + 1];

	int setCount = PORT_SEARCH_MAX / FD_SETSIZE + 1;
	FD_SET* rsets = new FD_SET[setCount];

	WCHAR clientIP[16];

	while (1)
	{
		for (int i = 0; i < setCount; i++)
			FD_ZERO(rsets+i);
		
		for (int i = 0; i < PORT_SEARCH_MAX; i++)
		{
			int set = i / FD_SETSIZE;
			FD_SET(socks[i], rsets+set);
		}

		const timeval TimeVal{ 0, 200000 };
		for(int i=0; i<setCount; i++)
			select(0, rsets+i, nullptr, nullptr, &TimeVal);

		for (int i = 0; i < PORT_SEARCH_MAX; i++)
		{
			int set = i / FD_SETSIZE;
			if (FD_ISSET(socks[i], rsets+set)) {
				addrLen = sizeof(peerAddr);
				retval = recvfrom(socks[i], (char*)buf, BUFSIZE, 0, (SOCKADDR*)&peerAddr, &addrLen);
				InetNtop(AF_INET, &peerAddr.sin_addr, clientIP, 16);
				wprintf(L"[TCP/%s:%d] : %s\n", clientIP, ntohs(peerAddr.sin_port), buf);

				memset(buf, 0, BUFSIZE);

				if (retval == SOCKET_ERROR)
					return 1;
			}
		}
	}

	delete[] rsets;

	for (int i = 0; i < PORT_SEARCH_MAX; i++)
	{
		closesocket(socks[i]);
	}
	
	WSACleanup();
	return 0;
}