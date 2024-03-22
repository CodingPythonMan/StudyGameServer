#include "LockFreeStack.h"
#include <process.h>
#include <windows.h>

LockFreeStack<int> lockFreeStack;

unsigned int WorkerThread(LPVOID lpParam)
{
	while (1)
	{
		for (int i=0; i<500; i++)
			lockFreeStack.Push(i);

		for (int i=0; i<500; i++)
			lockFreeStack.Pop();
	}

	return 0;
}

int main()
{
	HANDLE Threads[3];

	for (int i = 0; i < 3; i++)
	{
		Threads[i] = (HANDLE)_beginthreadex(nullptr, 0, WorkerThread, nullptr, 0, nullptr);
	}

	WaitForMultipleObjects(3, Threads, true, INFINITE);
}