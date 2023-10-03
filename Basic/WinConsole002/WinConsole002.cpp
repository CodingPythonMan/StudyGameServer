#include <stdio.h>
#include <tchar.h>
#include <windows.h>

// DuplicateHandleChildProcess.cpp

int wmain(int argc, WCHAR* argv[])
{
	HANDLE hParent = (HANDLE)_wtoi(argv[1]);
	DWORD isSuccess = WaitForSingleObject(hParent, INFINITE);

	wprintf(L"[Child Process] \n");

	if (isSuccess == WAIT_FAILED)
	{
		wprintf(L"WAIT_FAILED returned!");
		Sleep(10000);
		return -1;
	}
	else
	{
		wprintf(L"General Lee said, \"Don't inform ");
		wprintf(L"the enemy my death\"");
		Sleep(10000);
		return 0;
	}
}