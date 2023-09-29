#include <stdio.h>
#include <tchar.h>
#include <windows.h>

int wmain(int argc, WCHAR* argv[])
{
    STARTUPINFO si = { 0, };
    PROCESS_INFORMATION pi;
    si.cb = sizeof(si);

    TCHAR command[] = L"WinConsole002.exe";

    CreateProcessW(
        NULL, command, NULL, NULL, TRUE,
        0, NULL, NULL, &si, &pi
    );

    DWORD timing = 0;
    while (1)
    {
        for (DWORD i = 0; i < 10000; i++)
            for (DWORD i = 0; i < 10000; i++)

        fputws(L"Parent \n", stdout);

        timing += 1;
        if (timing == 2)
            SetPriorityClass(pi.hProcess, NORMAL_PRIORITY_CLASS);
    }

    return 0;
}