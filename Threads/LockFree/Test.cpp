#include "Test.h"

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
		for (int i = 0; i < 1000; i++)
			lockFreeStack.Push(i);

		for (int i = 0; i < 1000; i++)
			lockFreeStack.Pop();
	}

	return 0;
}

void LockFreeStackTestCode()
{
	HANDLE Threads[4];

	for (int i = 0; i < 3; i++)
	{
		Threads[i] = (HANDLE)_beginthreadex(nullptr, 0, StackWorkerThread, nullptr, 0, nullptr);
	}

	Threads[3] = (HANDLE)_beginthreadex(nullptr, 0, StackMonitorThread, nullptr, 0, nullptr);

	WaitForMultipleObjects(4, Threads, true, INFINITE);
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