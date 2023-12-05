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

	void Print();
	int GetMaxDepth() const;

private:
	RowList GetRowList(int maxDepth) const;
	vector<string> FormatRow(const RowList& rowList) const;
	void TrimRow(vector<string>& rows);

	Node* _Root;
};