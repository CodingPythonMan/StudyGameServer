#include <stdio.h>
#include <tchar.h>
#include <windows.h>

// DuplicateHandle.cpp

int wmain(int argc, WCHAR* argv[])
{
	HANDLE hProcess;
	WCHAR cmdString[1024];

	DuplicateHandle(GetCurrentProcess(), GetCurrentProcess(),
		GetCurrentProcess(), &hProcess, 0,
		TRUE, DUPLICATE_SAME_ACCESS);

	swprintf_s(cmdString, L"%s %u", L"WinConsole002.exe", (unsigned)hProcess);

	STARTUPINFO si = { 0, };
	PROCESS_INFORMATION pi = { 0, };
	si.cb = sizeof(si);

	BOOL isSuccess = CreateProcess(nullptr, cmdString, nullptr, nullptr,TRUE,
		CREATE_NEW_CONSOLE, nullptr, nullptr, &si, &pi);

	if (isSuccess == FALSE)
	{
		wprintf(L"CreateProcess failed \n");
		return -1;
	}

	CloseHandle(pi.hProcess);
	CloseHandle(pi.hThread);

	wprintf(L"[Parent Process]\n");
	wprintf(L"Ooooooooooooooooooooooopps! \n");

	return 0;
}