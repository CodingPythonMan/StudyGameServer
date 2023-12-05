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
	bool Search(Node* node);

	RowList GetRowList(int maxDepth) const;
	vector<string> FormatRow(const RowList& rowList) const;
	void TrimRow(vector<string>& rows);

	Node* _Root;
};