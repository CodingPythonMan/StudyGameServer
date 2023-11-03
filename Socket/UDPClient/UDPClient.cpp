#include <iostream>
#include <WS2tcpip.h>
#include <locale.h>
using namespace std;

#pragma comment(lib, "ws2_32.lib")

#define PORT_SEARCH_MAX 99
#define SEARCH_PORT 10001

int main()
{
	_wsetlocale(LC_ALL, L"korean");

	int retval;
	WCHAR messages[10][50] = { L"여기는 형제방",  L"여기는 방",  L"방방",
	 L"링겜" ,  L"스타" ,  L"초보만" ,
	 L"형제방" ,  L"허허" ,  L"왓" ,
	 L"응?" };

	// 윈속 초기화
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		return 1;

	// socket
	SOCKET sock = socket(AF_INET, SOCK_DGRAM, 0);
	if (sock == INVALID_SOCKET)
		return -1;

	// 소켓 구조 구조체 동기화
	SOCKADDR_IN remoteAddrs[10];
	for (int i = 1; i < 10; i++)
	{
		memset(&remoteAddrs[i], 0, sizeof(remoteAddrs[i]));
		remoteAddrs[i].sin_family = AF_INET;
		remoteAddrs[i].sin_port = htons(SEARCH_PORT + (rand() % 100));
		InetPton(AF_INET, L"127.0.0.1", &(remoteAddrs[i].sin_addr));
	}
	remoteAddrs[0].sin_family = AF_INET;
	remoteAddrs[0].sin_port = htons(1902);
	InetPton(AF_INET, L"127.0.0.1", &(remoteAddrs[0].sin_addr));

	retval = sendto(sock, (char*)messages[0], 50, 0, (SOCKADDR*)&remoteAddrs[0], (int)sizeof(remoteAddrs[0]));
	for (int i = 1; i < 10; i++)
	{
		retval = sendto(sock, (char*)messages[i], 50, 0, (SOCKADDR*)&remoteAddrs[i], (int)sizeof(remoteAddrs[i]));
	}

	if (retval == SOCKET_ERROR)
		return -1;

	closesocket(sock);

	WSACleanup();

	return 0;
}