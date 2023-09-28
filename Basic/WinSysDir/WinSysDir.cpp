#include <stdio.h>
#include <windows.h>

#define DIR_LEN MAX_PATH+1

int main()
{
    WCHAR sysDir[DIR_LEN];
    WCHAR winDir[DIR_LEN];

    // 시스템 디렉터리 정보 추출
    GetSystemDirectory(sysDir, DIR_LEN);

    // Windows 디렉터리 정보 추출
    GetWindowsDirectory(winDir, DIR_LEN);

    wprintf(L"System Dir: %s \n", sysDir);
    wprintf(L"Windows Dir: %s \n", winDir);

    return 0;
}