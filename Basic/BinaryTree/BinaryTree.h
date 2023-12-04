class BinaryTree {
	struct Node {
		int Data;
		Node* Left;
		Node* Right;

		Node()
		{
			Data = 0;
			Left = nullptr;
			Right = nullptr;
		}
	};

public:
	BinaryTree();
	virtual ~BinaryTree();

	bool Insert(int Data);
	bool Delete(int Data);

	bool Search(int Data);

	void Print();

private:
	Node* _Root;

};