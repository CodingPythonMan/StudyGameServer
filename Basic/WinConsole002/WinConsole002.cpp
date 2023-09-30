#include <stdio.h>
#include <tchar.h>
#include <windows.h>

int wmain(int argc, WCHAR* argv[])
{
	DWORD n = 0;
	while (n < 100)
	{
		for (DWORD i = 0; i < 10000; i++)
			for (DWORD i = 0; i < 10000; i++) // Busy Waiting!!
				fputws(L"WinConsole002.exe \n", stdout);
		n++;
	}

	return 0;
}