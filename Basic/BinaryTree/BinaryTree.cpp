#include "BinaryTree.h"

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
	while (node != nullptr)
	{
		if (Data < node->Data)
		{
			node = node->Left;
		}
		else if (Data == node->Data)
		{
			return false;
		}
		else
		{
			node = node->Right;
		}
	}
	node = new Node;
	node->Data = Data;
	
	return true;
}

bool BinaryTree::Delete(int Data)
{

	return false;
}

bool BinaryTree::Search(int Data)
{
	

	return false;
}

void BinaryTree::Print()
{

}