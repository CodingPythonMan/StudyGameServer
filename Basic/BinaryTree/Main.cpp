#include <iostream>
#include "BinaryTree.h"
using namespace std;

int main()
{
	BinaryTree binaryTree;

	binaryTree.Insert(1);
	binaryTree.Insert(2);
	binaryTree.Insert(3);

	binaryTree.Print();
}