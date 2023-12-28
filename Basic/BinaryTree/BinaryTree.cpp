#include "BinaryTree.h"
#include <iostream>

BinaryTree::BinaryTree()
{
	_Root = nullptr;
}

BinaryTree::~BinaryTree()
{
	DeleteDestructor(_Root);
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
	if (_Root == nullptr)
		return false;

	Node* node = _Root;
	Node* Parent = nullptr;
	while (1)
	{
		if (Data < node->Data)
		{
			if (node->Left == nullptr)
				break;
			else
			{
				Parent = node;
				node = node->Left;
			}
		}
		else if (Data == node->Data)
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

			// Root 면 Parent 관련 설정할 필요가 없다.
			if (node == _Root)
			{
				if (node->Left != nullptr)
				{
					_Root = node->Left;
				}
				else if (node->Right != nullptr)
				{
					_Root = node->Right;
				}
				else
				{
					_Root = nullptr;
				}
				delete node;
				return true;
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
		else
		{
			if (node->Right == nullptr)
				break;
			else
			{
				Parent = node;
				node = node->Right;
			}
		}
	}

	return false;
}

bool BinaryTree::Find(int Data)
{
	if (_Root == nullptr)
		return false;

	Node* node = _Root;
	while (1)
	{
		if (Data < node->Data)
		{
			if (node->Left == nullptr)
				break;
			else
				node = node->Left;
		}
		else if (Data == node->Data)
			return true;
		else
		{
			if (node->Right == nullptr)
				break;
			else
				node = node->Right;
		}
	}

	return false;
}

void BinaryTree::DeleteDestructor(Node* node)
{
	if (node == nullptr)
		return;

	// 소멸자는 후위 연산
	DeleteDestructor(node->Left);
	DeleteDestructor(node->Right);
	delete node;
}

int BinaryTree::GetMaxDepth() const
{
	int maxLevel = 0;

	if (_Root == nullptr)
		return maxLevel;

	maxLevel = _Root->GetMaxDepth();

	return maxLevel;
}