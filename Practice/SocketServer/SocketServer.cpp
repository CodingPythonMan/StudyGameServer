#include "DomainToIP.h"

int main()
{
	// 윈속 초기화
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		return 1;

    //PrintIP();

    SOCKET listen_sock = socket(AF_INET, SOCK_STREAM, 0);
    if (listen_sock == INVALID_SOCKET)
        return 0;
    
    // Bind
    SOCKADDR_IN serverAddr;
    memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    InetPton(AF_INET, L"127.0.0.1", &serverAddr.sin_addr);
    serverAddr.sin_port = htons(14444);
    int retval = bind(listen_sock, (SOCKADDR*)&serverAddr, sizeof(serverAddr));
    if (retval == SOCKET_ERROR)
        return 0;

    // Listen
    retval = listen(listen_sock, SOMAXCONN_HINT(50000));
    if (retval == SOCKET_ERROR)
        return 0;

    while (true)
    {

    }
    
    WSACleanup();
}