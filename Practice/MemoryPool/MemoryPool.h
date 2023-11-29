template <class T>
class MemoryPool
{
public:
	struct Node {
		T Data;
		Node* Prev;
	};

	//////////////////////////////////////////////////////////////////////////
	// 생성자, 파괴자.
	//
	// Parameters:	(int) 초기 블럭 개수.
	//				(bool) Alloc 시 생성자 / Free 시 파괴자 호출 여부
	// Return:
	//////////////////////////////////////////////////////////////////////////
	MemoryPool(int BlockNum = 0, bool PlacementNew = false);
	virtual	~MemoryPool();


	//////////////////////////////////////////////////////////////////////////
	// 블럭 하나를 할당받는다.  
	//
	// Parameters: 없음.
	// Return: (DATA *) 데이타 블럭 포인터.
	//////////////////////////////////////////////////////////////////////////
	T* Alloc(void);

	//////////////////////////////////////////////////////////////////////////
	// 사용중이던 블럭을 해제한다.
	//
	// Parameters: (DATA *) 블럭 포인터.
	// Return: (BOOL) TRUE, FALSE.
	//////////////////////////////////////////////////////////////////////////
	bool	Free(T* pData);


	//////////////////////////////////////////////////////////////////////////
	// 현재 확보 된 블럭 개수를 얻는다. (메모리풀 내부의 전체 개수)
	//
	// Parameters: 없음.
	// Return: (int) 메모리 풀 내부 전체 개수
	//////////////////////////////////////////////////////////////////////////
	int		GetCapacityCount(void) { return _Capacity; }

	//////////////////////////////////////////////////////////////////////////
	// 현재 사용중인 블럭 개수를 얻는다.
	//
	// Parameters: 없음.
	// Return: (int) 사용중인 블럭 개수.
	//////////////////////////////////////////////////////////////////////////
	int		GetUseCount(void) { return _UseCount; }

private:
	int _Capacity;
	int _UseCount;
	bool _PlacementNew;
	// 스택 방식으로 반환된 (미사용) 오브젝트 블럭을 관리.
	Node* _FreeNode;
};

template<class T>
inline MemoryPool<T>::MemoryPool(int BlockNum, bool PlacementNew)
{
	// 더미 노드 설정
	_FreeNode = (Node*)malloc(sizeof(Node));
	_Capacity = BlockNum;
	_PlacementNew = PlacementNew;
	_UseCount = 0;

	for (int i = 0; i < _Capacity; i++)
	{
		Node* node;
		// PlacementNew 가 활성화 되어있다면 생성자가 호출되지 않는다.
		if (PlacementNew == true)
		{
			node = (Node*)malloc(sizeof(Node));
		}
		else
		{
			node = new Node;
		}
		if(node == nullptr)
			return;

		node->Prev = _FreeNode;
		_FreeNode = node;
		//printf("[Node %d] : 0x%p\n", i, node);
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
	Node* node;

	// 가용 풀을 모두 쓰고 있을 때 이 경우가 호출된다.
	ptr = &_FreeNode->Data;
	// 생성자 호출
	if (_PlacementNew == true)
	{
		new(&_FreeNode->Data) T;
	}

	// Capacity 가 충분할 때 쉽게 할당해줄 수 있다.
	if (_Capacity > 0)
	{
		_FreeNode = _FreeNode->Prev;
		_Capacity--;
	}
	else
	{
		node = (Node*)malloc(sizeof(Node));
		_FreeNode->Prev = node;
		_FreeNode = node;
	}
	
	_UseCount++;

	//printf("[Node Alloc] : 0x%p, Capacity : %d, UseCount : %d\n", ptr, _Capacity, _UseCount);

	return ptr;
}

template<class T>
inline bool MemoryPool<T>::Free(T* pData)
{
	if (pData != nullptr)
	{
		(*pData).~T();
		((Node*)pData)->Prev = _FreeNode;
		_FreeNode = (Node*)pData;
		_UseCount--;
		_Capacity++;
	}
	else
	{
		__debugbreak();
		return false;
	}

	//printf("[Node Free] : 0x%p, Capacity : %d, UseCount : %d\n", pData, _Capacity, _UseCount);

	return true;
}