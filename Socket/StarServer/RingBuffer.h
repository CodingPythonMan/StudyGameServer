#pragma once

#define RING_DEFAULT_SIZE 1000

class RingBuffer {
public:
	RingBuffer();
	RingBuffer(int bufferSize);

	~RingBuffer();

	int GetBufferSize();

	// ������� �뷮 ���
	int GetUseSize();

	// ���� �뷮 ���
	int GetFreeSize();

	// WritePos �� ����Ÿ ����.
	int Enqueue(char* data, int size);

	// ReadPos ���� ����Ÿ ������. ReadPos �̵�.
	int Dequeue(char* dest, int size);

	// ReadPos ���� ����Ÿ �о��. ReadPos ����.
	int Peek(char* dest, int size);

	// ������ ��� ������ ����
	void ClearBuffer();

private:
	// ���� �����ͷ� �ܺο��� �ѹ濡 �а�, �� �� �ִ� ����.(������ ���� ����)
	// ���� ť�� ������ ������ ���ܿ� �ִ� �����ʹ� �� -> ó������ ���ư���
	// 2���� �����͸� ��ų� ���� �� ����. �� �κп��� �������� ���� ���̸� �ǹ�
	int DirectEnqueueSize();
	int DirectDequeueSize();

	// ���ϴ� ���̸�ŭ �б���ġ ���� ���� / ���� ��ġ �̵�
	int	MoveRear(int size);
	int	MoveFront(int size);

	// ������ Front ������ ����.
	char* GetFrontBufferPtr();

	// ������ RearPos ������ ����.
	char* GetRearBufferPtr();

	// ������ ó�������� ����.
	char* GetStartBufferPtr();

private:
	int Front;
	int Rear;
	int BufferSize;

	char* Buffer;
};