#include "Test.h"

#define THREAD_COUNT 5

LockFreeStack<int> lockFreeStack;
LockFreeQueue<int> lockFreeQueue;

unsigned int StackMonitorThread(LPVOID lpParam)
{
	while (1)
	{
		printf("Size : %d\n", lockFreeStack.GetSize());

		Sleep(1000);
	}
}

unsigned int StackWorkerThread(LPVOID lpParam)
{
	while (1)
	{
		for (int i = 1; i < 501; i++)
			lockFreeStack.Push(i);

		for (int i = 1; i < 501; i++)
			lockFreeStack.Pop();
	}

	return 0;
}

void LockFreeStackTestCode()
{
	HANDLE Threads[THREAD_COUNT+1];

	for (int i = 0; i < THREAD_COUNT; i++)
	{
		Threads[i] = (HANDLE)_beginthreadex(nullptr, 0, StackWorkerThread, nullptr, 0, nullptr);
	}

	Threads[THREAD_COUNT] = (HANDLE)_beginthreadex(nullptr, 0, StackMonitorThread, nullptr, 0, nullptr);

	WaitForMultipleObjects(THREAD_COUNT+1, Threads, true, INFINITE);
}

unsigned int QueueMonitorThread(LPVOID lpParam)
{
	while (1)
	{
		printf("Size : %d\n", lockFreeQueue.GetSize());

		Sleep(1000);
	}
}

unsigned int QueueWorkerThread(LPVOID lpParam)
{
	int data;

	while (1)
	{
		for (int i = 0; i < 5; i++)
			lockFreeQueue.Enqueue(i);

		for (int i = 0; i < 5; i++)
			lockFreeQueue.Dequeue(data);
	}

	return 0;
}

void LockFreeQueueTestCode()
{
	HANDLE Threads[6];

	for (int i = 0; i < 5; i++)
	{
		Threads[i] = (HANDLE)_beginthreadex(nullptr, 0, QueueWorkerThread, nullptr, 0, nullptr);
	}

	Threads[5] = (HANDLE)_beginthreadex(nullptr, 0, QueueMonitorThread, nullptr, 0, nullptr);

	WaitForMultipleObjects(6, Threads, true, INFINITE);
}