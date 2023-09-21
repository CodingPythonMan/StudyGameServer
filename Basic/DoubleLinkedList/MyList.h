#pragma once

template<typename T>
class MyList
{
public:
	struct Node
	{
		T _Data;
		Node* _Prev;
		Node* _Next;
	};

	class iterator
	{
	private:
		Node* _Node;
	public:
		iterator(Node* node = nullptr) : _Node(nullptr)
		{

		}

		iterator operator++(int)
		{
			// 현재 노드를 다음 노드로 이동

		}

		iterator& operator++()
		{
			return *this;
		}

		iterator operator--(int)
		{

		}

		iterator& operator--()
		{
			return *this;
		}

		T& operator*()
		{
			// 현재 노드의 데이터 뽑음
		}
		bool operator==(const iterator& other)
		{

		}
		bool operator!=(const iterator& other)
		{

		}
	};

public:
	MyList()
	{

	}
	~MyList()
	{

	}

	iterator begin()
	{
		// 첫번째 데이터 노드를 가리키는 이터레이터 리턴
	}

	iterator end()
	{
		// Tail 노드를 가리키는 (데이터가 없는 진짜 더미 끝 노드) 이터레이터를 리턴
		// 또는 끝으로 인지할 수 있는 이터레이터를 리턴
	}

	void push_front(T data)
	{
		Node node;
		node._Data = data;

	}

	void push_back(T data)
	{

	}

	void pop_front()
	{

	}

	void pop_back()
	{

	}

	void clear()
	{

	}

	int size()
	{
		return _size;
	}

	bool empty()
	{

	}

	iterator erase(iterator iter)
	{
		// 이터레이터의 그 노드를 지움.
		// 그리고 지운 노드의 다음 노드를 가리키는 이터레이터 리턴
	}

	void remove(T data)
	{

	}

private:
	int _Size = 0;
	Node* _Head;
	Node* _Tail;
};