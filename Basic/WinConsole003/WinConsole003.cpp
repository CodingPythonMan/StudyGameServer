#include <stdio.h>
#include <tchar.h>
#include <windows.h>

// MailSender2_2.cpp

int wmain(int argc, WCHAR* argv[])
{
    HANDLE hReadPipe, hWritePipe;

    WCHAR sendString[] = L"anonymous pipe";
    WCHAR recvString[100];

    DWORD bytesWritten;
    DWORD bytesRead;

    // pipe 생성
    CreatePipe(&hReadPipe, &hWritePipe, nullptr, 0);

    // pipe 의 한쪽 끝을 이용한 데이터 송신
    WriteFile(
        hWritePipe, sendString,
        lstrlen(sendString) * sizeof(WCHAR), &bytesWritten, nullptr);
    wprintf(L"string send: %s \n", sendString);

    // pipe 의 다른 한쪽 끝을 이용한 데이터 수신
    ReadFile(
        hReadPipe, recvString,
        bytesWritten, &bytesRead, nullptr);
    recvString[bytesRead / sizeof(WCHAR)] = 0;
    wprintf(L"strinmg recv: %s \n", recvString);

    CloseHandle(hReadPipe);
    CloseHandle(hWritePipe);
    
    return 0;
}