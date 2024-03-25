#include "MemoryPoolTest.h"
#include <process.h>

MemoryPool<int> dataPool;

unsigned int WINAPI MonitorThread(LPVOID lpParam)
{
	while (1)
	{
		printf("Memory Pool Use : %d\n", dataPool.GetUseCount());

		Sleep(1000);
	}

	return 0;
}

unsigned int WINAPI WorkerThread(LPVOID lpParam)
{
	int* ptrs[1000];

	while (1)
	{
		for(int i=0; i<1000; i++)
			ptrs[i] = dataPool.Alloc();

		for (int i = 0; i < 1000; i++)
			dataPool.Free(ptrs[i]);
	}

	return 0;
}

void ThreadUse()
{
	HANDLE Threads[6];
	for (int i = 0; i < 5; i++)
	{
		Threads[i] = (HANDLE)_beginthreadex(nullptr, 0, WorkerThread, nullptr, 0, nullptr);
	}
	Threads[5] = (HANDLE)_beginthreadex(nullptr, 0, MonitorThread, nullptr, 0, nullptr);

	WaitForMultipleObjects(6, Threads, true, INFINITE);
}

int main()
{
	//Performance();
	//BasicTest();

	ThreadUse();
}