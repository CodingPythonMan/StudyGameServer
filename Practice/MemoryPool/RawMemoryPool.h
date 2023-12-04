#define BUCKET_COUNT 

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

private:
	Node* PageUnderNode;
	Node* PageAboveNode;
};