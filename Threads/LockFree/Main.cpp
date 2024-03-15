#include <queue>
#include <stdio.h>
#include <process.h>
#include <windows.h>

#pragma comment(lib, "winmm.lib")

#define ThreadCount 5

std::queue<int> Queue;
CRITICAL_SECTION cs;

unsigned int WINAPI WorkerThread(LPVOID lpParam)
{
	unsigned __int64* time = (unsigned __int64*)lpParam;

	while (GetTickCount64() - (*time) < 1000)
	{
		EnterCriticalSection(&cs);
		Queue.push(5);
		LeaveCriticalSection(&cs);
	}

	return 0;
}

int main()
{
	InitializeCriticalSection(&cs);

	HANDLE Threads[ThreadCount];
	unsigned __int64 time;

	time = GetTickCount64();

	while (GetTickCount64() - time < 1000)
	{
		Queue.push(5);
	}

	int CoreCount = (int)Queue.size();

	printf("단일 스레드 코어 Queue 개수 : %d\n", CoreCount);

	while (Queue.empty() == false)
	{
		Queue.pop();
	}

	for (int i = 0; i < 100000; i++)
	{
		time = GetTickCount64();

		for (int i = 0; i < ThreadCount; i++)
		{
			Threads[i] = (HANDLE)_beginthreadex(nullptr, 0, WorkerThread, &time, 0, nullptr);
		}

		WaitForMultipleObjects(ThreadCount, Threads, true, INFINITE);

		int MultiCoreCount = (int)Queue.size();

		printf("멀티 스레드 코어 Queue 개수 : %d\n", MultiCoreCount);

		while (Queue.empty() == false)
		{
			Queue.pop();
		}

		if (MultiCoreCount > CoreCount)
			__debugbreak();
	}

	DeleteCriticalSection(&cs);
}