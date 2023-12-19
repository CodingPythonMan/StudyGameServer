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
	Init();
	_HDC = GetDC(hWnd);
	_OpenList.push_back(_Start);

	while (_OpenList.size() > 0)
	{
		Node* node = _OpenList[_OpenList.size() - 1];
		_OpenList.pop_back();

		if (node->_Parent == nullptr)
		{
			if (Search(node, Direction::RR))
				break;

			Search(node, Direction::RU);
			Search(node, Direction::UU);
			Search(node, Direction::LU);
			Search(node, Direction::LL);
			Search(node, Direction::LD);
			Search(node, Direction::DD);
			Search(node, Direction::RD);

			RenderSearch(_HDC);
			RenderOpen(_HDC);
		}
		else
		{
			switch (node->_Direct)
			{
			case Direction::RR:
				// 오른쪽으로 향하는 양 대각선 Option 체크 필요
				//Search(node, Direction::RR);
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

	Node* RouteStart = _End;
	Node* RouteEnd = _End->_Parent;
	HPEN gFinishPen = CreatePen(PS_SOLID, 1, RGB(255, 0,0));
	HPEN OldPen = (HPEN)SelectObject(_HDC, gFinishPen);
	while (RouteStart != nullptr)
	{
		MoveToEx(_HDC, RouteStart->_X * GRID_SIZE + 8, RouteStart->_Y * GRID_SIZE + 8, NULL);
		LineTo(_HDC, RouteEnd->_X * GRID_SIZE + 8, RouteEnd->_Y * GRID_SIZE + 8);
		RouteEnd = RouteStart;
		RouteStart = RouteEnd->_Parent;
	}
	SelectObject(_HDC, OldPen);
}

bool JumpPointSearch::Search(Node* node, Direction direct)
{
	int X = node->_X;
	int Y = node->_Y;

	switch (direct)
	{
	case Direction::RR:
	{
		return SearchRR(node, X + 1, Y);
	}
	break;
	case Direction::RU:
	{
		return SearchRU(node, X + 1, Y - 1);
	}
	break;
	case Direction::UU:
	{
		return SearchUU(node, X, Y - 1);
	}
	break;
	case Direction::LU:
	{
		return SearchLU(node, X - 1, Y - 1);
	}
	break;
	case Direction::LL:
	{
		return SearchLL(node, X-1, Y);
	}
	break;
	case Direction::LD:
	{
		return SearchLD(node, X - 1, Y + 1);
	}
	break;
	case Direction::DD:
	{
		return SearchDD(node, X, Y + 1);
	}
	break;
	case Direction::RD:
	{
		return SearchRD(node, X+1, Y+1);
	}
	break;
	}

	return false;
}

bool JumpPointSearch::SearchRR(Node* node, int X, int Y)
{
	// 끝에 도달했다면 더이상 탐색하지 않음.
	while (Y >= 0 && X >= 0 && Y < GRID_HEIGHT && X < GRID_WIDTH)
	{
		// 진행 방향 만났을 때 종료.
		if (gTile[Y][X] > 0)
			break;

		if (_End->_X == X && _End->_Y == Y)
		{
			_End->_Parent = node;
			return true;
		}

		// 노드 생성
		if ((Y >= 1 && X < GRID_WIDTH - 1 &&
			gTile[Y - 1][X] == 1 && gTile[Y - 1][X + 1] == 0)
			|| (Y < GRID_HEIGHT - 1 && X < GRID_WIDTH - 1 &&
				gTile[Y + 1][X] == 1 && gTile[Y + 1][X + 1] == 0))
		{
			CreateOpenNode(node, X, Y);
			break;
		}

		gDesignTile[Y][X] = (int)Mode::SEARCH;
		X = X + 1;
	}

	return false;
}

bool JumpPointSearch::SearchRU(Node* node, int X, int Y)
{
	// 끝에 도달했다면 더이상 탐색하지 않음.
	while (Y >= 0 && X >= 0 && Y < GRID_HEIGHT && X < GRID_WIDTH)
	{
		// 진행 방향 만났을 때 종료.
		if (gTile[Y][X] > 0)
			break;

		if (_End->_X == X && _End->_Y == Y)
		{
			_End->_Parent = node;
			return true;
		}			

		// 수직 조사
		if (SearchUU(node, X, Y - 1))
		{
			_End->_Parent = CreateOpenNode(node, X, Y);
			return true;
		}
			
		// 수평 조사
		if (SearchRR(node, X + 1, Y))
		{
			_End->_Parent = CreateOpenNode(node, X, Y);
			return true;
		}

		// 노드 생성
		if ((X >= 1 && Y >= 1 &&
			gTile[Y][X - 1] == 1 && gTile[Y - 1][X - 1] == 0)
			|| (Y < GRID_HEIGHT - 1 && X < GRID_WIDTH - 1 &&
			gTile[Y + 1][X] == 1 && gTile[Y + 1][X + 1] == 0))
		{
			CreateOpenNode(node, X, Y);
			break;
		}

		gDesignTile[Y][X] = (int)Mode::SEARCH;
		X = X + 1;
		Y = Y - 1;
	}

	return false;
}

bool JumpPointSearch::SearchUU(Node* node, int X, int Y)
{
	// 끝에 도달했다면 더이상 탐색하지 않음.
	while (Y >= 0 && X >= 0 && Y < GRID_HEIGHT && X < GRID_WIDTH)
	{
		// 진행 방향 만났을 때 종료.
		if (gTile[Y][X] > 0)
			break;

		if (_End->_X == X && _End->_Y == Y)
		{
			_End->_Parent = node;
			return true;
		}

		// 노드 생성
		if ((X >= 1 && Y >= 1 &&
			gTile[Y][X - 1] > 0 && gTile[Y - 1][X - 1] == 0)
			|| (X < GRID_WIDTH - 1 && Y >= 1 &&
			gTile[Y][X + 1] > 0 && gTile[Y - 1][X + 1] == 0))
		{
			CreateOpenNode(node, X, Y);
			break;
		}

		gDesignTile[Y][X] = (int)Mode::SEARCH;
		Y = Y - 1;
	}

	return false;
}

bool JumpPointSearch::SearchLU(Node* node, int X, int Y)
{
	// 끝에 도달했다면 더이상 탐색하지 않음.
	while (Y >= 0 && X >= 0 && Y < GRID_HEIGHT && X < GRID_WIDTH)
	{
		// 진행 방향 만났을 때 종료.
		if (gTile[Y][X] > 0)
			break;

		if (_End->_X == X && _End->_Y == Y)
		{
			_End->_Parent = node;
			return true;
		}

		// 수직 조사
		if (SearchUU(node, X, Y - 1))
		{
			_End->_Parent = CreateOpenNode(node, X, Y);
			return true;
		}
			
		// 수평 조사
		if (SearchLL(node, X - 1, Y))
		{
			_End->_Parent = CreateOpenNode(node, X, Y);
			return true;
		}
			
		// 노드 생성
		if ((X < GRID_WIDTH - 1 && Y >= 1 &&
			gTile[Y][X + 1] > 0 && gTile[Y - 1][X + 1] == 0)
			|| (X >= 1 && Y < GRID_HEIGHT - 1 &&
			gTile[Y + 1][X] > 0 && gTile[Y + 1][X - 1] == 0))
		{
			CreateOpenNode(node, X, Y);
			break;
		}

		gDesignTile[Y][X] = (int)Mode::SEARCH;
		X = X - 1;
		Y = Y - 1;
	}

	return false;
}

bool JumpPointSearch::SearchLL(Node* node, int X, int Y)
{
	// 끝에 도달했다면 더이상 탐색하지 않음.
	while (Y >= 0 && X >= 0 && Y < GRID_HEIGHT && X < GRID_WIDTH)
	{
		// 진행 방향 만났을 때 종료.
		if (gTile[Y][X] > 0)
			break;

		if (_End->_X == X && _End->_Y == Y)
		{
			_End->_Parent = node;
			return true;
		}

		// 노드 생성
		if ((Y >= 1 && X < GRID_WIDTH - 1 &&
			gTile[Y - 1][X] > 0 && gTile[Y - 1][X - 1] == 0)
			|| (Y < GRID_HEIGHT - 1 && X >= 1 &&
			gTile[Y + 1][X] > 0 && gTile[Y + 1][X - 1] == 0))
		{
			CreateOpenNode(node, X, Y);
			break;
		}

		gDesignTile[Y][X] = (int)Mode::SEARCH;
		X = X - 1;
	}

	return false;
}

bool JumpPointSearch::SearchLD(Node* node, int X, int Y)
{
	// 끝에 도달했다면 더이상 탐색하지 않음.
	while (Y >= 0 && X >= 0 && Y < GRID_HEIGHT && X < GRID_WIDTH)
	{
		// 진행 방향 만났을 때 종료.
		if (gTile[Y][X] > 0)
			break;

		if (_End->_X == X && _End->_Y == Y)
		{
			_End->_Parent = node;
			return true;
		}

		// 수직 조사
		if (SearchDD(node, X, Y + 1))
		{
			_End->_Parent = CreateOpenNode(node, X, Y);
			return true;
		}
			
		// 수평 조사
		if (SearchLL(node, X - 1, Y))
		{
			_End->_Parent = CreateOpenNode(node, X, Y);
			return true;
		}

		// 노드 생성
		if ((Y >= 1 && X >= 1 &&
			gTile[Y - 1][X] > 0 && gTile[Y - 1][X - 1] == 0)
			|| (X < GRID_WIDTH - 1 && Y < GRID_HEIGHT - 1 &&
			gTile[Y][X + 1] > 0 && gTile[Y + 1][X + 1] == 0))
		{
			CreateOpenNode(node, X, Y);
			break;
		}

		gDesignTile[Y][X] = (int)Mode::SEARCH;
		X = X - 1;
		Y = Y + 1;
	}
	return false;
}

bool JumpPointSearch::SearchDD(Node* node, int X, int Y)
{
	// 끝에 도달했다면 더이상 탐색하지 않음.
	while (Y >= 0 && X >= 0 && Y < GRID_HEIGHT && X < GRID_WIDTH)
	{
		// 진행 방향 만났을 때 종료.
		if (gTile[Y][X] > 0)
			break;

		if (_End->_X == X && _End->_Y == Y)
		{
			_End->_Parent = node;
			return true;
		}

		// 노드 생성
		if ((X >= 1 && Y >= 1 &&
			gTile[Y - 1][X - 1] > 0 && gTile[Y][X - 1] == 0)
			|| (X < GRID_WIDTH - 1 && Y >= 1 &&
			gTile[Y - 1][X + 1] > 0 && gTile[Y][X + 1] == 0))
		{
			CreateOpenNode(node, X, Y);
			break;
		}

		gDesignTile[Y][X] = (int)Mode::SEARCH;
		Y = Y + 1;
	}
	return false;
}

bool JumpPointSearch::SearchRD(Node* node, int X, int Y)
{
	// 끝에 도달했다면 더이상 탐색하지 않음.
	while (Y >= 0 && X >= 0 && Y < GRID_HEIGHT && X < GRID_WIDTH)
	{
		// 진행 방향 만났을 때 종료.
		if (gTile[Y][X] > 0)
			break;

		if (_End->_X == X && _End->_Y == Y)
		{
			_End->_Parent = node;
			return true;
		}

		// 수직 조사
		if (SearchDD(node, X, Y + 1))
		{
			_End->_Parent = CreateOpenNode(node, X, Y);
			return true;
		}
			
		// 수평 조사
		if (SearchRR(node, X + 1, Y))
		{
			_End->_Parent = CreateOpenNode(node, X, Y);
			return true;
		}
			
		// 노드 생성
		if ((Y >= 1 && X < GRID_WIDTH - 1 &&
			gTile[Y - 1][X] > 0 && gTile[Y - 1][X + 1] == 0)
			|| (X >= 1 && Y < GRID_HEIGHT - 1 &&
			gTile[Y][X - 1] > 0 && gTile[Y + 1][X - 1] == 0))
		{
			CreateOpenNode(node, X, Y);
			break;
		}

		gDesignTile[Y][X] = (int)Mode::SEARCH;
		X = X + 1;
		Y = Y + 1;
	}
	return false;
}

Node* JumpPointSearch::CreateOpenNode(Node* Parent, int X, int Y)
{
	Node* newNode = new Node;
	newNode->_X = X;
	newNode->_Y = Y;
	newNode->_G = CalUclide(newNode, _Start);
	newNode->_H = CalManhatan(newNode, _Start);
	newNode->_F = newNode->_G + newNode->_H;
	newNode->_Parent = Parent;
	_OpenList.push_back(newNode);
	
	gTile[Y][X] = (int)Mode::OPENLIST;

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

bool JumpPointSearch::IsExistCloseList(int X, int Y)
{
	return false;
}

bool JumpPointSearch::IsExistOpenList(int X, int Y, Node* Parent)
{
	return false;
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
		}
	}
}