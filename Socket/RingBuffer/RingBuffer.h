#pragma once
#include <cstring>

#define DEFAULT_SIZE 1000

class RingBuffer {
public:
	RingBuffer();
	RingBuffer(int bufferSize);

	~RingBuffer();

	int GetBufferSize();
	
	// 사용중인 용량 얻기
	int GetUseSize();
	
	// 남은 용량 얻기
	int GetFreeSize();

	// WritePos 에 데이타 넣음.
	int Enqueue(char* data, int size);

	// ReadPos 에서 데이타 가져옴. ReadPos 이동.
	int Dequeue(char* dest, int size);

	// ReadPos 에서 데이타 읽어옴. ReadPos 고정.
	int Peek(char* dest, int size);

	// 버퍼의 모든 데이터 삭제
	void ClearBuffer();

private:
	int Front;
	int Rear;
	int BufferSize;
	int FreeSize;

	char* Buffer;
};