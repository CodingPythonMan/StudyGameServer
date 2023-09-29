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

	while (1)
	{
		for (DWORD i = 0; i < 10000; i++)
			for (DWORD i = 0; i < 10000; i++);

		fputws(L"WinConsole001.exe \n", stdout);
	}
	return 0;
}