#include "RingBuffer.h"

RingBuffer::RingBuffer() : BufferSize{DEFAULT_SIZE}
{
	Buffer = new char[BufferSize];
	Front = Rear = 0;
	FreeSize = BufferSize;
}

RingBuffer::RingBuffer(int bufferSize) : BufferSize{bufferSize}
{
	Buffer = new char[BufferSize];
	Front = Rear = 0;
	FreeSize = BufferSize;
}

RingBuffer::~RingBuffer()
{
	delete[] Buffer;
}

int RingBuffer::GetBufferSize()
{
	return BufferSize;
}

int RingBuffer::GetUseSize()
{
	return BufferSize - FreeSize;
}

int RingBuffer::GetFreeSize()
{
	return FreeSize;
}

int RingBuffer::Enqueue(char* src, int size)
{
	if (Rear + size > BufferSize - 1)
	{
		int rest = BufferSize - Rear;
		memcpy(&Buffer[Rear], src, rest);

		Rear = Rear + size - BufferSize;
		memcpy(&Buffer[0], src+rest, Rear);
	}	
	else
	{
		memcpy(&Buffer[Rear], src, size);
		Rear += size;
	}

	FreeSize -= size;
	return size;
}

int RingBuffer::Dequeue(char* dest, int size)
{
	if (Front + size > BufferSize - 1)
	{
		int rest = BufferSize - Front;
		memcpy(dest, &Buffer[Front], rest);

		Front = Front + size - BufferSize;
		memcpy(dest+rest, &Buffer[0], Front);
	}
	else
	{
		memcpy(dest, &Buffer[Front], size);
		Front += size;
	}

	FreeSize += size;

	return size;
}

int RingBuffer::Peek(char* dest, int size)
{
	if (Front + size > BufferSize - 1)
	{
		int rest = BufferSize - Front;
		memcpy(dest, &Buffer[Front], rest);
		memcpy(dest + rest, &Buffer[0], size - rest);
	}
	else
	{
		memcpy(dest, &Buffer[Front], size);
	}

	return size;
}

void RingBuffer::ClearBuffer()
{
	Front = Rear;
}