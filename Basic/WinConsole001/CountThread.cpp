#include <stdio.h>
#include <tchar.h>
#include <windows.h>

// CountThread.cpp
#define MAX_THREADS (1024*10)

DWORD WINAPI ThreadProcCount(LPVOID lpParam)
{
	DWORD threadNum = (DWORD)lpParam;

	while (1)
	{
		wprintf(L"thread num : %d \n", threadNum);
		Sleep(5000);
	}

	return 0;
}

DWORD countOfThread = 0;

int CountMaxThreads(int argc, WCHAR* argv[])
{
	DWORD threadID[MAX_THREADS];
	HANDLE hThread[MAX_THREADS];

	// 생성 가능한 최대 개수의 쓰레드 생성
	while (1)
	{
		hThread[countOfThread] =
			CreateThread(
				nullptr, // 디폴트 보안 속성 지정
				10, // 디폴트 스택 사이즈
				ThreadProcCount, // 쓰레드 함수
				(LPVOID)countOfThread, // 쓰레드 함수 전달인자
				0,
				&threadID[countOfThread] // 쓰레드 ID 반환
			);
		// 쓰레드 생성 확인
		if (hThread[countOfThread] == nullptr)
		{
			wprintf(L"MAXIMUM THREAD NUMBER: %d \n", countOfThread);
			break;
		}
		countOfThread++;
	}

	for (DWORD i = 0; i < countOfThread; i++)
	{
		CloseHandle(hThread[i]);
	}

	return 0;
}