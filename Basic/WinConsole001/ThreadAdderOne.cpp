#include <stdio.h>
#include <tchar.h>
#include <windows.h>

// ThreadAdderOne.cpp

DWORD WINAPI ThreadAdderOne(LPVOID lpParam)
{
	DWORD* nPtr = (DWORD*)lpParam;

	DWORD numOne = *nPtr;
	DWORD numTwo = *(nPtr + 1);

	DWORD total = 0;
	for (DWORD i = numOne; i <= numTwo; i++)
	{
		total += i;
	}

	return total;
}

int ThreadAddThree(int argc, WCHAR* argv[])
{
	DWORD threadID[3];
	HANDLE hThread[3];

	DWORD paramThread[] = { 1,3,4,7,8,10 };
	DWORD total = 0;
	DWORD result = 0;

	hThread[0] = CreateThread(
		nullptr, 0, ThreadAdderOne, (LPVOID)(&paramThread[0]), 0, &threadID[0]
	);

	hThread[1] = CreateThread(
		nullptr, 0, ThreadAdderOne, (LPVOID)(&paramThread[2]), 0, &threadID[1]
	);

	hThread[2] = CreateThread(
		nullptr, 0, ThreadAdderOne, (LPVOID)(&paramThread[4]), 0, &threadID[2]
	);

	if (hThread[0] == nullptr || hThread[1] == nullptr || hThread[2] == nullptr)
	{
		wprintf(L"Thread creation fault! \n");
		return -1;
	}

	WaitForMultipleObjects(3, // �� 3���� Ŀ�� ������Ʈ�� ����
		hThread, // Ŀ�� ������Ʈ �ڵ��� �迭 ����
		TRUE, // ��� Ŀ�� ������Ʈ�� signaled ���°� �Ǳ⸦
		INFINITE // ������ ��ٸ���.
	);

	GetExitCodeThread(hThread[0], &result);
	total += result;

	GetExitCodeThread(hThread[1], &result);
	total += result;

	GetExitCodeThread(hThread[2], &result);
	total += result;

	wprintf(L"total (1~10): %d \n", total);

	CloseHandle(hThread[0]);
	CloseHandle(hThread[1]);
	CloseHandle(hThread[2]);

	return 0;
}