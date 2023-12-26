#include "JPS.h"
#include "Tile.h"
#include <algorithm>

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
	Init();
	_HDC = GetDC(hWnd);
	_OpenList.push_back(_Start);

	while (_OpenList.size() > 0)
	{
		Node* node = _OpenList[_OpenList.size() - 1];
		_OpenList.pop_back();

		// 도착지 도착
		if (node->_X == _End->_X && node->_Y == _End->_Y)
		{
			Node* RouteStart = node;
			Node* RouteEnd = node->_Parent;
			HPEN FinishPen = CreatePen(PS_SOLID, 2, RGB(255, 0, 0));
			HPEN OldPen = (HPEN)SelectObject(_HDC, FinishPen);
			while (RouteStart != nullptr)
			{
				MoveToEx(_HDC, RouteStart->_X * GRID_SIZE + 8, RouteStart->_Y * GRID_SIZE + 8, NULL);
				LineTo(_HDC, RouteEnd->_X * GRID_SIZE + 8, RouteEnd->_Y * GRID_SIZE + 8);
				RouteEnd = RouteStart;
				RouteStart = RouteEnd->_Parent;
			}
			SelectObject(_HDC, OldPen);
			gTile[_Start->_Y][_Start->_X] = (char)Mode::START;
			gTile[_End->_Y][_End->_X] = (char)Mode::END;
			RenderStart(_HDC);
			RenderEnd(_HDC);
			RenderText(_HDC);

			Clear();
			_OpenList.clear();
			_CloseList.clear();
			break;
		}
		else
		{
			// Close List 추가
			gTile[node->_Y][node->_X] = (char)Mode::CLOSELIST;
			if(node != _Start)
				_CloseList.push_back(node);
			RenderClose(_HDC);
		}

		if (node->_Parent == nullptr)
		{
			Search(node, Direction::RR);
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
			int X = node->_X;
			int Y = node->_Y;
			Search(node, node->_Direct);
			switch (node->_Direct)
			{
			case Direction::RR:
				// 오른쪽으로 향하는 양 대각선 Option 체크 필요
				if (Y >= 1 && X < GRID_WIDTH - 1 &&
					gTile[Y - 1][X] == 1 && gTile[Y - 1][X + 1] == 0)
				{
					Search(node, Direction::RU);
				}
				if (Y < GRID_HEIGHT - 1 && X < GRID_WIDTH - 1 &&
					gTile[Y + 1][X] == 1 && gTile[Y + 1][X + 1] == 0)
				{
					Search(node, Direction::RD);
				}
				break;
			case Direction::RU:
				Search(node, Direction::RR);
				Search(node, Direction::UU);
				if (X >= 1 && Y >= 1 &&
					gTile[Y][X - 1] == 1 && gTile[Y - 1][X - 1] == 0)
				{
					Search(node, Direction::LU);
				}
				if (Y < GRID_HEIGHT - 1 && X < GRID_WIDTH - 1 &&
					gTile[Y + 1][X] == 1 && gTile[Y + 1][X + 1] == 0)
				{
					Search(node, Direction::RD);
				}
				break;
			case Direction::UU:
				if (X >= 1 && Y >= 1 &&
					gTile[Y][X - 1] > 0 && gTile[Y - 1][X - 1] == 0)
				{
					Search(node, Direction::LU);
				}
				if (X < GRID_WIDTH - 1 && Y >= 1 &&
					gTile[Y][X + 1] > 0 && gTile[Y - 1][X + 1] == 0)
				{
					Search(node, Direction::RU);
				}
				break;
			case Direction::LU:
				Search(node, Direction::LL);
				Search(node, Direction::UU);
				if (X < GRID_WIDTH - 1 && Y >= 1 &&
					gTile[Y][X + 1] == 1 && gTile[Y - 1][X + 1] == 0)
				{
					Search(node, Direction::RU);
				}
				if (X >= 1 && Y < GRID_HEIGHT - 1 &&
					gTile[Y + 1][X] == 1 && gTile[Y + 1][X - 1] == 0)
				{
					Search(node, Direction::LD);
				}
				break;
			case Direction::LL:
				if (Y >= 1 && X < GRID_WIDTH - 1 &&
					gTile[Y - 1][X] > 0 && gTile[Y - 1][X - 1] == 0)
				{
					Search(node, Direction::LU);
				}
				if (Y < GRID_HEIGHT - 1 && X >= 1 &&
					gTile[Y + 1][X] > 0 && gTile[Y + 1][X - 1] == 0)
				{
					Search(node, Direction::LD);
				}
				break;
			case Direction::LD:
				Search(node, Direction::LL);
				Search(node, Direction::DD);
				if (Y >= 1 && X >= 1 &&
					gTile[Y - 1][X] == 1 && gTile[Y - 1][X - 1] == 0)
				{
					Search(node, Direction::LU);
				}
				if (X < GRID_WIDTH - 1 && Y < GRID_HEIGHT - 1 &&
					gTile[Y][X + 1] == 1 && gTile[Y + 1][X + 1] == 0)
				{
					Search(node, Direction::RD);
				}
				break;
			case Direction::DD:
				if (X >= 1 && Y >= 1 &&
					gTile[Y][X - 1] > 0 && gTile[Y + 1][X - 1] == 0)
				{
					Search(node, Direction::LD);
				}
				if (X < GRID_WIDTH - 1 && Y >= 1 &&
					gTile[Y][X + 1] > 0 && gTile[Y + 1][X + 1] == 0)
				{
					Search(node, Direction::RD);
				}
				break;
			case Direction::RD:
				Search(node, Direction::RR);
				Search(node, Direction::DD);
				if (Y >= 1 && X < GRID_WIDTH - 1 &&
					gTile[Y - 1][X] == 1 && gTile[Y - 1][X + 1] == 0)
				{
					Search(node, Direction::RU);
				}
				if (X >= 1 && Y < GRID_HEIGHT - 1 &&
					gTile[Y][X - 1] == 1 && gTile[Y + 1][X - 1] == 0)
				{
					Search(node, Direction::LD);
				}
				break;
			}
		}

		RenderSearch(_HDC);
		RenderOpen(_HDC);

		sort(_OpenList.begin(), _OpenList.end(), [](Node* node1, Node* node2) {
			return node1->_F > node2->_F;
		});
	}
}

