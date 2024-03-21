#pragma once
#include <windows.h>
#include <stdio.h>
#include "MyList.h"

#include <excpt.h>

struct History {
	int Action;
	LONG64 newNode;
	LONG64 lastNode;
};

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
		MyList<History>* myList = (MyList<History>*)TlsGetValue(TLSIndex);
		if (myList == nullptr)
		{
			myList = new MyList<History>();
			TlsSetValue(TLSIndex, (LPVOID)myList);
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
	
		History history;
		history.Action = 0;
		history.newNode = (LONG64)newNode;
		history.lastNode = (LONG64)lastTop;
		myList->push_back(history);
	}

	void Pop(void)
	{
		MyList<History>* myList;
		History history;

		__try
		{
			myList = (MyList<History>*)TlsGetValue(TLSIndex);
			if (myList == nullptr)
			{
				myList = new MyList<History>();
				TlsSetValue(TLSIndex, (LPVOID)myList);
			}

			Node* lastTop;
			Node* newTop = nullptr;
			do
			{
				lastTop = _top;

				if (_top == nullptr)
					continue;

				newTop = _top->Next;
			} while (InterlockedCompareExchange64((LONG64*)&_top, (LONG64)newTop, (LONG64)lastTop) != (LONG64)lastTop);

			history.Action = 1;
			history.newNode = (LONG64)newTop;
			history.lastNode = (LONG64)lastTop;
			myList->push_back(history);

			//delete lastTop;
			Delete(lastTop);
		}
		__except (EXCEPTION_EXECUTE_HANDLER)
		{
			unsigned int gc = GetCurrentThreadId();
			while (myList->empty() == false)
			{
				printf("Thread ID : %d, Action : %d, newNode : %lld, lastNode : %lld\n", gc, history.Action, history.newNode, history.lastNode);
			}
		}
	}

	void Delete(Node* node)
	{
		delete node;
	}

private:
	Node* _top;

	int TLSIndex;
};

