#include <iostream>
#include "BinaryTree.h"
#include "Test.h"
using namespace std;

void TestCode1()
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

	bool Result1 = binaryTree.Find(3);

	binaryTree.Delete(3);
	binaryTree.Delete(6);
	binaryTree.Delete(10);

	bool Result2 = binaryTree.Find(3);
	bool Result3 = binaryTree.Find(10);

	cout << Result1 << "\n";
	cout << Result2 << "\n";
	cout << Result3 << "\n";
}

int main()
{
	BinaryTree binaryTree;

	binaryTree.Insert(30);
	binaryTree.Insert(20);
	binaryTree.Insert(40);

	binaryTree.Delete(30);
	binaryTree.Delete(20);

	TestCode1();
}