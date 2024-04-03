#pragma once
#include <windows.h>
#include <new>

#define NOT_TAGGING_BIT 48

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
	LONG64 GetCounter(LONG64 ptr) { return  static_cast<unsigned __int64>(ptr) >> NOT_TAGGING_BIT; }

	//////////////////////////////////////////////////////////////////////////
	// ABA 문제 해결용 Counter 셋팅
	//////////////////////////////////////////////////////////////////////////
	LONG64 SetCounter(LONG64 counter) { return  counter << NOT_TAGGING_BIT; }

private:
	int _Capacity;
	int _UseCount;
	bool _PlacementNew;
	// 스택 방식으로 반환된 (미사용) 오브젝트 블럭을 관리.
	LONG64 _FreeNode;
};

template<class T>
inline MemoryPool<T>::MemoryPool(int BlockNum, bool PlacementNew)
{
	_FreeNode = 0;
	_Capacity = BlockNum;
	_PlacementNew = PlacementNew;
	_UseCount = 0;

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
	T* ptr;

	LONG64 newFree;
	LONG64 lastFree;

	LONG64 ptrValue = _FreeNode - SetCounter(GetCounter(_FreeNode));
	ptr = &reinterpret_cast<Node*>(ptrValue)->Data;

	// Placement New 활성화라면 Alloc 에서 생성자 호출.
	if (ptrValue != NULL && _PlacementNew == true)
	{
		new(reinterpret_cast<T*>(ptrValue)) T;
	}

	do
	{
		lastFree = _FreeNode;

		if (lastFree == NULL)
		{
			Node* newNode = (Node*)malloc(sizeof(Node));

			if (newNode == nullptr)
				return nullptr;

			ptr = &newNode->Data;
			new(ptr) T;

			InterlockedIncrement((long*)&_UseCount);

			return ptr;
		}

		Node* newTop = reinterpret_cast<Node*>(lastFree - SetCounter(GetCounter(lastFree)));
		newFree = reinterpret_cast<LONG64>(newTop->Next);
	} 
	while (InterlockedCompareExchange64(&_FreeNode, newFree, lastFree) != lastFree);

	InterlockedIncrement((long*)&_UseCount);
	InterlockedDecrement((long*)&_Capacity);

	return ptr;
}

template<class T>
inline bool MemoryPool<T>::Free(T* pData)
{
	LONG64 newFree;
	LONG64 lastFree;
	unsigned __int64 Counter;

	newFree = reinterpret_cast<LONG64>(pData);
	LONG64 ptr = newFree - SetCounter(GetCounter(newFree));
	reinterpret_cast<T*>(ptr)->~T();

	do 
	{
		lastFree = _FreeNode;
		Counter = GetCounter(lastFree) + 1;
		reinterpret_cast<Node*>(ptr)->Next = reinterpret_cast<Node*>(lastFree);
		// new Free 에 태깅
		newFree = ptr + SetCounter(Counter);
	} 
	while (InterlockedCompareExchange64(&_FreeNode, newFree, lastFree) != lastFree);

	InterlockedDecrement((long*)&_UseCount);
	InterlockedIncrement((long*)&_Capacity);

	return true;
}