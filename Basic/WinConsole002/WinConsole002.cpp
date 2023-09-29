#include <stdio.h>
#include <tchar.h>
#include <windows.h>

int wmain(int argc, WCHAR* argv[])
{
	SetPriorityClass(GetCurrentProcess(), HIGH_PRIORITY_CLASS);

	while (1)
	{
		for (DWORD i = 0; i < 10000; i++)
			for (DWORD i = 0; i < 10000; i++)

		fputws(L"WinConosle002.exe \n", stdout);
	}

	return 0;
}