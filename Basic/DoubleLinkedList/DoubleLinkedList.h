#pragma once

template <typename T>
class MyDoubleLinkedList
{
public:
	struct Node
	{
		T _data;
		Node* _Prev;
		Node* _Next;
	};
};