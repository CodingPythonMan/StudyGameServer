#include <stdio.h>
#include <tchar.h>
#include <windows.h>

// Receiver

#define SLOT_NAME L"\\\\.\\mailslot\\mailbox"

int MailSlotReceive()
{
	HANDLE hMailSlot;
	WCHAR messageBox[50];
	DWORD bytesRead; // number of bytes read

	// mailslot 생성
	hMailSlot = CreateMailslot(SLOT_NAME, 0, MAILSLOT_WAIT_FOREVER, NULL);

	if (hMailSlot == INVALID_HANDLE_VALUE)
	{
		fputws(L"Unable to create mailslot!\n", stdout);
		return 1;
	}

	// Message 수신
	fputws(L"***** Message *****\n", stdout);
	while (1)
	{
		if (!ReadFile(hMailSlot, messageBox, sizeof(WCHAR) * 50, &bytesRead, nullptr))
		{
			fputws(L"Unable to read!", stdout);
			CloseHandle(hMailSlot);
			return 1;
		}

		if (!wcsncmp(messageBox, L"exit", 4))
		{
			fputws(L"Good Bye!", stdout);
			break;
		}

		messageBox[bytesRead / sizeof(WCHAR)] = 0; // nullptr 삽입
		fputws(messageBox, stdout);
	}

	CloseHandle(hMailSlot);
	return 0;
}