#include "RingBuffer.h"
#include <iostream>
#include <process.h>
#include <windows.h>

#define SEED 1902
#define MessageSize 82

char message[MessageSize] = "1234567890 abcdefghijklmnopqrstuvwxyz 1234567890 abcdefghijklmnopqrstuvwxyz 12345";

RingBuffer* ringBuffer = new RingBuffer(100);

unsigned int WINAPI EnqueueThread(LPVOID lpParam)
{
	int start = 0;
	int messageSize = MessageSize;

	while (1)
	{
		int FreeSize = ringBuffer->GetFreeSize();

		if (FreeSize <= 0)
			continue;

		int size = rand() % (MessageSize - start);

		if (size <= 0)
			size = MessageSize - start - 1;

		// 1. FreeSize 보다 크게 잡히면 안됨.
		if (FreeSize < size)
			size = ringBuffer->GetFreeSize();
		
		// 2. message 보다 크게 size 가 잡히면 안됨.		
		if (start + size >= MessageSize)
			size = MessageSize - start - 1;
		
		ringBuffer->Enqueue(message + start, size);

		start += size;

		start %= (MessageSize - 1);
	}

	return 0;
}

unsigned int WINAPI DequeueThread(LPVOID lpParam)
{
	while (1)
	{
		int useSize = ringBuffer->GetUseSize();

		if (useSize <= 0)
			continue;

		char* dequeueString = new char[useSize + 1];
		dequeueString[useSize] = '\0';

		ringBuffer->Dequeue(dequeueString, useSize);

		printf("%s", dequeueString);
		delete[] dequeueString;
	}

	return 0;
}

void ThreadTest()
{
	srand(SEED);

	HANDLE Threads[2];
	Threads[0] = (HANDLE)_beginthreadex(nullptr, 0, EnqueueThread, nullptr, 0, nullptr);
	Threads[1] = (HANDLE)_beginthreadex(nullptr, 0, DequeueThread, nullptr, 0, nullptr);

	WaitForMultipleObjects(2, Threads, true, INFINITE);
}

void Test()
{
	srand(SEED);
	RingBuffer* ringBuffer = new RingBuffer(500);

	int start = 0;
	int messageSize = (int)strlen(message);

	while (1)
	{
		int size = rand() % (messageSize + 1);
		char* dequeueString = new char[size + 1];
		char* peekString = new char[size + 1];
		dequeueString[size] = '\0';
		peekString[size] = '\0';

		if (start + size > messageSize)
		{
			ringBuffer->Enqueue(message + start, messageSize - start);
			ringBuffer->Enqueue(message, size - messageSize + start);

			start = start + size - messageSize;
		}
		else
		{
			ringBuffer->Enqueue(message + start, size);

			start += size;
		}

		ringBuffer->Peek(peekString, size);
		ringBuffer->Dequeue(dequeueString, size);

		if (memcmp(peekString, dequeueString, size) != 0)
		{
			__debugbreak();
		}

		printf("%s", dequeueString);

		delete[] dequeueString;
		delete[] peekString;
	}

	delete ringBuffer;
}

int main()
{
	ThreadTest();

	//Test();
}