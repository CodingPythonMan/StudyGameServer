#pragma once
#include "MemoryPool.h"

template<typename T>
class LockFreeQueue
{
private:
	long _size;

	struct Node
	{
		T data;
		Node* next;
	};

	Node* _head;        // 시작노드를 포인트한다.
	Node* _tail;        // 마지막노드를 포인트한다.

	MemoryPool<Node> _nodePool;

public:
	LockFreeQueue()
	{
		_size = 0;
		_head = new Node;
		_head->next = NULL;
		_tail = _head;
	}

	void Enqueue(T t)
	{
		Node* node = _nodePool.Alloc();
		node->data = t;
		node->next = NULL;

		while (true)
		{
			Node* tail = _tail;
			Node* next = tail->next;

			if (next == NULL)
			{
				if (InterlockedCompareExchangePointer((PVOID*)&_tail->next, node, next) == next)
				{
					InterlockedCompareExchangePointer((PVOID*)&_tail, node, tail);
					break;
				}
			}
		}

		InterlockedExchangeAdd(&_size, 1);
	}

	int Dequeue(T& t)
	{
		if (_size == 0)
			return -1;

		while (true)
		{
			Node* head = _head;
			Node* next = head->next;

			if (next == NULL)
			{
				return -1;
			}
			else
			{
				if (InterlockedCompareExchangePointer((PVOID*)&_head, next, head) == head)
				{
					t = next->data;
					_nodePool.Free(head);
					break;
				}
			}
		}
		InterlockedExchangeAdd(&_size, -1);
		return 0;
	}


	int GetSize()
	{
		return _size;
	}
};