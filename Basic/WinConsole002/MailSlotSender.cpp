#include <stdio.h>
#include <tchar.h>
#include <windows.h>

// Sender

#define SLOT_NAME L"\\\\.\\mailslot\\mailbox"

int MailSlotSend()
{
	HANDLE hMailSlot;
	WCHAR message[50];
	DWORD bytesWritten; // number of bytes write

	hMailSlot = CreateFile(SLOT_NAME, GENERIC_WRITE, FILE_SHARE_READ,
		nullptr, OPEN_EXISTING, // 생성방식
		FILE_ATTRIBUTE_NORMAL, nullptr);

	if (hMailSlot == INVALID_HANDLE_VALUE)
	{
		fputws(L"Unable to create mailslot!\n", stdout);
		return 1;
	}

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