#pragma once
#include <vector>
#include <windows.h>
using namespace std;

#define DIRECTION 8

struct Node {
	int _X;
	int _Y;
	double _G;
	double _H;
	double _F;
	Node* _Parent;

	Node()
	{
		_X = 0;
		_Y = 0;
		_G = 0;
		_H = 0;
		_F = 0;
		_Parent = nullptr;
	}

	Node(int X, int Y)
	{
		_X = X;
		_Y = Y;
		_G = 0;
		_H = 0;
		_F = 0;
		_Parent = nullptr;
	}
};

class Astar
{
public:
	Astar();
	virtual ~Astar();

	void RoutingStart(HWND hWnd);

	Node* _Start;
	Node* _End;
	vector<Node*> _OpenList;
	vector<Node*> _CloseList;

private:
	double CalUclide(Node* node1, Node* node2);
	double CalManhatan(Node* node1, Node* node2);

	void SetCloseList();
};