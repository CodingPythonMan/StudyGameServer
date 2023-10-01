#include <stdio.h>
#include <tchar.h>
#include <windows.h>

// MailSender2_2.cpp

int wmain(int argc, WCHAR* argv[])
{
    HANDLE hMailSlot;
    WCHAR message[50];
    DWORD bytesWritten; // number of bytes write

    // 핸들을 얻는 코드
    FILE* file = nullptr;
    fopen_s(&file, "InheritableHandle,txt", "rt");
    fwscanf(file, L"%d", &hMailSlot);
    fclose(file);
    wprintf(L"Inheritable Handle : %d \n", hMailSlot);

    while (1)
    {

    }
}