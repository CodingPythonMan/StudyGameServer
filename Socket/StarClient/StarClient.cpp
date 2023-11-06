#include <iostream>
#include <WS2tcpip.h>
#include <windows.h>
#include "Protocol.h"

SOCKET sock;

int main()
{
    printf("접속할 IP 주소를 입력하세요 : ");

    char IP[16];
    scanf_s("%s", IP, 16);


    
    // 블락 논블락은 언제든 변할 수 있다.
    // connect 후 논 블락.

    while (1)
    {

        // 네트워크 : FD_SET ReadSet


        // 송신 : 좌표 변경되었다면, MoveStar send 한다.
    }
}