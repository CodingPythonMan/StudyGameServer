#include "Astar.h"
#include "Tile.h"
#include <algorithm>

int _dx[DIRECTION] = { 1,1,0,-1,-1,-1,0,1 };
int _dy[DIRECTION] = { 0,-1,-1,-1,0,1,1,1 };

Astar::Astar()
{
	_Start = new Node;
	_End = new Node;
}

Astar::~Astar()
{
	delete _Start;
	delete _End;

	for (int i = 0; i < _OpenList.size(); i++)
		delete _OpenList[i];
}

void Astar::RoutingStart(HWND hWnd)
{
	HDC hdc = GetDC(hWnd);

	// 1. 우선 시작점을 OpenList 에 넣는다.
	_OpenList.push_back(_Start);

	// 2. 반복문을 돈다.
	while (1)
	{
		// 3. F가 가장 작은 아이를 참조한다.
		Node* node = _OpenList[(int)_OpenList.size() - 1];
		_OpenList.pop_back();

		// 4. 목적지 도착.
		if (node->_X == _End->_X && node->_Y == _End->_Y)
		{
			node = node->_Parent;
			while (_Start != node)
			{
				gTile[node->_Y][node->_X] = (char)Mode::ROUTE;
				node = node->_Parent;
			}
			_OpenList.clear();
			gTile[_Start->_Y][_Start->_X] = (char)Mode::START;
			RenderRoute(hdc);
			RenderStart(hdc);
			RenderText(hdc);
			return;	
		}
		else
		{
			// 이렇게 속성값 바꾸는 게 CloseList 추가 개념으로 간다.
			gTile[node->_Y][node->_X] = (char)Mode::CLOSELIST;
			RenderClose(hdc);
		}

		// 5. Open List 추가
		int dx, dy;
		for (int i = 0; i < DIRECTION; i++)
		{
			dx = node->_X + _dx[i];
			dy = node->_Y + _dy[i];

			// 경계 범위 밖으로 나가면 안됨.
			if (dx < 0 || dx >= GRID_WIDTH || dy < 0 || dy >= GRID_HEIGHT)
				continue;

			// 없어도 되지만 출발점 색깔 지키기 위해서 남겨둠.
			if (dx == _Start->_X && dy == _Start->_Y)
				continue;

			if (gTile[dy][dx] == (char)Mode::CLOSELIST
				|| gTile[dy][dx] == (char)Mode::OBSTACLE)
			{
				continue;
			}

			if (IsExistOpenList(dx, dy, node))
				continue;

			Node* newNode = new Node(dx, dy);
			newNode->_Parent = node;
			// 유클리드
			newNode->_G = node->_G + CalUclide(newNode, node);
			// 맨하탄
			newNode->_H = CalManhatan(newNode, _End);
			newNode->_F = newNode->_G + newNode->_H;
			gTileInfo[dy][dx].G = newNode->_G;
			gTileInfo[dy][dx].H = newNode->_H;
			_OpenList.push_back(newNode);

			if(dx != _End->_X || dy != _End->_Y)
				gTile[newNode->_Y][newNode->_X] = (int)Mode::OPENLIST;
			RenderOpen(hdc);
		}

		sort(_OpenList.begin(), _OpenList.end(), [](const Node* o1, const Node* o2) {
			return o1->_F > o2->_F;
		});
	}
}

double Astar::CalUclide(Node* node1, Node* node2)
{
	double dx = node2->_X - node1->_X;
	double dy = node2->_Y - node1->_Y;

	return sqrt(dx*dx + dy*dy);
}

double Astar::CalUclide(int X, int Y, Node* node)
{
	double dx = X - node->_X;
	double dy = Y - node->_Y;

	return sqrt(dx * dx + dy * dy);
}

double Astar::CalManhatan(Node* node1, Node* node2)
{
	double dx = abs(node2->_X - node1->_X);
	double dy = abs(node2->_Y - node1->_Y);

	return dx + dy;
}

bool Astar::IsExistOpenList(int X, int Y, Node* node)
{
	for (int i = 0; i < _OpenList.size(); i++)
	{
		if (_OpenList[i]->_X == X && _OpenList[i]->_Y == Y)
		{
			double G = node->_G + CalUclide(X, Y, node);
			// 이번에 들어온 X, Y 가 G가 가깝다면 새롭게 Parent 설정
			if (G < _OpenList[i]->_G)
			{
				_OpenList[i]->_Parent = node;
				_OpenList[i]->_G = G;
				_OpenList[i]->_F = G + _OpenList[i]->_H;
			}

			return true;
		}
	}
	return false;
}