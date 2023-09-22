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
		iterator(Node* node = nullptr)
		{
			_Node = node;
		}

		iterator operator++(int)
		{
			iterator it(_Node);
			// 현재 노드를 다음 노드로 이동
			_Node = _Node->_Next;
			return it;
		}

		iterator& operator++()
		{
			_Node = _Node->_Next;
			return *this;
		}

		iterator operator--(int)
		{
			iterator it(_Node);
			_Node = _Node->_Prev;
			return it;
		}

		iterator& operator--()
		{
			_Node = _Node->_Prev;
			return *this;
		}

		T& operator*()
		{
			// 현재 노드의 데이터 뽑음
			return _Node->_Data;
		}
		bool operator==(const iterator& other)
		{
			if (_Node == other._Node)
				return true;
			return false;
		}
		bool operator!=(const iterator& other)
		{
			if (_Node == other._Node)
				return false;
			return true;
		}
	};

public:
	MyList()
	{
		_Head._Next = &_Tail;
		_Tail._Prev = &_Head;
	}
	~MyList()
	{
	}

	iterator begin()
	{
		// 첫번째 데이터 노드를 가리키는 이터레이터 리턴
		iterator it(_Head._Next);
		return it;
	}

	iterator end()
	{
		// Tail 노드를 가리키는 (데이터가 없는 진짜 더미 끝 노드) 이터레이터를 리턴
		// 또는 끝으로 인지할 수 있는 이터레이터를 리턴
		iterator it(&_Tail);
		return it;
	}

	void push_front(T data)
	{
		Node* node = new Node();
		node->_Data = data;

		_Head._Next->_Prev = node;
		node->_Next = _Head._Next;
		node->_Prev = &_Head;
		_Head._Next = node;
	}

	void push_back(T data)
	{
		Node* node = new Node();
		node->_Data = data;

		_Tail._Prev->_Next = node;
		node->_Prev = _Tail._Prev;
		node->_Next = &_Tail;
		_Tail._Prev = node;
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
		return _Size;
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
	Node _Head;
	Node _Tail;
};