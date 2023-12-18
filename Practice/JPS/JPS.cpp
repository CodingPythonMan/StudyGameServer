#include "JPS.h"

int _dx[DIRECTION] = { 1,1,0,-1,-1,-1,0,1 };
int _dy[DIRECTION] = { 0,-1,-1,-1,0,1,1,1 };

JumpPointSearch::JumpPointSearch()
{
	_Start = new Node;
	_End = new Node;
	_HDC = nullptr;
}

JumpPointSearch::~JumpPointSearch()
{
	delete _Start;
	delete _End;
}

void JumpPointSearch::RoutingStart(HWND hWnd)
{
	HDC _HDC = GetDC(hWnd);
	_OpenList.push_back(_Start);

	while (_OpenList.size() > 0)
	{
		Node* node = _OpenList[_OpenList.size() - 1];
		_OpenList.pop_back();

		if (node->_Parent == nullptr)
		{
			if(Search(node, Direction::RR))

			Search(node, Direction::RU);
			Search(node, Direction::UU);
			Search(node, Direction::LU);
			Search(node, Direction::LL);
			Search(node, Direction::LD);
			Search(node, Direction::DD);
			Search(node, Direction::RD);
		}
		else
		{
			switch (node->_Direct)
			{
			case Direction::RR:
				// 오른쪽으로 향하는 양 대각선 Option 체크 필요
				Search(node, Direction::RR);
				break;
			case Direction::RU:
				//Search(node, node->_Direct);
				break;
			case Direction::UU:
				break;
			case Direction::LU:
				break;
			case Direction::LL:
				break;
			case Direction::LD:
				break;
			case Direction::DD:
				break;
			case Direction::RD:
				break;
			}
		}
	}
}

bool JumpPointSearch::Search(Node* node, Direction direct)
{
	switch (direct)
	{
	case Direction::RR:
	{
		int X = node->_X + 1;
		int Y = node->_Y;
		// 끝에 도달했다면 더이상 탐색하지 않음.
		while (Y >= 0 && X >= 0 && Y <= GRID_HEIGHT && X <= GRID_WIDTH)
		{
			if (_End->_X == X && _End->_Y == Y)
				return true;
			
			// 노드 생성
			if (IsRUConner(X, Y) || IsRDConner(X, Y))
			{
				CreateOpenNode(X, Y);
				RenderSearch(_HDC);
				break;
			}

			gTile[Y][X] = (int)Mode::SEARCH;
			X = X + 1;
		}
		RenderSearch(_HDC);
	}
	break;
	case Direction::RU:
		break;
	case Direction::UU:
		break;
	case Direction::LU:
		break;
	case Direction::LL:
		break;
	case Direction::LD:
		break;
	case Direction::DD:
		break;
	case Direction::RD:
		break;
	}

	return false;
}

bool JumpPointSearch::IsRUConner(int X, int Y)
{
	if (gTile[Y - 1][X] == 0 && gTile[Y - 1][X + 1] > 0)
		return true;

	return false;
}

bool JumpPointSearch::IsRDConner(int X, int Y)
{
	if (gTile[Y + 1][X] == 0 && gTile[Y + 1][X + 1] > 0)
		return true;

	return false;
}

void JumpPointSearch::CreateOpenNode(int X, int Y)
{
	Node* newNode = new Node;
	newNode->_X = X;
	newNode->_Y = Y;
	newNode->_G = CalUclide(newNode, _Start);
	newNode->_H = CalManhatan(newNode, _Start);
	newNode->_F = newNode->_G + newNode->_H;
	_OpenList.push_back(newNode);
	
	gTile[Y][X] = (int)Mode::OPENLIST;
	RenderOpen(_HDC);
}

double JumpPointSearch::CalUclide(Node* node1, Node* node2)
{
	double dx = node2->_X - node1->_X;
	double dy = node2->_Y - node1->_Y;

	return sqrt(dx * dx + dy * dy);
}

double JumpPointSearch::CalManhatan(Node* node1, Node* node2)
{
	double dx = abs(node2->_X - node1->_X);
	double dy = abs(node2->_Y - node1->_Y);

	return dx + dy;
}

bool JumpPointSearch::IsExistCloseList(int X, int Y)
{
	return false;
}

bool JumpPointSearch::IsExistOpenList(int X, int Y, Node* Parent)
{
	return false;
}