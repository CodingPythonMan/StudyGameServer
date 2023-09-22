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

		friend MyList;
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

		_Size++;
	}

	void push_back(T data)
	{
		Node* node = new Node();
		node->_Data = data;

		_Tail._Prev->_Next = node;
		node->_Prev = _Tail._Prev;
		node->_Next = &_Tail;
		_Tail._Prev = node;

		_Size++;
	}

	void pop_front()
	{
		Node* node = _Head._Next;
		_Head._Next = node->_Next;
		node->_Next->_Prev = node->_Prev;

		delete(node);

		_Size--;
	}

	void pop_back()
	{
		Node* node = _Tail._Prev;
		_Tail._Prev = node->_Prev;
		node->_Prev->_Next = node->_Next;

		delete(node);

		_Size--;
	}

	void clear()
	{
		Node* node = _Head._Next;
		_Head._Next = &_Tail;
		_Tail._Prev = &_Head;

		while (node != &_Tail)
		{
			Node* nextNode = node->_Next;
			delete(node);
			node = nextNode;
			_Size--;
		}
	}

	int size()
	{
		return _Size;
	}

	bool empty()
	{
		if (_Size <= 0)
			return true;
		else
			return false;
	}

	iterator erase(iterator iter)
	{
		// 이터레이터의 그 노드를 지움.
		// 그리고 지운 노드의 다음 노드를 가리키는 이터레이터 리턴
		iterator newIter = ++iter;
		delete(iter._Node);
		return newIter;
	}

	void remove(T data)
	{
		for (iterator iter = begin(); iter != end(); ++iter)
		{
			if (*iter == data)
			{
				erase(iter);
			}
		}
	}

private:
	int _Size = 0;
	Node _Head;
	Node _Tail;
};