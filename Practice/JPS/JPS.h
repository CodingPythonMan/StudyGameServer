#pragma once
#include <vector>
#include <windows.h>
using namespace std;

#define DIRECTION 8

enum class Direction {
	RR,
	RU,
	UU,
	LU,
	LL,
	LD,
	DD,
	RD,
};

struct Node {
	int _X;
	int _Y;
	double _G;
	double _H;
	double _F;
	Node* _Parent;
	Direction _Direct;

	Node()
	{
		_X = 0;
		_Y = 0;
		_G = 0;
		_H = 0;
		_F = 0;
		_Parent = nullptr;
		_Direct = Direction::RR;
	}

	Node(int X, int Y)
	{
		_X = X;
		_Y = Y;
		_G = 0;
		_H = 0;
		_F = 0;
		_Parent = nullptr;
		_Direct = Direction::RR;
	}
};

class JumpPointSearch
{
public:
	JumpPointSearch();
	virtual ~JumpPointSearch();

	void RoutingStart(HWND hWnd);

	void Route(HWND hWnd);

	void Search(Node* node, Direction direct);

	bool SearchRR(Node* node, int X, int Y);
	void SearchRU(Node* node, int X, int Y);
	bool SearchUU(Node* node, int X, int Y);
	void SearchLU(Node* node, int X, int Y);
	bool SearchLL(Node* node, int X, int Y);
	void SearchLD(Node* node, int X, int Y);
	bool SearchDD(Node* node, int X, int Y);
	void SearchRD(Node* node, int X, int Y);

	Node* CreateOpenNode(Node* Parent, int X, int Y, Direction Direct);

	Node* _Start;
	Node* _End;
	vector<Node*> _OpenList;
	vector<Node*> _CloseList;

	HDC _HDC;

private:
	double CalUclide(Node* node1, Node* node2);
	double CalManhatan(Node* node1, Node* node2);

	void Init();
	void Clear();
};