void JumpPointSearch::Search(Node* node, Direction direct)
{
	int X = node->_X;
	int Y = node->_Y;

	if (node == _Start)
	{
		node->_Direct = direct;
	}

	switch (direct)
	{
	case Direction::RR:
	{
		SearchRR(node, X + 1, Y);
	}
	break;
	case Direction::RU:
	{
		SearchRU(node, X + 1, Y - 1);
	}
	break;
	case Direction::UU:
	{
		SearchUU(node, X, Y - 1);
	}
	break;
	case Direction::LU:
	{
		SearchLU(node, X - 1, Y - 1);
	}
	break;
	case Direction::LL:
	{
		SearchLL(node, X-1, Y);
	}
	break;
	case Direction::LD:
	{
		SearchLD(node, X - 1, Y + 1);
	}
	break;
	case Direction::DD:
	{
		SearchDD(node, X, Y + 1);
	}
	break;
	case Direction::RD:
	{
		SearchRD(node, X+1, Y+1);
	}
	break;
	}
}

bool JumpPointSearch::SearchRR(Node* node, int X, int Y)
{
	int StoreX = X - 1;
	int StoreY = Y;

	// 끝에 도달했다면 더이상 탐색하지 않음.
	while (1)
	{
		if (false == (Y >= 0 && X >= 0 && Y < GRID_HEIGHT && X < GRID_WIDTH))
			return false;

		// 진행 방향 만났을 때 종료.
		if (gTile[Y][X] == 1 || gTile[Y][X] == (char)Mode::CLOSELIST)
			return false;

		if (_End->_X == X && _End->_Y == Y)
			break;

		if (gTile[Y][X] == (char)Mode::OPENLIST)
			break;

		// 노드 생성
		if ((Y >= 1 && X < GRID_WIDTH - 1 &&
			gTile[Y - 1][X] == 1 && gTile[Y - 1][X + 1] == 0)
			|| (Y < GRID_HEIGHT - 1 && X < GRID_WIDTH - 1 &&
			gTile[Y + 1][X] == 1 && gTile[Y + 1][X + 1] == 0))
		{
			break;
		}

		gTile[Y][X] = (char)Mode::SEARCH;
		X = X + 1;
	}

	Node* newNode;
	if (gTile[StoreY][StoreX] != (char)Mode::CLOSELIST)
	{
		Node* storeNode = CreateOpenNode(node, StoreX, StoreY, node->_Direct);
		newNode = CreateOpenNode(storeNode, X, Y, Direction::RR);
	}
	else
	{
		newNode = CreateOpenNode(node, X, Y, Direction::RR);
	}

	return true;
}

