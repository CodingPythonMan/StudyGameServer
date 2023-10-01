#include <stdio.h>
#include <tchar.h>
#include <windows.h>

// PartAdder.cpp

int wmain(int argc, WCHAR* argv[])
{
	if (argc != 3)
	{
		return -1;
	}

	DWORD start = _wtoi(argv[1]);
	DWORD end = _wtoi(argv[2]);

	DWORD total = 0;

	for (DWORD i = start; i <= end; i++)
		total += i;

	return total;
}