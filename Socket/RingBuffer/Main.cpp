#include "RingBuffer.h"
#include <iostream>
#include <process.h>
#include <windows.h>

#define SEED 1902

#define MessageMaxSize 200
#define MessageMinSize 5
#define MessageSize 81

char message[MessageSize + 1] = "1234567890 abcdefghijklmnopqrstuvwxyz 1234567890 abcdefghijklmnopqrstuvwxyz 12345";

RingBuffer* ringBuffer = new RingBuffer(10000);

unsigned int WINAPI EnqueueThread(LPVOID lpParam)
{
	int start = 0;
	
	while (1)
	{
		int size = rand() % (MessageSize + 1);

		if (ringBuffer->GetFreeSize() < size)
			__debugbreak();

		if (start + size > MessageSize)
		{
			ringBuffer->Enqueue(message + start, MessageSize - start);
			ringBuffer->Enqueue(message, size - MessageSize + start);

			start += size - MessageSize;
		}
		else
		{
			ringBuffer->Enqueue(message + start, size);

			start += size;
		}

		Sleep(20);
	}

	return 0;
}

unsigned int WINAPI DequeueThread(LPVOID lpParam)
{
	while (1)
	{
		int useSize = ringBuffer->GetUseSize();
			
		if (useSize > 0)
		{
			char* dequeueString = new char[useSize + 1];
			dequeueString[useSize] = '\0';

			ringBuffer->Dequeue(dequeueString, useSize);

			printf("%s", dequeueString);
			delete[] dequeueString;
		}

		Sleep(20);
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