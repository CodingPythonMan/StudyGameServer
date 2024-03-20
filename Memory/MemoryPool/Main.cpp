#include "MemoryPoolTest.h"
#include <process.h>

struct Data {
	int index;
	int data;
};

MemoryPool<Data> dataPool;

unsigned int WINAPI WorkerThread(LPVOID lpParam)
{
	Data** datas = new Data*[100000];

	while (1)
	{
		for(int i=0; i<100000; i++)
			datas[i] = dataPool.Alloc();

		for (int i = 0; i < 100000; i++)
			dataPool.Free(datas[i]);
	}

	return 0;
}

int main()
{
	//Performance();
	//BasicTest();

	HANDLE Threads[5];
	for (int i = 0; i < 5; i++)
	{
		Threads[i] = (HANDLE)_beginthreadex(nullptr, 0, WorkerThread, nullptr, 0, nullptr);
	}

	WaitForMultipleObjects(5, Threads, true, INFINITE);
}