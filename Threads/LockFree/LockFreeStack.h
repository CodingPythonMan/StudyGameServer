#pragma once
#include "History.h"
#include "MyList.h"

template<typename T>
class LockFreeStack
{
	struct Node {
		Node* Next;
		T Data;
		int New;
		int Delete;
	};

public:
	LockFreeStack()
	{
		TLSIndex = TlsAlloc();
	}

	void Push(T& data)
	{
		MyList<History>* myList = (MyList*)TlsGetValue(TLSIndex);
		if (myList == nullptr)
		{
			myList = new MyList<History>();
			TlsGetValue(TLSIndex, (LPVOID)myList);
		}

		Node* newNode = new Node();
		newNode->Data = data;

		Node* lastTop;
		do 
		{
			lastTop = _top;
			newNode->Next = lastTop;
		} 
		while (InterlockedCompareExchange64((LONG64*)&_top, (LONG64)newNode, (LONG64)lastTop) != (LONG64)lastTop);
	}

	void Pop(void)
	{
		Node* lastTop;
		Node* newTop = nullptr;
		do
		{
			lastTop = _top;

			if (_top == nullptr)
				continue;

			newTop = _top->Next;
		}
		while (InterlockedCompareExchange64((LONG64*)&_top, (LONG64)newTop, (LONG64)lastTop) != (LONG64)lastTop);

		__try {

		}
		__finally {

		}
		delete lastTop;
	}

private:
	Node* _top;

	int TLSIndex;
};

