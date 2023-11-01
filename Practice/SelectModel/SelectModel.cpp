#include <iostream>
#include <ws2tcpip.h>
#include <windows.h>

#pragma comment(lib, "ws2_32")

#define SERVERPORT 1902
#define BUFSIZE 512

// 소켓 정보 저장을 위한 구조체와 변수
int main()
{
    int retval;

    // 윈속 초기화
    WSADATA wsa;
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
        return 1;

    // socket
    SOCKET listenSock = socket(AF_INET, SOCK_STREAM, 0);
    if (listenSock == INVALID_SOCKET)
        return 1;

    // bind
    SOCKADDR_IN serverAddr;
    memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    serverAddr.sin_port = htons(SERVERPORT);
    retval = bind(listenSock, (SOCKADDR*)&serverAddr, sizeof(serverAddr));

    if (retval == SOCKET_ERROR)
        return 1;

    // listen
    retval = listen(listenSock, SOMAXCONN);
    if (retval == SOCKET_ERROR)
        return 1;

    // 논블로킹 소켓으로 전환
    u_long on = 1;
    retval = ioctlsocket(listenSock, FIONBIO, &on);
    if (retval == SOCKET_ERROR)
        return 1;

    // 데이터 통신에 사용할 변수
    FD_SET rset, wset;

}