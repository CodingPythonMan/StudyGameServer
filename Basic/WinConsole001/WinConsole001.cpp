#include <stdio.h>
#include <tchar.h>
#include <windows.h>

// ThreadAdderTwo

static int total = 0;

DWORD WINAPI ThreadProc(LPVOID lpParam)
{
	DWORD* nPtr = (DWORD*)lpParam;

	DWORD numOne = *nPtr;
	DWORD numTwo = *(nPtr + 1);

	for (DWORD i = numOne; i <= numTwo; i++)
	{
		total += i;
	}

	return 0;
}

int wmain(int argc, WCHAR* argv[])
{
	DWORD threadID[3];
	HANDLE hThread[3];

	DWORD paramThread[] = { 1,3,4,7,8,10 };

	hThread[0] = CreateThread(
		nullptr, 0, ThreadProc, (LPVOID)(&paramThread[0]), 0, &threadID[0]
	);

	hThread[1] = CreateThread(
		nullptr, 0, ThreadProc, (LPVOID)(&paramThread[2]), 0, &threadID[1]
	);

	hThread[2] = CreateThread(
		nullptr, 0, ThreadProc, (LPVOID)(&paramThread[4]), 0, &threadID[2]
	);

	if (hThread[0] == nullptr || hThread[1] == nullptr || hThread[2] == nullptr)
	{
		wprintf(L"Thread creation fault! \n");
		return -1;
	}

	WaitForMultipleObjects(3, hThread, TRUE, INFINITE);

	wprintf(L"total (1~10): %d \n", total);

	CloseHandle(hThread[0]);
	CloseHandle(hThread[1]);
	CloseHandle(hThread[2]);

	return 0;
}