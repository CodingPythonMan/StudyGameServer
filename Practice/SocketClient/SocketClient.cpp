#include <iostream>
#include <WS2tcpip.h>
#include <windows.h>
using namespace std;

#pragma comment(lib, "ws2_32.lib")

#define BUFSIZE 512

int main()
{
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
    InetPton(AF_INET, L"127.0.0.1", &serverAddr.sin_addr);
    serverAddr.sin_port = htons(1902);
    retval = connect(sock, (SOCKADDR*)&serverAddr, sizeof(serverAddr));
    if (retval == SOCKET_ERROR)
        return 0;

    // 데이터 통신에 사용할 변수
    char buf[BUFSIZE + 1];
    size_t len;

    // 서버와 데이터 통신
    while (1)
    {
        // 데이터 입력
        printf("\n[보낼 데이터] ");
        if (fgets(buf, BUFSIZE + 1, stdin) == NULL)
            break;

        // '\n\ 문자 제거
        len = strlen(buf);
        if (buf[len - 1] == '\n')
            buf[len - 1] = '\0';
        if (strlen(buf) == 0)
            break;

        // 데이터 보내기
        retval = send(sock, buf, (int)strlen(buf), 0);
        if (retval == SOCKET_ERROR)
            break;

        printf("[TCP 클라이언트] %d바이트를 보냈습니다.\n", retval);

        // 데이터 받기
        retval = recv(sock, buf, retval, 0);
        if (retval == SOCKET_ERROR)
            break;
        else if (retval == 0)
            break;

        // 받은 데이터 출력
        buf[retval] = '\0';
        printf("[TCP 클라이언트] %d바이트를 받았습니다. \n", retval);
        printf("[받은 데이터] %s\n", buf);
    }

    // close
    closesocket(sock);

    WSACleanup();
}
