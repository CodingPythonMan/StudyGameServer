class RawMemoryPool
{
public:
	struct Node {
		void* Data;
		unsigned int Size;
		Node* Next;
	};

	RawMemoryPool();
	virtual	~RawMemoryPool();

	void*	Alloc(void);

	bool	Free(void* pData);

	int		GetCapacityCount(void) { return _Capacity; }

	int		GetUseCount(void) { return _UseCount; }

private:
	int _Capacity;
	int _UseCount;
	bool _PlacementNew;
	Node* _FreeNode;
};