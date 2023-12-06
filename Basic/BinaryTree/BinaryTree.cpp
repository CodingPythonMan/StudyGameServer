#include "BinaryTree.h"
#include <iostream>

BinaryTree::BinaryTree()
{
	_Root = nullptr;
}

BinaryTree::~BinaryTree()
{
	delete _Root;
}

bool BinaryTree::Insert(int Data)
{
	if (_Root == nullptr)
	{
		_Root = new Node;
		_Root->Data = Data;
		return true;
	}

	Node* node = _Root;
	while (1)
	{
		if (Data < node->Data)
		{
			if (node->Left == nullptr)
			{		
				Node* newNode = new Node;
				newNode->Data = Data;
				node->Left = newNode;
				break;
			}
			else
			{
				node = node->Left;
			}
		}
		else if (Data == node->Data)
		{
			return false;
		}
		else
		{
			if (node->Right == nullptr)
			{
				Node* newNode = new Node;
				newNode->Data = Data;
				node->Right = newNode;
				break;
			}
			else
			{
				node = node->Right;
			}
		}
	}

	return true;
}

bool BinaryTree::Delete(int Data)
{
	// 하위 두개 있으면 오류날 확률 있음
	return Delete(_Root, nullptr, Data);
}


bool BinaryTree::Find(int Data)
{
	return Find(_Root, Data);
}

void BinaryTree::Print()
{
	const int maxDepth = GetMaxDepth();

	// 빈 트리임을 확인
	if (maxDepth == 0) {
		cout << " <empty tree>\n";
		return;
	}

	const RowList rowList = GetRowList(maxDepth);
	// 트리 Depth만큼 텍스트로 가공
	vector<string> formattedRows = FormatRow(rowList);
	// 왼쪽 Trim
	TrimRow(formattedRows);
	for (const auto& row : formattedRows) 
	{
		cout << ' ' << row << '\n';
	}
}

bool BinaryTree::Delete(Node* node, Node* Parent, int Data)
{
	if (node == nullptr)
		return false;

	bool leftResult, rightResult;
	leftResult = Delete(node->Left, node, Data);
	// 데이터에 대한 값 처리할 때 하위 있는지 확인 필요
	if (Data == node->Data)
	{
		// 자식 둘다 있을 경우
		if (node->Left != nullptr && node->Right != nullptr)
		{
			// 왼쪽의 맨 오른쪽으로 접근하고, 해당 node 설정 후 삭제.
			node = node->Left;
			while (node == nullptr)
			{
				Parent = node;
				node = node->Right;
			}
		}

		// 왼쪽 자식이 있는 경우
		if (node->Left != nullptr)
		{
			if (Parent->Left == node)
			{
				Parent->Left = node->Left;
			}
			else
			{
				Parent->Right = node->Left;
			}
		}
		else if (node->Right != nullptr)
		{
			if (Parent->Left == node)
			{
				Parent->Left = node->Right;
			}
			else
			{
				Parent->Right = node->Right;
			}
		}
		else
		{
			// 자식이 없는 경우
			if (Parent->Left == node)
			{
				Parent->Left = nullptr;
			}
			else
			{
				Parent->Right = nullptr;
			}
		}
		delete node;
		return true;
	}
	rightResult = Delete(node->Right, node, Data);

	return leftResult || rightResult;
}

bool BinaryTree::Find(Node* node, int Data)
{
	if (node == nullptr)
		return false;

	bool leftResult, rightResult;

	leftResult = Find(node->Left, Data);
	if (node->Data == Data)
	{
		return true;
	}
	rightResult = Find(node->Right, Data);

	return leftResult || rightResult;
}

int BinaryTree::GetMaxDepth() const
{
	int maxLevel = 0;

	if (_Root == nullptr)
		return maxLevel;

	maxLevel = _Root->GetMaxDepth();

	return maxLevel;
}

RowList BinaryTree::GetRowList(int maxDepth) const
{
	RowList rowList;

	vector<Node*> traversalStack;
	vector<vector<Node*>> rows;
	if (_Root == nullptr) 
		return rowList;

	Node* node = _Root;
	rows.resize(maxDepth);
	int depth = 0;
	for (;;) {
		// 제일 깊은 노드는 null or 단말
		if (depth == maxDepth - 1) {
			rows[depth].push_back(node);
			if (depth == 0) 
				break;
			--depth;
			continue;
		}

		// 처음 방문이라면 왼쪽 노드
		if (traversalStack.size() == depth) {
			rows[depth].push_back(node);
			traversalStack.push_back(node);
			if (node != nullptr)
				node = node->Left;
			++depth;
			continue;
		}

		// 하위가 하나라면 오른쪽 노드도 탐색
		if (rows[depth + 1].size() % 2) {
			node = traversalStack.back();
			if (node != nullptr)
				node = node->Right;
			++depth;
			continue;
		}

		// 루프 탈출
		if (depth == 0) 
			break;

		traversalStack.pop_back();
		node = traversalStack.back();
		--depth;
	}

	// 각 행마다 텍스트로 가공되기 위해 준비
	for (const auto& row : rows) {
		rowList.emplace_back();
		for (Node* pn : row) {
			Cell cell;
			if (pn) {
				cell.Value = to_string(pn->Data);
				cell.Present = true;
				rowList.back().push_back(cell);
			}
			else {
				rowList.back().push_back(cell);
			}
		}
	}

	return rowList;
}

