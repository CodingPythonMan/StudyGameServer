#pragma once
#include <vector>
#include <windows.h>
using namespace std;

#define DIRECTION 8

struct Node {
	int X;
	int Y;
	double G;
	double H;
	double F;
	Node* Parent;

	Node()
	{
		X = 0;
		Y = 0;
		G = 0;
		H = 0;
		F = 0;
		Parent = nullptr;
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
	
};