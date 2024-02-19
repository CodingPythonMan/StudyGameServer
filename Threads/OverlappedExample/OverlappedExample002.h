#pragma comment(lib, "ws2_32")
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>
#include <process.h>

#define SERVERPORT 9000
#define BUFSIZE 512

// 소켓 정보 저장을 위한 구조체와 변수
struct SOCKETINFO
{
	WSAOVERLAPPED overlapped;
	SOCKET sock;
	char buf[BUFSIZE + 1];
	int recvbytes;
	int sendbytes;
	WSABUF wsabuf;
};


// 비동기 입출력 시작과 처리 함수
unsigned int WINAPI WorkerThread002(LPVOID arg);
void CALLBACK CompletionRoutine(
	DWORD dwError, DWORD Transferred,
	LPWSAOVERLAPPED lpOverlapped, DWORD Flags
);

int OverlapMain002();