#include <iostream>
#include "BinaryTree.h"
using namespace std;

int main()
{
	BinaryTree binaryTree;

	binaryTree.Insert(10);
	binaryTree.Insert(5);
	binaryTree.Insert(15);
	binaryTree.Insert(3);
	binaryTree.Insert(13);
	binaryTree.Insert(4);
	binaryTree.Insert(6);
	binaryTree.Insert(11);
	binaryTree.Insert(14);

	binaryTree.Print();
}