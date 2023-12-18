#include "Tile.h"

char gTile[GRID_HEIGHT][GRID_WIDTH];

HBRUSH gOpenBrush;
HBRUSH gCloseBrush;
HBRUSH gRouteBrush;

void RenderOpen(HDC hdc)
{
	int X = 0;
	int Y = 0;
	HBRUSH OldBrush = (HBRUSH)SelectObject(hdc, gOpenBrush);
	SelectObject(hdc, GetStockObject(NULL_PEN));
	for (int i = 0; i < GRID_WIDTH; i++)
	{
		for (int j = 0; j < GRID_HEIGHT; j++)
		{
			if (gTile[j][i] == (int)Mode::OPENLIST)
			{
				X = i * GRID_SIZE;
				Y = j * GRID_SIZE;
				// 테두리 크기가 있으므로 + 2 한다.
				Rectangle(hdc, X, Y, X + GRID_SIZE + 2, Y + GRID_SIZE + 2);

				//DrawText(hdc, TEXT("T"), (int)strlen("T"), &rect, DT_TOP | DT_LEFT);
			}
		}
	}
	SelectObject(hdc, OldBrush);
}

void RenderClose(HDC hdc)
{
	int X = 0;
	int Y = 0;
	HBRUSH OldBrush = (HBRUSH)SelectObject(hdc, gCloseBrush);
	SelectObject(hdc, GetStockObject(NULL_PEN));
	for (int i = 0; i < GRID_WIDTH; i++)
	{
		for (int j = 0; j < GRID_HEIGHT; j++)
		{
			if (gTile[j][i] == (int)Mode::CLOSELIST)
			{
				X = i * GRID_SIZE;
				Y = j * GRID_SIZE;
				// 테두리 크기가 있으므로 + 2 한다.
				Rectangle(hdc, X, Y, X + GRID_SIZE + 2, Y + GRID_SIZE + 2);
			}
		}
	}
	SelectObject(hdc, OldBrush);
}

void RenderRoute(HDC hdc)
{
	int X = 0;
	int Y = 0;
	HBRUSH OldBrush = (HBRUSH)SelectObject(hdc, gRouteBrush);
	SelectObject(hdc, GetStockObject(NULL_PEN));
	for (int i = 0; i < GRID_WIDTH; i++)
	{
		for (int j = 0; j < GRID_HEIGHT; j++)
		{
			if (gTile[j][i] == (int)Mode::ROUTE)
			{
				X = i * GRID_SIZE;
				Y = j * GRID_SIZE;
				// 테두리 크기가 있으므로 + 2 한다.
				Rectangle(hdc, X, Y, X + GRID_SIZE + 2, Y + GRID_SIZE + 2);
			}
		}
	}
	SelectObject(hdc, OldBrush);
}

void RenderSearch(HDC hdc)
{
	int X = 0;
	int Y = 0;
	DeleteObject(gRouteBrush);
	gRouteBrush = CreateSolidBrush(RGB(rand() % 255, rand() % 255, rand() % 255));
	HBRUSH OldBrush = (HBRUSH)SelectObject(hdc, gRouteBrush);
	SelectObject(hdc, GetStockObject(NULL_PEN));
	for (int i = 0; i < GRID_WIDTH; i++)
	{
		for (int j = 0; j < GRID_HEIGHT; j++)
		{
			if (gTile[j][i] == (int)Mode::SEARCH)
			{
				X = i * GRID_SIZE;
				Y = j * GRID_SIZE;
				// 테두리 크기가 있으므로 + 2 한다.
				Rectangle(hdc, X, Y, X + GRID_SIZE + 2, Y + GRID_SIZE + 2);
				gTile[j][i] = (int)Mode::SEARCHED;
			}
		}
	}
	SelectObject(hdc, OldBrush);
}
