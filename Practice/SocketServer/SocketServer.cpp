#include <WS2tcpip.h>
#include <windows.h>
#include <iostream>
#include <locale.h>
#include "SetServer.h"
using namespace std;

#pragma comment(lib, "ws2_32.lib")

#define BUFSIZE 100000

struct st_PACKET_HEADER
{
	DWORD	dwPacketCode;		// 0x11223344	우리의 패킷확인 고정값

	WCHAR	szName[32];		// 본인이름, 유니코드 NULL 문자 끝
	WCHAR	szFileName[128];	// 파일이름, 유니코드 NULL 문자 끝
	int	iFileSize;
};

int main()
{
	SetDefaultEchoServer();

	/*
	_wsetlocale(LC_ALL, L"korean");

	// 윈속 초기화
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		return 1;

	SOCKET listenSock = socket(AF_INET, SOCK_STREAM, 0);
	if (listenSock == INVALID_SOCKET)
		return 0;

	// Bind
	SOCKADDR_IN serverAddr;
	memset(&serverAddr, 0, sizeof(serverAddr));
	serverAddr.sin_family = AF_INET;
	InetPton(AF_INET, L"127.0.0.1", &serverAddr.sin_addr);
	serverAddr.sin_port = htons(1902);
	int retval = bind(listenSock, (SOCKADDR*)&serverAddr, sizeof(serverAddr));
	if (retval == SOCKET_ERROR)
		return 0;

	// Listen
	retval = listen(listenSock, SOMAXCONN);
	if (retval == SOCKET_ERROR)
		return 0;

	// 데이터 통신에 사용할 변수
	SOCKET clientSock;
	SOCKADDR_IN clientAddr;
	int addrLen;
	char* buf = (char*)malloc(BUFSIZE);
	char* packData = (char*)malloc(sizeof(st_PACKET_HEADER));
	char* fileData = (char*)malloc(BUFSIZE);
	WCHAR clientIP[16] = { 0 };

	while (true)
	{
		// accept()
		addrLen = sizeof(clientAddr);
		clientSock = accept(listenSock, (SOCKADDR*)&clientAddr, &addrLen);
		if (clientSock == INVALID_SOCKET)
			break;

		// 접속한 클라이언트 정보 출력
		InetNtop(AF_INET, &clientAddr.sin_addr, clientIP, 16);
		wprintf(L"\n[TCP 서버] 클라이언트 접속: IP 주소=%s, 포트번호=%d\n", clientIP, ntohs(clientAddr.sin_port));

		// 클라이언트와 데이터 통신
		if (buf == 0 || fileData == 0)
			break;

		// 데이터 받기
		retval = recv(clientSock, buf, BUFSIZE, 0);

		st_PACKET_HEADER packHeader;
		int pos = 0;
		memcpy_s(&packHeader.dwPacketCode, sizeof(packHeader.dwPacketCode), buf, sizeof(packHeader.dwPacketCode));
		pos += sizeof(packHeader.dwPacketCode);
		memcpy_s(packHeader.szName, 64, buf + pos, 64);
		pos += 64;
		memcpy_s(packHeader.szFileName, 128 * 2, buf + pos, 128 * 2);
		pos += (128 * 2);
		memcpy_s(&packHeader.iFileSize, 4, buf + pos, 4);
		pos += 4;

		memcpy_s(fileData, packHeader.iFileSize, buf + pos, packHeader.iFileSize);

		FILE* file;
		_wfopen_s(&file, packHeader.szFileName, L"wb");

		if (file == nullptr)
			break;

		fwrite(fileData, 1, packHeader.iFileSize, file);

		fclose(file);

		if (retval == SOCKET_ERROR)
			break;
		else if (retval == 0)
			break;

		// close
		closesocket(clientSock);
		wprintf(L"\n[TCP 서버] 클라이언트 종료: IP 주소=%s, 포트번호=%d\n", clientIP, ntohs(clientAddr.sin_port));
	}

	free(buf);
	free(packData);
	free(fileData);

	closesocket(listenSock);

	WSACleanup();

	return 0;*/
}