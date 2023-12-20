#include "Tile.h"
#include <stdio.h>

char gTile[GRID_HEIGHT][GRID_WIDTH];
double gTileF[GRID_HEIGHT][GRID_WIDTH];

HBRUSH gOpenBrush;
HBRUSH gCloseBrush;
HBRUSH gRouteBrush;

void RenderOpen(HDC hdc)
{
	int X = 0;
	int Y = 0;
	HBRUSH OldBrush = (HBRUSH)SelectObject(hdc, gOpenBrush);
	SelectObject(hdc, GetStockObject(NULL_PEN));

	HFONT hFont = CreateFont(10, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, ANSI_CHARSET,
		OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, L"Arial");

	SelectObject(hdc, hFont);

	SetBkMode(hdc, TRANSPARENT);
	WCHAR arr[10] = {0};

	for (int i = 0; i < GRID_WIDTH; i++)
	{
		for (int j = 0; j < GRID_HEIGHT; j++)
		{
			if (gTile[j][i] == (char)Mode::OPENLIST)
			{
				X = i * GRID_SIZE;
				Y = j * GRID_SIZE;
				// 테두리 크기가 있으므로 + 2 한다.
				Rectangle(hdc, X, Y, X + GRID_SIZE + 2, Y + GRID_SIZE + 2);

				swprintf_s(arr, L"%.1f", gTileF[j][i]);
				TextOut(hdc, X, Y, arr, (int)wcslen(arr));
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
