#include <stdio.h>
#include <tchar.h>
#include <windows.h>

#define SLOT_NAME L"\\\\.\\mailslot\\mailbox"

int wmain(int argc, WCHAR* argv[])
{
	HANDLE hMailSlot;
	WCHAR message[50];
	DWORD bytesWritten; // number of bytes write

	SECURITY_ATTRIBUTES sa;
	sa.nLength = sizeof(sa);
	sa.lpSecurityDescriptor = nullptr;
	sa.bInheritHandle = TRUE;

	hMailSlot = CreateFileW(SLOT_NAME, GENERIC_WRITE, FILE_SHARE_READ,
		&sa, OPEN_EXISTING, // 생성방식
		FILE_ATTRIBUTE_NORMAL, nullptr);

	if (hMailSlot == INVALID_HANDLE_VALUE)
	{
		fputws(L"Unable to create mailslot!\n", stdout);
		return 1;
	}

	wprintf(L"Inheritable Handle : %d \n", hMailSlot);
	FILE* file = nullptr;
	fopen_s(&file, "InheritableHandle.txt", "wt");
	fwprintf(file, L"%d", hMailSlot);
	fclose(file);

	STARTUPINFO si = { 0, };
	PROCESS_INFORMATION pi;
	si.cb = sizeof(si);

	WCHAR command[] = L"WinConsole003.exe";

	CreateProcessW(nullptr, command, nullptr, nullptr,
		TRUE,// 핸들 테이블 상속 결정
		CREATE_NEW_CONSOLE, nullptr, nullptr, &si, &pi);

	while (1)
	{
		fputws(L"MY CMD>", stdout);
		fgetws(message, sizeof(message) / sizeof(WCHAR), stdin);

		if (!WriteFile(hMailSlot, message, wcslen(message) * sizeof(WCHAR), &bytesWritten, nullptr))
		{
			fputws(L"Unable to write!", stdout);
			CloseHandle(hMailSlot);
			return 1;
		}

		if (!wcscmp(message, L"exit"))
		{
			fputws(L"Good Bye!", stdout);
			break;
		}
	}

	CloseHandle(hMailSlot);
	return 0;
}