#include "RingBuffer.h"
#include <iostream>
#include <assert.h>

#define SEED 1902
char message[82] = "1234567890 abcdefghijklmnopqrstuvwxyz 1234567890 abcdefghijklmnopqrstuvwxyz 12345";

int main()
{
	srand(SEED);
	RingBuffer* ringBuffer = new RingBuffer;

	int start = 0;

	while (1)
	{
		int size = rand() % 82;
		char* dequeueString = new char[size + 1];
		char* peekString = new char[size + 1];
		dequeueString[size] = '\0';
		peekString[size] = '\0';

		if (start + size > 81)
		{
			ringBuffer->Enqueue(message + start, 81 - start);
			ringBuffer->Enqueue(message, size - 81 + start);

			start = start + size - 81;
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