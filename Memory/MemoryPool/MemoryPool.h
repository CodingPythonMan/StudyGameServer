#pragma once
#include <windows.h>
#include <new>

#define NOT_TAGGING_BIT 48

struct History {
	unsigned int SequenceNum;
	unsigned int Action;
	LONG64 newNode;
	LONG64 lastNode;
};

template <class T>
class MemoryPool
{
public:
	struct Node {
		T Data;
		Node* Next;
	};

	//////////////////////////////////////////////////////////////////////////
	// 생성자, 파괴자.
	//////////////////////////////////////////////////////////////////////////
	MemoryPool(int BlockNum = 0, bool PlacementNew = false);
	virtual	~MemoryPool();


	//////////////////////////////////////////////////////////////////////////
	// 블럭 하나를 할당받는다.  
	//////////////////////////////////////////////////////////////////////////
	T* Alloc(void);

	//////////////////////////////////////////////////////////////////////////
	// 사용중이던 블럭을 해제한다.
	//////////////////////////////////////////////////////////////////////////
	bool	Free(T* pData);


	//////////////////////////////////////////////////////////////////////////
	// 현재 확보 된 블럭 개수를 얻는다. (메모리풀 내부의 전체 개수)
	//////////////////////////////////////////////////////////////////////////
	int		GetCapacityCount(void) { return _Capacity; }

	//////////////////////////////////////////////////////////////////////////
	// 현재 사용중인 블럭 개수를 얻는다.
	//////////////////////////////////////////////////////////////////////////
	int		GetUseCount(void) { return _UseCount; }

	//////////////////////////////////////////////////////////////////////////
	// ABA 문제 해결용 Counter 추출
	//////////////////////////////////////////////////////////////////////////
	inline LONG64 GetCounter(LONG64 ptr) { return  static_cast<unsigned __int64>(ptr) >> NOT_TAGGING_BIT; }

	//////////////////////////////////////////////////////////////////////////
	// ABA 문제 해결용 Counter 셋팅
	//////////////////////////////////////////////////////////////////////////
	inline LONG64 SetCounter(LONG64 counter) { return  counter << NOT_TAGGING_BIT; }

private:
	int _Capacity;
	int _UseCount;
	bool _PlacementNew;
	// 스택 방식으로 반환된 (미사용) 오브젝트 블럭을 관리.
	LONG64 _FreeNode;

	int TLSIndex;
	int TLSArray;
	unsigned int _SequenceNum;
};

template<class T>
inline MemoryPool<T>::MemoryPool(int BlockNum, bool PlacementNew)
{
	_FreeNode = 0;
	_Capacity = BlockNum;
	_PlacementNew = PlacementNew;
	_UseCount = 0;

	TLSIndex = TlsAlloc();
	if (TLSIndex == TLS_OUT_OF_INDEXES)
		__debugbreak();

	TLSArray = TlsAlloc();
	if (TLSArray == TLS_OUT_OF_INDEXES)
		__debugbreak();

	_SequenceNum = 0;

	for (int i = 0; i < _Capacity; i++)
	{
		Node* node = (Node*)malloc(sizeof(Node));
		
		if (node == nullptr)
			return;

		// PlacementNew 가 활성화 안 되어있다면 생성자 호출
		if (PlacementNew == false)
		{
			new(&node->Data) T;
		}

		// 굳이 처음부터 태깅해둘 필요는 없다.
		// CAS 가 일어나는 순간부터 태깅이 필요하다.
		node->Next = reinterpret_cast<Node*>(_FreeNode);
		_FreeNode = reinterpret_cast<LONG64>(node);
	}
}

template<class T>
inline MemoryPool<T>::~MemoryPool()
{
	
}

template<class T>
inline T* MemoryPool<T>::Alloc(void)
{
	History* myArray = (History*)TlsGetValue(TLSArray);
	unsigned int* myIndex = (unsigned int*)TlsGetValue(TLSIndex);
	if (myArray == nullptr)
	{
		myArray = new History[10000000];
		myIndex = new unsigned int;
		*myIndex = 0;
		TlsSetValue(TLSArray, (LPVOID)myArray);
		TlsSetValue(TLSIndex, (LPVOID)myIndex);
	}

	LONG64 newFree;
	LONG64 lastFree;
	Node* lastTop;

	do
	{
		lastFree = _FreeNode;

		if (lastFree == NULL)
		{
			Node* newNode = (Node*)malloc(sizeof(Node));

			if (newNode == nullptr)
				return nullptr;

			new(&newNode->Data) T;

			InterlockedIncrement((long*)&_UseCount);

			myArray[*myIndex].SequenceNum = _SequenceNum;
			myArray[*myIndex].Action = 0;
			myArray[*myIndex].newNode = (LONG64)newNode;
			myArray[*myIndex].lastNode = lastFree;

			return &newNode->Data;
		}

		// 전 _FreeNode 에서 lastTop 으로 값 복원시킨다.
		lastTop = reinterpret_cast<Node*>(lastFree - SetCounter(GetCounter(lastFree)));
		newFree = reinterpret_cast<LONG64>(lastTop->Next);
	}
	while (InterlockedCompareExchange64(&_FreeNode, newFree, lastFree) != lastFree);

	InterlockedIncrement((long*)&_SequenceNum);

	myArray[*myIndex].SequenceNum = _SequenceNum;
	myArray[*myIndex].Action = 1;
	myArray[*myIndex].newNode = newFree;
	myArray[*myIndex].lastNode = lastFree;

	(*myIndex)++;

	// Placement New 활성화라면 Alloc 에서 생성자 호출.
	if (_PlacementNew == true)
	{
		new(reinterpret_cast<T*>(lastTop)) T;
	}

	InterlockedIncrement((long*)&_UseCount);
	InterlockedDecrement((long*)&_Capacity);

	return reinterpret_cast<T*>(lastTop);
}

template<class T>
inline bool MemoryPool<T>::Free(T* pData)
{
	History* myArray = (History*)TlsGetValue(TLSArray);
	unsigned int* myIndex = (unsigned int*)TlsGetValue(TLSIndex);
	if (myArray == nullptr)
	{
		myArray = new History[10000000];
		myIndex = new unsigned int;
		*myIndex = 0;
		TlsSetValue(TLSArray, (LPVOID)myArray);
		TlsSetValue(TLSIndex, (LPVOID)myIndex);
	}

	LONG64 newFree;
	LONG64 lastFree;
	unsigned __int64 Counter;

	LONG64 ptr = reinterpret_cast<LONG64>(pData);
	pData->~T();

	do 
	{
		lastFree = _FreeNode;
		Counter = GetCounter(lastFree) + 1;
		reinterpret_cast<Node*>(ptr)->Next = reinterpret_cast<Node*>(lastFree);
		// new Free 에 태깅
		newFree = ptr + SetCounter(Counter);
	} 
	while (InterlockedCompareExchange64(&_FreeNode, newFree, lastFree) != lastFree);

	InterlockedIncrement((long*)&_SequenceNum);

	myArray[*myIndex].SequenceNum = _SequenceNum;
	myArray[*myIndex].Action = 0;
	myArray[*myIndex].newNode = newFree;
	myArray[*myIndex].lastNode = lastFree;

	(*myIndex)++;

	InterlockedDecrement((long*)&_UseCount);
	InterlockedIncrement((long*)&_Capacity);

	return true;
}