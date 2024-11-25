#include <iostream>
#include <ws2tcpip.h>
#pragma comment(lib, "ws2_32.lib") // ������ ���� ���̺귯�� ��ũ

int main() 
{
    // 1. ����(WinSock) �ʱ�ȭ
    WSADATA wsaData;
    int wsResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (wsResult != 0)
    {
        std::cerr << "WSAStartup failed: " << wsResult << std::endl;
        return -1;
    }

    // 2. ���� ����
    SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == INVALID_SOCKET) 
    {
        std::cerr << "Socket creation failed: " << WSAGetLastError() << std::endl;
        WSACleanup();
        return -1;
    }

    // 3. ���� �ּ� ����
    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(8080); // ������ ��Ʈ ��ȣ

    // ���ڿ� IP �ּҸ� ���� ���·� ��ȯ
    int addrResult = inet_pton(AF_INET, "127.0.0.1", &serverAddr.sin_addr);
    if (addrResult <= 0) 
    {
        std::cerr << "Invalid address or Address not supported!" << std::endl;
        closesocket(sock);
        WSACleanup();
        return -1;
    }

    // 4. ������ ����
    std::cerr << "Connecting... " << std::endl;
    if (connect(sock, (sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) 
    {
        std::cerr << "Connection failed: " << WSAGetLastError() << std::endl;
        closesocket(sock);
        WSACleanup();
        return -1;
    }
    std::cout << "Connected to the server!" << std::endl;

    // 5. ������ �޽��� ����
    const char* message = "Hello, server!";
    int sendResult = send(sock, message, strlen(message), 0);
    if (sendResult == SOCKET_ERROR) 
    {
        std::cerr << "Send failed: " << WSAGetLastError() << std::endl;
        closesocket(sock);
        WSACleanup();
        return -1;
    }
    std::cout << "Message sent to server!" << std::endl;

    // 6. ���� ����
    closesocket(sock);
    WSACleanup();
    return 0;
}