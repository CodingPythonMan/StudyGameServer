#include <stdio.h>
#include <tchar.h>
#include <windows.h>

int wmain(int argc, WCHAR* argv[])
{
	DWORD val1, val2;
	val1 = _wtoi(argv[1]);
	val2 = _wtoi(argv[2]);

	wprintf(L"%d + %d = %d \n", val1, val2, val1+val2);

	_gettchar();
	return 0;
}