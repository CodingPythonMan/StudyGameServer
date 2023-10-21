#include <iostream>
#include <WS2tcpip.h>
#include <windows.h>
using namespace std;

#pragma comment(lib, "ws2_32.lib")

void SetClients()
{
	int index = 0;
	SOCKET* clients = new SOCKET[50000];
	while (true)
	{
		clients[index] = socket(AF_INET, SOCK_STREAM, 0);
		if (clients[index] == INVALID_SOCKET)
			return;

		sockaddr_in addr;
		addr.sin_family = AF_INET;
		InetPton(AF_INET, L"127.0.0.1", &addr.sin_addr);
		addr.sin_port = htons(14444);
		int retval = connect(clients[index], (sockaddr*)&addr, sizeof(addr));
		if (retval == SOCKET_ERROR)
			break;

		index++;

	}
	printf("%d client connect", index);
	delete[] clients;

	printf("WSAGetLastError : %d", WSAGetLastError());
}

void SetCloseClient()
{
	int index = 0;
	SOCKET client;
	while (true)
	{
		client = socket(AF_INET, SOCK_STREAM, 0);
		if (client == INVALID_SOCKET)
			return;

		sockaddr_in addr;
		addr.sin_family = AF_INET;
		InetPton(AF_INET, L"127.0.0.1", &addr.sin_addr);
		addr.sin_port = htons(14444);
		int retval = connect(client, (sockaddr*)&addr, sizeof(addr));
		if (retval == SOCKET_ERROR)
			break;

		index++;

		closesocket(client);
	}
	printf("%d client connect", index);

	printf("WSAGetLastError : %d", WSAGetLastError());
}

int main()
{
    WSADATA wsa;
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
        return 1;

	//SetClients();
	SetCloseClient();

    WSACleanup();
}
