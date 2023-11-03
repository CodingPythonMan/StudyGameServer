#include <iostream>
#include <ws2tcpip.h>
#include <windows.h>

#pragma comment(lib, "ws2_32")

#define SERVERPORT 1902
#define BUFSIZE 512

struct SOCKETINFO
{
    SOCKET sock;
    char buf[BUFSIZE + 1];
};

int TotalSockets = 0;
SOCKETINFO* SocketInfoArr[FD_SETSIZE];

// 소켓 정보 추가
BOOL AddSocketInfo(SOCKET sock)
{
    if (TotalSockets >= FD_SETSIZE)
    {
        printf("[오류] 소켓 정보를 추가할 수 없습니다!\n");
        return FALSE;
    }

    SOCKETINFO* ptr = new SOCKETINFO;
    if (ptr == nullptr)
    {
        printf("[오류] 메모리가 부족합니다!\n");
        return FALSE;
    }

    ptr->sock = sock;
    SocketInfoArr[TotalSockets] = ptr;
    TotalSockets++;

    return TRUE;
}

// 소켓 정보 삭제
void RemoveSocketInfo(int index)
{
    SOCKETINFO* ptr = SocketInfoArr[index];

    // 클라이언트 정보 얻기
    SOCKADDR_IN clientAddr;
    int addrLen = sizeof(clientAddr);
    getpeername(ptr->sock, (SOCKADDR*)&clientAddr, &addrLen);
    WCHAR clientIP[16] = { 0 };
	InetNtop(AF_INET, &clientAddr.sin_addr, clientIP, 16);
	wprintf(L"\n[TCP 서버] 클라이언트 종료: IP 주소=%s, 포트번호=%d\n", clientIP, ntohs(clientAddr.sin_port));

    closesocket(ptr->sock);
    delete ptr;

    if (index != (TotalSockets - 1))
        SocketInfoArr[index] = SocketInfoArr[TotalSockets - 1];

    TotalSockets--;
}

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
    FD_SET rset;
    SOCKET clientSock;
    SOCKADDR_IN clientAddr;
    int addrLen;
    WCHAR clientIP[16] = {0};

    while (1)
    {
        // 소켓 셋 초기화
        FD_ZERO(&rset);
        FD_SET(listenSock, &rset);
        for (int i = 0; i < TotalSockets; i++)
        {
            FD_SET(SocketInfoArr[i]->sock, &rset);
        }

        // select()
        retval = select(0, &rset, nullptr, nullptr, nullptr);
        if (retval == SOCKET_ERROR)
            return 1;

        // 소켓 셋 검사(1) : 클라이언트 접속 수용
        if (FD_ISSET(listenSock, &rset)) {
            addrLen = sizeof(clientAddr);
            clientSock = accept(listenSock, (SOCKADDR*)&clientAddr, &addrLen);
            if (clientSock == INVALID_SOCKET)
                return 1;
            else
            {
                InetNtop(AF_INET, &clientAddr.sin_addr, clientIP, 16);
                wprintf(L"\n[TCP 서버] 클라이언트 접속: IP 주소=%s, 포트번호=%d\n", clientIP, ntohs(clientAddr.sin_port));

                // 소켓 정보 추가
                AddSocketInfo(clientSock);
            }
        }

        // 소켓 셋 검사(2) : 데이터 통신
        for (int i = 0; i < TotalSockets; i++)
        {
            SOCKETINFO* ptr = SocketInfoArr[i];
            if (FD_ISSET(ptr->sock, &rset))
            {
                // 데이터 받기
                retval = recv(ptr->sock, ptr->buf, BUFSIZE, 0);
                if (retval == SOCKET_ERROR) {
                    RemoveSocketInfo(i);
                    continue;
                }
                else if(retval == 0)
                {
                    RemoveSocketInfo(i);
                    continue;
                }

                // 받은 데이터 출력
                addrLen = sizeof(clientAddr);
                getpeername(ptr->sock, (SOCKADDR*)&clientAddr, &addrLen);
                ptr->buf[retval] = '\0';
                InetNtop(AF_INET, &clientAddr.sin_addr, clientIP, 16);
                wprintf(L"[TCP/%s:%d]", clientIP, ntohs(clientAddr.sin_port));
                printf(" %s\n", ptr->buf);

                // 우린 write_set 을 안 쓰므로 여기서 Send 해야한다.
                retval = send(ptr->sock, ptr->buf, retval, 0);
                if (retval == SOCKET_ERROR)
                {
                    RemoveSocketInfo(i);
                    return;
                }
            }
        }
    }

    // 윈속 종료
    WSACleanup();
    return 0;
}