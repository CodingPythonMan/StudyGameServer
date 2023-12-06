#include <vector>
#include <string>
using namespace std;

struct Cell {
	string Value;
	bool Present;

	Cell()
	{
		Present = false;
	}
};

using RowList = vector<vector<Cell>>;

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

		int GetMaxDepth() const
		{
			int maxDepth;
			int leftDepth = 0;
			int rightDepth = 0;

			if (Left != nullptr)
				leftDepth = Left->GetMaxDepth();
			
			if (Right != nullptr)
				rightDepth = Right->GetMaxDepth();

			if (leftDepth > rightDepth)
			{
				maxDepth = leftDepth + 1;
			}
			else
			{
				maxDepth = rightDepth + 1;
			}

			return maxDepth;
		}
	};

public:
	BinaryTree();
	virtual ~BinaryTree();

	bool Insert(int Data);

	bool Delete(int Data);
	
	bool Find(int Data);

private:
	bool Delete(Node* node, Node* Parent, int Data);
	bool Find(Node* node, int Data);

	int GetMaxDepth() const;

	Node* _Root;
};