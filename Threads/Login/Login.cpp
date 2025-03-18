#include <iostream>
#include <WS2tcpip.h>
#include <windows.h>
#include <process.h>

#pragma comment(lib, "ws2_32.lib")

#define SERVERPORT 2025

HANDLE iocp;
SOCKET sock;

void RegisterThread(SOCKET& sock)
{
	if (nullptr == CreateIoCompletionPort((HANDLE)sock, iocp, 0, 0))
	{
		printf("failed - CreateIoCompletionPort:%d", GetLastError());
		return;
	}
}

unsigned int WINAPI AcceptThread(LPVOID lpParam)
{
	while (1)
	{
		// 데이터 통신에 사용할 변수
		SOCKET clientSock;
		SOCKADDR_IN clientAddr;
		int addrLen;
		DWORD recvbytes, flags;
		WCHAR IP[16];

		// accept
		addrLen = sizeof(clientAddr);
		clientSock = accept(sock, (SOCKADDR*)&clientAddr, &addrLen);
		if (clientSock == INVALID_SOCKET)
		{
			printf("혹시 WSASocket 으로 생성하면 비동기니?\n");
			continue;
		}

		InetNtop(AF_INET, &(clientAddr.sin_addr), IP, 16);
		wprintf(L"\n[TCP Server] Client Connect : IP Address=%s, Port=%d\n",
			IP, ntohs(clientAddr.sin_port));

		RegisterThread(sock);
	}
}

int main()
{
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
	{
		return 0;
	}

	sock = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, nullptr, 0, WSA_FLAG_OVERLAPPED);
	if (sock == INVALID_SOCKET)
	{
		return 0;
	}

	// bind
	SOCKADDR_IN serverAddr;
	memset(&serverAddr, 0, sizeof(serverAddr));
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	serverAddr.sin_port = htons(SERVERPORT);
	int retval = bind(sock, (SOCKADDR*)&serverAddr, sizeof(serverAddr));
	if (retval == SOCKET_ERROR)
		return 1;

	// listen
	retval = listen(sock, SOMAXCONN);
	if (retval == SOCKET_ERROR)
	{
		return 1;
	}

	// 스레드 생성
	HANDLE hThread = (HANDLE)_beginthreadex(nullptr, 0, AcceptThread, nullptr, 0, nullptr);
	if (hThread == NULL)
	{
		return 1;
	}

	printf("Hello Login\n");
	while (1)
	{
		Sleep(200);
	}

	// 윈속 종료
	WSACleanup();

	return 0;
}