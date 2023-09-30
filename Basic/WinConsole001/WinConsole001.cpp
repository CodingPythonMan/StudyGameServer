#include <stdio.h>
#include <tchar.h>
#include <windows.h>

int wmain(int argc, WCHAR* argv[])
{
	STARTUPINFO si = { 0, };
	PROCESS_INFORMATION pi;
	si.cb = sizeof(si);
	
	WCHAR command[] = L"WinConsole002.exe";
	CreateProcessW(NULL, command, NULL, NULL,
		TRUE, 0, NULL, NULL, &si, &pi);

	CloseHandle(pi.hProcess);
	
	return 0;
}