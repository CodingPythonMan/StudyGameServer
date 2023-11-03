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
	int retval;

	_wsetlocale(LC_ALL, L"korean");

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
	char buf[BUFSIZE + 1];

	FD_SET rset1;
	FD_SET rset2;

	WCHAR clientIP[16];
	while (1)
	{
		FD_ZERO(&rset1);
		FD_ZERO(&rset2);
		for (int i = 0; i < 64; i++)
		{
			FD_SET(socks[i], &rset1);
		}
		for (int i = 64; i < PORT_SEARCH_MAX; i++)
		{
			FD_SET(socks[i], &rset2);
		}

		const timeval TimeVal{ 0, 200000 };
		select(0, &rset1, nullptr, nullptr, &TimeVal);
		select(0, &rset2, nullptr, nullptr, &TimeVal);
		for (int i = 0; i < 64; i++)
		{
			if (FD_ISSET(socks[i], &rset1)) {
				addrLen = sizeof(peerAddr);
				// handle socket 1
				retval = recvfrom(socks[i], buf, BUFSIZE, 0, (SOCKADDR*)&peerAddr, &addrLen);
				printf("%s", buf);
				InetNtop(AF_INET, &peerAddr.sin_addr, clientIP, 16);
				wprintf(L"[TCP/%s:%d]", clientIP, ntohs(peerAddr.sin_port));

				memset(buf, 0, BUFSIZE);

				if (retval == SOCKET_ERROR)
					return 1;
			}
		}

		for (int i = 64; i < PORT_SEARCH_MAX; i++)
		{
			if (FD_ISSET(socks[i], &rset2)) {
				addrLen = sizeof(peerAddr);
				// handle socket 1
				retval = recvfrom(socks[i], buf, BUFSIZE, 0, (SOCKADDR*)&peerAddr, &addrLen);
				printf("%s", buf);
				InetNtop(AF_INET, &peerAddr.sin_addr, clientIP, 16);
				wprintf(L"[TCP/%s:%d]", clientIP, ntohs(peerAddr.sin_port));

				memset(buf, 0, BUFSIZE);

				if (retval == SOCKET_ERROR)
					return 1;
			}
		}
	}

	for (int i = 0; i < PORT_SEARCH_MAX; i++)
	{
		closesocket(socks[i]);
	}
	
	WSACleanup();
	return 0;
}