void JumpPointSearch::SearchRU(Node* node, int X, int Y)
{
	Direction ParentDirect = node->_Direct;
	node->_Direct = Direction::RU;

	// 끝에 도달했다면 더이상 탐색하지 않음.
	while (Y >= 0 && X >= 0 && Y < GRID_HEIGHT && X < GRID_WIDTH)
	{
		// 진행 방향 만났을 때 종료.
		if (gTile[Y][X] == 1 || gTile[Y][X] == (char)Mode::CLOSELIST)
			break;

		if ((_End->_X == X && _End->_Y == Y) || gTile[Y][X] == (char)Mode::OPENLIST)
		{
			CreateOpenNode(node, X, Y, Direction::RU);
			break;
		}

		// 노드 생성
		if ((X >= 1 && Y >= 1 &&
			gTile[Y][X - 1] == 1 && gTile[Y - 1][X - 1] == 0)
			|| (Y < GRID_HEIGHT - 1 && X < GRID_WIDTH - 1 &&
			gTile[Y + 1][X] == 1 && gTile[Y + 1][X + 1] == 0))
		{
			CreateOpenNode(node, X, Y, Direction::RU);
			break;
		}

		// 수직 조사
		if (SearchUU(node, X, Y - 1))
			break;
		// 수평 조사
		if (SearchRR(node, X + 1, Y))
			break;

		gTile[Y][X] = (char)Mode::SEARCH;
		X = X + 1;
		Y = Y - 1;
	}

	node->_Direct = ParentDirect;
}

bool JumpPointSearch::SearchUU(Node* node, int X, int Y)
{
	int StoreX = X;
	int StoreY = Y + 1;

	// 끝에 도달했다면 더이상 탐색하지 않음.
	while (1)
	{
		if (false == (Y >= 0 && X >= 0 && Y < GRID_HEIGHT && X < GRID_WIDTH))
			return false;

		// 진행 방향 만났을 때 종료.
		if (gTile[Y][X] == 1 || gTile[Y][X] == (char)Mode::CLOSELIST)
			return false;

		if (_End->_X == X && _End->_Y == Y)
			break;

		if (gTile[Y][X] == (char)Mode::OPENLIST)
			break;

		// 노드 생성
		if ((X >= 1 && Y >= 1 &&
			gTile[Y][X - 1] == 1 && gTile[Y - 1][X - 1] == 0)
			|| (X < GRID_WIDTH - 1 && Y >= 1 &&
			gTile[Y][X + 1] == 1 && gTile[Y - 1][X + 1] == 0))
		{
			break;
		}

		gTile[Y][X] = (char)Mode::SEARCH;
		Y = Y - 1;
	}


	Node* newNode;
	if (gTile[StoreY][StoreX] != (char)Mode::CLOSELIST)
	{
		Node* storeNode = CreateOpenNode(node, StoreX, StoreY, node->_Direct);
		newNode = CreateOpenNode(storeNode, X, Y, Direction::UU);
	}
	else
	{
		newNode = CreateOpenNode(node, X, Y, Direction::UU);
	}

	return true;
}

void JumpPointSearch::SearchLU(Node* node, int X, int Y)
{
	Direction ParentDirect = node->_Direct;
	node->_Direct = Direction::LU;

	// 끝에 도달했다면 더이상 탐색하지 않음.
	while (Y >= 0 && X >= 0 && Y < GRID_HEIGHT && X < GRID_WIDTH)
	{
		// 진행 방향 만났을 때 종료.
		if (gTile[Y][X] == 1 || gTile[Y][X] == (char)Mode::CLOSELIST)
			break;

		if ((_End->_X == X && _End->_Y == Y) || gTile[Y][X] == (char)Mode::OPENLIST)
		{
			CreateOpenNode(node, X, Y, Direction::LU);
			break;
		}

		// 노드 생성
		if ((X < GRID_WIDTH - 1 && Y >= 1 &&
			gTile[Y][X + 1] == 1 && gTile[Y - 1][X + 1] == 0)
			|| (X >= 1 && Y < GRID_HEIGHT - 1 &&
			gTile[Y + 1][X] == 1 && gTile[Y + 1][X - 1] == 0))
		{
			CreateOpenNode(node, X, Y, Direction::LU);
			break;
		}

		// 수직 조사
		if (SearchUU(node, X, Y - 1))
			break;
		// 수평 조사
		if (SearchLL(node, X - 1, Y))
			break;
			
		gTile[Y][X] = (char)Mode::SEARCH;
		X = X - 1;
		Y = Y - 1;
	}

	node->_Direct = ParentDirect;
}

