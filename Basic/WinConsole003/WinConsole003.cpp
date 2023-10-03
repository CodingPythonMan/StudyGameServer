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
    int iHandle;
    fopen_s(&file, "InheritableHandle.txt", "rt");
    fwscanf_s(file, L"%d", &iHandle);
    hMailSlot = (HANDLE)iHandle;
    fclose(file);
    wprintf(L"Inheritable Handle : %d \n", hMailSlot);

    while (1)
    {
        fputws(L"MY CMD>", stdout);
        fgetws(message, sizeof(message) / sizeof(WCHAR), stdin);

        if (!WriteFile(hMailSlot, message, wcslen(message) * sizeof(WCHAR), &bytesWritten, nullptr))
        {
            fputws(L"Unable to write!", stdout);
            getwchar();
            CloseHandle(hMailSlot);
            return 1;
        }

        if (!wcscmp(message, L"exit"))
        {
            fputws(L"Good Bye!", stdout);
            break;
        }
    }

	CloseHandle(hMailSlot);
	return 0;
}