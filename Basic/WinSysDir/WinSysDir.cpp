#include <stdio.h>
#include <windows.h>

#define DIR_LEN MAX_PATH+1

int main()
{
    /*
    WCHAR sysDir[DIR_LEN];
    WCHAR winDir[DIR_LEN];

    // 시스템 디렉터리 정보 추출
    GetSystemDirectory(sysDir, DIR_LEN);

    // Windows 디렉터리 정보 추출
    GetWindowsDirectory(winDir, DIR_LEN);

    wprintf(L"System Dir: %s \n", sysDir);
    wprintf(L"Windows Dir: %s \n", winDir);

    return 0;*/

    STARTUPINFO si = { 0, };
    PROCESS_INFORMATION pi;

    si.cb = sizeof(si);
    si.dwFlags = STARTF_USEPOSITION | STARTF_USESIZE;
    si.dwX = 100;
    si.dwY = 200;
    si.dwXSize = 300;
    si.dwYSize = 200;
    si.lpTitle = const_cast<LPWSTR>(L"I am a boy!");
    
    WCHAR  command[] = L"WinConsole001.exe 10 20";
    WCHAR cDir[DIR_LEN];
    BOOL state;

    GetCurrentDirectory(DIR_LEN, cDir); // 현재 디렉터리 확인
    fputws(cDir, stdout);
    fputws(L"\n", stdout);

	SetCurrentDirectory(L"..\\WinConsole001\\x64\\Debug");

    GetCurrentDirectory(DIR_LEN, cDir); // 현재 디렉터리 확인
    fputws(cDir, stdout);
    fputws(L"\n", stdout);

    state = CreateProcessW(  // 프로세스 생성
        NULL,               // 실행파일의 이름
        command,            // main 함수에 전달될 문자열
        NULL, NULL, TRUE,
        CREATE_NEW_CONSOLE,
        NULL, NULL, &si, &pi
    );

    if (state != 0)
        fputws(L"Creation OK! \n", stdout);
    else
        fputws(L"Creation Error! \n", stdout);

    return 0;
}