bool JumpPointSearch::SearchLL(Node* node, int X, int Y)
{
	int StoreX = X + 1;
	int StoreY = Y;

	// 끝에 도달했다면 더이상 탐색하지 않음.
	while (1)
	{
		if (false == (Y >= 0 && X >= 0 && Y < GRID_HEIGHT && X < GRID_WIDTH))
			return false;

		// 진행 방향 만났을 때 종료.
		if (gTile[Y][X] == 1 || gTile[Y][X] == (char)Mode::CLOSELIST)
			return false;

		if (_End->_X == X && _End->_Y == Y)
			break;

		if (gTile[Y][X] == (char)Mode::OPENLIST)
			break;

		// 노드 생성
		if ((Y >= 1 && X < GRID_WIDTH - 1 &&
			gTile[Y - 1][X] == 1 && gTile[Y - 1][X - 1] == 0)
			|| (Y < GRID_HEIGHT - 1 && X >= 1 &&
			gTile[Y + 1][X] == 1 && gTile[Y + 1][X - 1] == 0))
		{
			break;
		}

		gTile[Y][X] = (char)Mode::SEARCH;
		X = X - 1;
	}

	Node* newNode;
	if (gTile[StoreY][StoreX] != (char)Mode::CLOSELIST)
	{
		Node* storeNode = CreateOpenNode(node, StoreX, StoreY, node->_Direct);
		newNode = CreateOpenNode(storeNode, X, Y, Direction::LL);
	}
	else
	{
		newNode = CreateOpenNode(node, X, Y, Direction::LL);
	}

	return true;
}

void JumpPointSearch::SearchLD(Node* node, int X, int Y)
{
	Direction ParentDirect = node->_Direct;
	node->_Direct = Direction::LD;

	// 끝에 도달했다면 더이상 탐색하지 않음.
	while (Y >= 0 && X >= 0 && Y < GRID_HEIGHT && X < GRID_WIDTH)
	{
		// 진행 방향 만났을 때 종료.
		if (gTile[Y][X] == 1 || gTile[Y][X] == (char)Mode::CLOSELIST)
			break;

		if ((_End->_X == X && _End->_Y == Y) || gTile[Y][X] == (char)Mode::OPENLIST)
		{
			CreateOpenNode(node, X, Y, Direction::LD);
			break;
		}

		// 노드 생성
		if ((Y >= 1 && X >= 1 &&
			gTile[Y - 1][X] == 1 && gTile[Y - 1][X - 1] == 0)
			|| (X < GRID_WIDTH - 1 && Y < GRID_HEIGHT - 1 &&
			gTile[Y][X + 1] == 1 && gTile[Y + 1][X + 1] == 0))
		{
			CreateOpenNode(node, X, Y, Direction::LD);
			break;
		}

		// 수직 조사
		if (SearchDD(node, X, Y + 1))
			break;
		// 수평 조사
		if (SearchLL(node, X - 1, Y))
			break;

		gTile[Y][X] = (char)Mode::SEARCH;
		X = X - 1;
		Y = Y + 1;
	}

	node->_Direct = ParentDirect;
}

bool JumpPointSearch::SearchDD(Node* node, int X, int Y)
{
	int StoreX = X;
	int StoreY = Y - 1;

	// 끝에 도달했다면 더이상 탐색하지 않음.
	while (1)
	{
		if(false == (Y >= 0 && X >= 0 && Y < GRID_HEIGHT && X < GRID_WIDTH))
			return false;

		// 진행 방향 만났을 때 종료.
		if (gTile[Y][X] == 1 || gTile[Y][X] == (char)Mode::CLOSELIST)
			return false;

		if (_End->_X == X && _End->_Y == Y)
			break;

		if (gTile[Y][X] == (char)Mode::OPENLIST)
			break;

		// 노드 생성
		if ((X >= 1 && Y >= 1 &&
			gTile[Y][X - 1] == 1 && gTile[Y + 1][X - 1] == 0)
			|| (X < GRID_WIDTH - 1 && Y >= 1 &&
			gTile[Y][X + 1] == 1 && gTile[Y + 1][X + 1] == 0))
		{
			break;
		}

		gTile[Y][X] = (char)Mode::SEARCH;
		Y = Y + 1;
	}

	Node* newNode;
	if (gTile[StoreY][StoreX] != (char)Mode::CLOSELIST)
	{
		Node* storeNode = CreateOpenNode(node, StoreX, StoreY, node->_Direct);
		newNode = CreateOpenNode(storeNode, X, Y, Direction::DD);
	}
	else
	{
		newNode = CreateOpenNode(node, X, Y, Direction::DD);
	}

	return true;
}

