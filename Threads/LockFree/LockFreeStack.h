#pragma once
#include <windows.h>
#include <stdio.h>
#include "MemoryPool.h"

struct History {
	unsigned int SequenceNum;
	unsigned int Action;
	LONG64 newNode;
	LONG64 lastNode;
};

template<typename T>
class LockFreeStack
{
	struct Node {
		Node* Next;
		T Data;
	};

public:
	LockFreeStack()
	{
		_top = nullptr;
		TLSIndex = TlsAlloc();
		if (TLSIndex == TLS_OUT_OF_INDEXES)
			__debugbreak();

		TLSArray = TlsAlloc();
		if (TLSArray == TLS_OUT_OF_INDEXES)
			__debugbreak();

		_size = 0;
		_SequenceNum = 0;
	}

	void Push(T& data)
	{
		History* myArray = (History*)TlsGetValue(TLSArray);
		unsigned int* myIndex = (unsigned int*)TlsGetValue(TLSIndex);
		if (myArray == nullptr)
		{
			myArray = new History[10000];
			myIndex = new unsigned int;
			*myIndex = 0;
			TlsSetValue(TLSArray, (LPVOID)myArray);
			TlsSetValue(TLSIndex, (LPVOID)myIndex);
		}

		Node* newNode = new Node();
		//Node* newNode = _nodePool.Alloc();
		newNode->Data = data;

		Node* lastTop;
		do 
		{
			lastTop = _top;
			newNode->Next = lastTop;
		} 
		while (InterlockedCompareExchange64((LONG64*)&_top, (LONG64)newNode, (LONG64)lastTop) != (LONG64)lastTop);
	
		InterlockedIncrement((long*)&_SequenceNum);

		myArray[*myIndex].SequenceNum = _SequenceNum;
		myArray[*myIndex].Action = 0;
		myArray[*myIndex].newNode = (LONG64)newNode;
		myArray[*myIndex].lastNode = (LONG64)lastTop;

		InterlockedIncrement((long*)&_size);
		(*myIndex)++;
	}

	void Pop(void)
	{
		History* myArray = (History*)TlsGetValue(TLSArray);
		unsigned int* myIndex = (unsigned int*)TlsGetValue(TLSIndex);
		if (myArray == nullptr)
		{
			myArray = new History[10000];
			myIndex = new unsigned int;
			*myIndex = 0;
			TlsSetValue(TLSArray, (LPVOID)myArray);
			TlsSetValue(TLSIndex, (LPVOID)myIndex);
		}

		Node* lastTop;
		Node* newTop = nullptr;
		do
		{
			lastTop = _top;

			if (_top == nullptr)
				return;

			newTop = _top->Next;
		} 
		while (InterlockedCompareExchange64((LONG64*)&_top, (LONG64)newTop, (LONG64)lastTop) != (LONG64)lastTop);

		InterlockedIncrement((long*)&_SequenceNum);

		myArray[*myIndex].SequenceNum = _SequenceNum;
		myArray[*myIndex].Action = 1;
		myArray[*myIndex].newNode = (LONG64)newTop;
		myArray[*myIndex].lastNode = (LONG64)lastTop;

		//_nodePool.Free(lastTop);
		delete lastTop;

		InterlockedDecrement((long*)&_size);
		(*myIndex)++;
	}

	int GetSize()
	{
		return _size;
	}

private:
	Node* _top;

	int TLSIndex;
	int TLSArray;
	//MemoryPool<Node> _nodePool;

	int _size;

	// Log ¿ë
	unsigned int _SequenceNum;
};

