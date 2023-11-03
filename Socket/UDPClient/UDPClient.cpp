#include <iostream>
#include <WS2tcpip.h>
#include <locale.h>
using namespace std;

#pragma comment(lib, "ws2_32.lib")

#define PORT_SEARCH_MAX 99
#define SEARCH_PORT 10001

int main()
{
	int retval;
	char protocol[11] = { (char)0xff, (char)0xee, (char)0xdd, 
		(char)0xaa, (char)0x00, (char)0x99, 
		(char)0x77, (char)0x55, (char)0x33, 
		(char)0x11, '\0' };

	_wsetlocale(LC_ALL, L"korean");

	// 윈속 초기화
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		return 1;

	// socket
	SOCKET sock = socket(AF_INET, SOCK_DGRAM, 0);
	if (sock == INVALID_SOCKET)
		return -1;

	// 소켓 구조 구조체 동기화
	SOCKADDR_IN remoteAddrs[PORT_SEARCH_MAX+1];
	for (int i = 0; i < PORT_SEARCH_MAX; i++)
	{
		memset(&remoteAddrs[i], 0, sizeof(remoteAddrs[i]));
		remoteAddrs[i].sin_family = AF_INET;
		remoteAddrs[i].sin_port = htons(SEARCH_PORT + i);
		InetPton(AF_INET, L"255.255.255.255", &(remoteAddrs[i].sin_addr));
	}
	
	BOOL bEnable = TRUE;
	retval = setsockopt(sock, SOL_SOCKET, SO_BROADCAST, (char*)&bEnable, sizeof(bEnable));
	if (retval == SOCKET_ERROR)
		return -1;

	for (int i = 0; i < PORT_SEARCH_MAX; i++)
	{
		retval = sendto(sock, protocol, 10, 0, (SOCKADDR*)(&remoteAddrs[i]), sizeof(remoteAddrs[i]));
	}

	if (retval == SOCKET_ERROR)
		return -1;

	closesocket(sock);

	WSACleanup();

	return 0;
}