#pragma once
#include <windows.h>
#include <stdio.h>
#include "MemoryPool.h"

/*
struct StackHistory {
	unsigned int SequenceNum;
	unsigned int Action;
	LONG64 newNode;
	LONG64 lastNode;
};*/

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
		_top = 0;
		_size = 0;
		/*
		TLSIndex = TlsAlloc();
		if (TLSIndex == TLS_OUT_OF_INDEXES)
			__debugbreak();

		TLSArray = TlsAlloc();
		if (TLSArray == TLS_OUT_OF_INDEXES)
			__debugbreak();

		_SequenceNum = 0;*/
	}

	void Push(T& data)
	{
		/*
		StackHistory* myArray = (StackHistory*)TlsGetValue(TLSArray);
		unsigned int* myIndex = (unsigned int*)TlsGetValue(TLSIndex);
		if (myArray == nullptr)
		{
			myArray = new StackHistory[10000000];
			myIndex = new unsigned int;
			*myIndex = 0;
			TlsSetValue(TLSArray, (LPVOID)myArray);
			TlsSetValue(TLSIndex, (LPVOID)myIndex);
		}*/

		Node* newNode = _nodePool.Alloc();
		newNode->Data = data;

		LONG64 newTop;
		LONG64 lastTop;
		unsigned __int64 Counter;

		LONG64 ptr = reinterpret_cast<LONG64>(newNode);

		do 
		{
			lastTop = _top;
			Counter = GetCounter(lastTop) + 1;
			newNode->Next = reinterpret_cast<Node*>(lastTop);
			newTop = ptr + SetCounter(Counter);
		} 
		while (InterlockedCompareExchange64(&_top, newTop, lastTop) != lastTop);

		//if (newTop == lastTop)
		//	__debugbreak();
	
		/*
		InterlockedIncrement((long*)&_SequenceNum);

		myArray[*myIndex].SequenceNum = _SequenceNum;
		myArray[*myIndex].Action = 0;
		myArray[*myIndex].newNode = newTop;
		myArray[*myIndex].lastNode = lastTop;
		
		(*myIndex)++;
		*/

		InterlockedIncrement((long*)&_size);
	}

	T Pop(void)
	{
		/*
		StackHistory* myArray = (StackHistory*)TlsGetValue(TLSArray);
		unsigned int* myIndex = (unsigned int*)TlsGetValue(TLSIndex);
		if (myArray == nullptr)
		{
			myArray = new StackHistory[10000000];
			myIndex = new unsigned int;
			*myIndex = 0;
			TlsSetValue(TLSArray, (LPVOID)myArray);
			TlsSetValue(TLSIndex, (LPVOID)myIndex);
		}*/

		LONG64 lastTop;
		LONG64 newTop;
		Node* lastPop;

		do
		{
			lastTop = _top;

			if (lastTop == NULL)
				return NULL;

			lastPop = reinterpret_cast<Node*>(lastTop - SetCounter(GetCounter(lastTop)));
			newTop = reinterpret_cast<LONG64>(lastPop->Next);
		} 
		while (InterlockedCompareExchange64(&_top, newTop, lastTop) != lastTop);

		//if (newTop == lastTop)
		//	__debugbreak();

		/*
		InterlockedIncrement((long*)&_SequenceNum);

		myArray[*myIndex].SequenceNum = _SequenceNum;
		myArray[*myIndex].Action = 1;
		myArray[*myIndex].newNode = newTop;
		myArray[*myIndex].lastNode = lastTop;

		(*myIndex)++; */

		_nodePool.Free(reinterpret_cast<Node*>(lastPop));

		InterlockedDecrement((long*)&_size);
		
		return reinterpret_cast<Node*>(lastPop)->Data;
	}

	int GetSize()
	{
		return _size;
	}

	LONG64 GetCounter(LONG64 ptr) { return  static_cast<unsigned __int64>(ptr) >> NOT_TAGGING_BIT; }
	LONG64 SetCounter(LONG64 counter) { return  counter << NOT_TAGGING_BIT; }

private:
	LONG64 _top;
	MemoryPool<Node> _nodePool;

	int _size;

	// Log ¿ë
	/*
	int TLSIndex;
	int TLSArray;
	unsigned int _SequenceNum;*/
};

