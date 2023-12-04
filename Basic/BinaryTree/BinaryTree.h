class BinaryTree {
	struct Node {
		int Data;
		Node* Left;
		Node* Right;
	};

public:
	bool Insert(int Data);
	bool Delete(int Data);

	bool Search(int Data);

private:
	Node* _Root;

};