vector<string> BinaryTree::FormatRow(const RowList& rowList) const
{
	// First find the maximum value string length and put it in cell_width
	size_t cell_width = 0;
	for (const auto& row_disp : rowList) 
	{
		for (const auto& cd : row_disp) 
		{
			if (cd.Present && cd.Value.length() > cell_width) 
			{
				cell_width = cd.Value.length();
			}
		}
	}

	// make sure the cell_width is an odd number
	if (cell_width % 2 == 0) 
		cell_width++;

	// allows leaf nodes to be connected when they are
	// all with size of a single character
	if (cell_width < 3) cell_width = 3;

	// formatted_rows will hold the results
	vector<string> formatted_rows;

	// some of these counting variables are related,
	// so its should be possible to eliminate some of them.
	size_t row_count = rowList.size();

	// this row's element count, a power of two
	int row_elem_count = 1i64 << (row_count - 1);

	// left_pad holds the number of space charactes at the beginning of the bottom row
	size_t left_pad = 0;

	// Work from the level of maximum depth, up to the root
	// ("formatted_rows" will need to be reversed when done) 
	for (int r = 0; r < row_count; ++r) {
		const auto& cd_row = rowList[row_count - r - 1]; // r reverse-indexes the row
		// "space" will be the number of rows of slashes needed to get
		// from this row to the next.  It is also used to determine other
		// text offsets.
		size_t space = (1i64 << r) * (cell_width + 1) / 2 - 1;
		// "row" holds the line of text currently being assembled
		string row;
		// iterate over each element in this row
		for (int c = 0; c < row_elem_count; ++c) {
			// add padding, more when this is not the leftmost element
			row += string(c ? left_pad * 2 + 1 : left_pad, ' ');
			if (cd_row[c].Present) {
				// This position corresponds to an existing Node
				const string& valstr = cd_row[c].Value;
				// Try to pad the left and right sides of the value string
				// with the same number of spaces.  If padding requires an
				// odd number of spaces, right-sided children get the longer
				// padding on the right side, while left-sided children
				// get it on the left side.
				size_t long_padding = cell_width - valstr.length();
				size_t short_padding = long_padding / 2;
				long_padding -= short_padding;
				row += string(c % 2 ? short_padding : long_padding, ' ');
				row += valstr;
				row += string(c % 2 ? long_padding : short_padding, ' ');
			}
			else {
				// This position is empty, Nodeless...
				row += string(cell_width, ' ');
			}
		}
		// A row of spaced-apart value strings is ready, add it to the result vector
		formatted_rows.push_back(row);

		// The root has been added, so this loop is finsished
		if (row_elem_count == 1) break;

		// Add rows of forward- and back- slash characters, spaced apart
		// to "connect" two rows' Node value strings.
		// The "space" variable counts the number of rows needed here.
		size_t left_space = space + 1;
		size_t right_space = space - 1;
		for (int sr = 0; sr < space; ++sr) {
			string row;
			for (int c = 0; c < row_elem_count; ++c) {
				if (c % 2 == 0) {
					row += string(c ? left_space * 2 + 1 : left_space, ' ');
					row += cd_row[c].Present ? '/' : ' ';
					row += string(right_space + 1, ' ');
				}
				else {
					row += string(right_space, ' ');
					row += cd_row[c].Present ? '\\' : ' ';
				}
			}
			formatted_rows.push_back(row);
			++left_space;
			--right_space;
		}
		left_pad += space + 1;
		row_elem_count /= 2;
	}

	// Reverse the result, placing the root node at the beginning (top)
	reverse(formatted_rows.begin(), formatted_rows.end());

	return formatted_rows;
}

void BinaryTree::TrimRow(vector<string>& rows) 
{
	if (rows.size() == 0) 
		return;

	auto min_space = rows.front().length();
	for (const auto& row : rows) 
	{
		auto i = row.find_first_not_of(' ');
		if (i == string::npos) 
			i = row.length();
		if (i == 0) 
			return;
		if (i < min_space) 
			min_space = i;
	}
	for (auto& row : rows) 
	{
		row.erase(0, min_space);
	}
}