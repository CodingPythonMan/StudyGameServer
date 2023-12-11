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
			Node* thisNode = node;
			Parent = node;
			node = node->Left;
			while (node->Right != nullptr)
			{
				Parent = node;
				node = node->Right;
			}
			thisNode->Data = node->Data;
			// 여기서 return 되지 않는 이유는 node 는 지워질 때 아래 사항 고려.
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
		// 오른 자식이 있는 경우
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
		// 자식이 없는 경우
		else
		{
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