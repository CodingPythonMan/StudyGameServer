#include "RingBuffer.h"
#include <iostream>
#include <process.h>
#include <windows.h>

#define SEED 1902
char message[82] = "1234567890 abcdefghijklmnopqrstuvwxyz 1234567890 abcdefghijklmnopqrstuvwxyz 12345";

RingBuffer* ringBuffer = new RingBuffer(1000);

int size = 40;

unsigned int WINAPI EnqueueThread(LPVOID lpParam)
{
	int start = 0;
	int messageSize = (int)strlen(message);

	while (1)
	//for(int i=0; i<30000000; i++)
	{
		if (ringBuffer->GetFreeSize() < size)
			continue;

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
	}

	return 0;
}

unsigned int WINAPI DequeueThread(LPVOID lpParam)
{
	while (1)
	//for (int i = 0; i < 30000000; i++)
	{
		if (ringBuffer->GetUseSize() < size)
			continue;

		char* dequeueString = new char[size+1];
		dequeueString[size] = '\0';

		ringBuffer->Dequeue(dequeueString, size);

		printf("%s", dequeueString);
		delete[] dequeueString;
	}

	return 0;
}

int main()
{
	srand(SEED);

	HANDLE Threads[2];
	Threads[0] = (HANDLE)_beginthreadex(nullptr, 0, EnqueueThread, nullptr, 0, nullptr);
	Threads[1] = (HANDLE)_beginthreadex(nullptr, 0, DequeueThread, nullptr, 0, nullptr);

	WaitForMultipleObjects(2, Threads, true, INFINITE);
}