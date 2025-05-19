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
	HDC hdc = GetDC(hWnd);

	// 1. �켱 �������� OpenList �� �ִ´�.
	_OpenList.push_back(_Start);

	// 2. �ݺ����� ����.
	while (1)
	{
		Sleep(1000);
		// 3. F�� ���� ���� ���̸� �����Ѵ�.
		Node* node = _OpenList[(int)_OpenList.size() - 1];
		_OpenList.pop_back();

		// 4. ������ ����.
		if (node->_X == _End->_X && node->_Y == _End->_Y)
		{
			node = node->_Parent;
			while (_Start != node)
			{
				gTile[node->_Y][node->_X] = (char)Mode::ROUTE;
				node = node->_Parent;
			}

			gTile[_Start->_Y][_Start->_X] = (char)Mode::START;
			RenderRoute(hdc);
			RenderStart(hdc);
			RenderText(hdc);

			Clear();
			_OpenList.clear();
			_CloseList.clear();
			return;	
		}
		else
		{
			// �̷��� �Ӽ��� �ٲٴ� �� CloseList �߰� �������� ����.
			gTile[node->_Y][node->_X] = (char)Mode::CLOSELIST;
			if(node != _Start)
				_CloseList.push_back(node);
			RenderClose(hdc);
		}

		// 5. Open List �߰�
		int dx, dy;
		for (int i = 0; i < DIRECTION; i++)
		{
			dx = node->_X + _dx[i];
			dy = node->_Y + _dy[i];

			// ��� ���� ������ ������ �ȵ�.
			if (dx < 0 || dx >= GRID_WIDTH || dy < 0 || dy >= GRID_HEIGHT)
				continue;

			// ��� ������ ����� ���� ��Ű�� ���ؼ� ���ܵ�.
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
			// ��Ŭ����
			newNode->_G = node->_G + CalUclide(newNode, node);
			// ����ź
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
			// �̹��� ���� X, Y �� G�� �����ٸ� ���Ӱ� Parent ����
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

void Astar::Clear()
{
	for (int i = 0; i < _OpenList.size(); i++)
		delete _OpenList[i];

	for (int i = 0; i < _CloseList.size(); i++)
		delete _CloseList[i];
}