void JumpPointSearch::SearchRD(Node* node, int X, int Y)
{
	Direction ParentDirect = node->_Direct;
	node->_Direct = Direction::RD;

	// 끝에 도달했다면 더이상 탐색하지 않음.
	while (Y >= 0 && X >= 0 && Y < GRID_HEIGHT && X < GRID_WIDTH)
	{
		// 진행 방향 만났을 때 종료.
		if (gTile[Y][X] == 1 || gTile[Y][X] == (char)Mode::CLOSELIST)
			break;

		if ((_End->_X == X && _End->_Y == Y) || gTile[Y][X] == (char)Mode::OPENLIST)
		{
			CreateOpenNode(node, X, Y, Direction::RD);
			break;
		}

		// 노드 생성
		if ((Y >= 1 && X < GRID_WIDTH - 1 &&
			gTile[Y - 1][X] == 1 && gTile[Y - 1][X + 1] == 0)
			|| (X >= 1 && Y < GRID_HEIGHT - 1 &&
			gTile[Y][X - 1] == 1 && gTile[Y + 1][X - 1] == 0))
		{
			CreateOpenNode(node, X, Y, Direction::RD);
			break;
		}

		// 수직 조사
		if (SearchDD(node, X, Y + 1))
			break;
		// 수평 조사
		if (SearchRR(node, X + 1, Y))
			break;

		gTile[Y][X] = (char)Mode::SEARCH;
		X = X + 1;
		Y = Y + 1;
	}

	node->_Direct = ParentDirect;
}

Node* JumpPointSearch::CreateOpenNode(Node* Parent, int X, int Y, Direction Direct)
{
	// OpenList 체크
	for (int i = 0; i < _OpenList.size(); i++)
	{
		if (_OpenList[i]->_X == X && _OpenList[i]->_Y == Y)
		{
			double G = Parent->_G + CalUclide(_OpenList[i], Parent);
			if (G < _OpenList[i]->_G)
			{
				_OpenList[i]->_Parent = Parent;
				_OpenList[i]->_G = G;
				_OpenList[i]->_F = G + _OpenList[i]->_H;
				_OpenList[i]->_Direct = Direct;
			}

			return _OpenList[i];
		}
	}

	Node* newNode = new Node;
	newNode->_X = X;
	newNode->_Y = Y;
	newNode->_G = Parent->_G + CalUclide(newNode, Parent);
	newNode->_H = CalManhatan(newNode, _End);
	newNode->_F = newNode->_G + newNode->_H;
	newNode->_Parent = Parent;
	newNode->_Direct = Direct;
	gTileInfo[Y][X].G = newNode->_G;
	gTileInfo[Y][X].H = newNode->_H;
	_OpenList.push_back(newNode);
	
	gTile[Y][X] = (char)Mode::OPENLIST;
	return newNode;
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

void JumpPointSearch::Init()
{
	for (int i = 0; i < GRID_HEIGHT; i++)
	{
		for (int j = 0; j < GRID_WIDTH; j++)
		{
			if (gTile[i][j] == (int)Mode::SEARCHED)
				gTile[i][j] = 0;

			if (gTile[i][j] == (int)Mode::OPENLIST)
				gTile[i][j] = 0;

			if (gTile[i][j] == (int)Mode::CLOSELIST)
				gTile[i][j] = 0;

			gTileInfo[i][j].G = 0;
			gTileInfo[i][j].H = 0;
		}
	}
}

void JumpPointSearch::Clear()
{
	for (int i = 0; i < _OpenList.size(); i++)
		delete _OpenList[i];

	for (int i = 0; i < _CloseList.size(); i++)
		delete _CloseList[i];
}