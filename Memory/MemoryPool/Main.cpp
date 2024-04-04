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
	int ThreadID = GetCurrentThreadId();

	while(1)
	{
		for (int j = 0; j < 1000; j++)
		{
			ptrs[j] = dataPool.Alloc();
			//printf("ThreadID : %d => Alloc 0x%p\n", ThreadID, ptrs[j]);
		}
			
		for (int j = 0; j < 1000; j++)
		{
			dataPool.Free(ptrs[j]);
			//printf("ThreadID : %d => Free 0x%p\n", ThreadID, ptrs[i]);
		}
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

	printf("DataPool UseCount : %d\n", dataPool.GetUseCount());
	printf("DataPool Capacity : %d\n", dataPool.GetCapacityCount());
}

int main()
{
	//Performance();
	//BasicTest();

	ThreadUse();

	//BasicTest02();
}