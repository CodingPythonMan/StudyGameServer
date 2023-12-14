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
}

void Astar::RoutingStart(HWND hWnd)
{
	// 0. 장애물을 CloseList에 추가한다.
	SetCloseList();

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
			while (nullptr != node)
			{
				gTile[node->_Y][node->_X] = (int)Mode::ROUTE;
				node = node->_Parent;
				InvalidateRect(hWnd, NULL, false);
			}
			return;
		}
		else
		{
			_CloseList.push_back(node);
			gTile[node->_Y][node->_X] = (int)Mode::CLOSELIST;
			InvalidateRect(hWnd, NULL, false);
		}

		// 5. Open List 추가
		bool existInClose;
		int dx, dy;
		for (int i = 0; i < DIRECTION; i++)
		{
			dx = node->_X + _dx[i];
			dy = node->_Y + _dy[i];
			existInClose = false;
			for (int j = 0; j < _CloseList.size(); j++)
			{
				if (_CloseList[j]->_X == dx && _CloseList[j]->_Y == dy)
				{
					existInClose = true;
					break;
				}
			}

			if (existInClose)
				continue;

			Node* newNode = new Node(dx, dy);
			newNode->_Parent = node;
			// 유클리드
			newNode->_G = CalUclide(newNode, _Start);
			// 맨하탄
			newNode->_H = CalManhatan(newNode, _End);
			newNode->_F = newNode->_G + newNode->_H;
			_OpenList.push_back(newNode);

			gTile[newNode->_Y][newNode->_X] = (int)Mode::OPENLIST;
			InvalidateRect(hWnd, NULL, false);
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

double Astar::CalManhatan(Node* node1, Node* node2)
{
	double dx = abs(node2->_X - node1->_X);
	double dy = abs(node2->_Y - node1->_Y);

	return dx + dy;
}

void Astar::SetCloseList()
{
	for (int i = 0; i < GRID_WIDTH; i++)
	{
		for (int j = 0; j < GRID_HEIGHT; j++)
		{
			if (gTile[j][i] == (int)Mode::OBSTACLE)
			{
				Node* node = new Node(i, j);
				_CloseList.push_back(node);
			}
		}
	}
}