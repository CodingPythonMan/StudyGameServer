#include "SetClients.h"
/*#include <cstdio>
#include <WS2tcpip.h>
#include <windows.h>

#define FILE_DATA 100000

struct st_PACKET_HEADER
{
	DWORD	dwPacketCode;		// 0x11223344	우리의 패킷확인 고정값

	WCHAR	szName[32];		// 본인이름, 유니코드 NULL 문자 끝
	WCHAR	szFileName[128];	// 파일이름, 유니코드 NULL 문자 끝
	int	iFileSize;
};*/

int main()
{
	SetClients();
	/*
	// 패킷 헤더 준비
	st_PACKET_HEADER packetHeader;
	packetHeader.dwPacketCode = 0x11223344;
	wcscpy_s(packetHeader.szName, L"JuYoungKim");
	packetHeader.szName[wcslen(packetHeader.szName)] = '\0';
	wcscpy_s(packetHeader.szFileName, L"nanachi.jpg");
	packetHeader.szFileName[wcslen(packetHeader.szFileName)] = '\0';

	// 파일 데이터 준비
	FILE* file;
	fopen_s(&file, "nanachi.jpg", "rb");

	if (file == nullptr)
		return -1;

	char* fileData = (char*)malloc(FILE_DATA);
	if (fileData == 0)
		return -1;

	fread(fileData, FILE_DATA, 1, file);

	fseek(file, 0L, SEEK_END);
	int size = (int)ftell(file);

	fseek(file, 0L, SEEK_SET);

	packetHeader.iFileSize = size;

	int retval;

	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		return 1;

	// socket
	SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock == INVALID_SOCKET)
		return 0;

	// connect
	SOCKADDR_IN serverAddr;
	memset(&serverAddr, 0, sizeof(serverAddr));
	serverAddr.sin_family = AF_INET;
	InetPton(AF_INET, L"106.245.38.108", &serverAddr.sin_addr);
	serverAddr.sin_port = htons(10010);
	retval = connect(sock, (SOCKADDR*)&serverAddr, sizeof(serverAddr));
	if (retval == SOCKET_ERROR)
		return 0;

	// 헤더 데이터 전달
	retval = send(sock, (char*)&packetHeader, sizeof(st_PACKET_HEADER), 0);
	if (retval == SOCKET_ERROR)
		return -1;

	// 파일 데이터 전달
	retval = send(sock, fileData, size, 0);
	if (retval == SOCKET_ERROR)
		return -1;

	char buf[8];
	// 데이터 받기
	retval = recv(sock, buf, 8, 0);
	if (retval == SOCKET_ERROR)
		return -1;
	else if (retval == 0)
		return -1;

	free(fileData);
	fclose(file);

	// close
	closesocket(sock);

	WSACleanup();

	return 0;*/
}