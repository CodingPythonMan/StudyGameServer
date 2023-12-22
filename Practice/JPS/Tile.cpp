#include "Tile.h"
#include <stdio.h>

char gTile[GRID_HEIGHT][GRID_WIDTH];
Info gTileInfo[GRID_HEIGHT][GRID_WIDTH];

HBRUSH gOpenBrush;
HBRUSH gCloseBrush;
HBRUSH gStartBrush;
HBRUSH gEndBrush;
HBRUSH gRouteBrush;
HPEN gGridPen;

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
			if (gTile[j][i] == (char)Mode::CLOSELIST)
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

void RenderStart(HDC hdc)
{
	int X = 0;
	int Y = 0;
	HBRUSH OldBrush = (HBRUSH)SelectObject(hdc, gStartBrush);
	SelectObject(hdc, GetStockObject(NULL_PEN));
	for (int i = 0; i < GRID_WIDTH; i++)
	{
		for (int j = 0; j < GRID_HEIGHT; j++)
		{
			if (gTile[j][i] == (int)Mode::START)
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

void RenderEnd(HDC hdc)
{
	int X = 0;
	int Y = 0;
	HBRUSH OldBrush = (HBRUSH)SelectObject(hdc, gEndBrush);
	SelectObject(hdc, GetStockObject(NULL_PEN));
	for (int i = 0; i < GRID_WIDTH; i++)
	{
		for (int j = 0; j < GRID_HEIGHT; j++)
		{
			if (gTile[j][i] == (int)Mode::END)
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
	unsigned char redColor = rand() % 106 + 150;
	unsigned char greenColor = rand() % 106 + 150;
	unsigned char blueColor = rand() % 106 + 150;
	gRouteBrush = CreateSolidBrush(RGB(redColor, greenColor, blueColor));
	HBRUSH OldBrush = (HBRUSH)SelectObject(hdc, gRouteBrush);
	SelectObject(hdc, GetStockObject(NULL_PEN));
	for (int i = 0; i < GRID_HEIGHT; i++)
	{
		for (int j = 0; j < GRID_WIDTH; j++)
		{
			if (gTile[i][j] == (int)Mode::SEARCH)
			{
				Y = i * GRID_SIZE;
				X = j * GRID_SIZE;
				// 테두리 크기가 있으므로 + 2 한다.
				Rectangle(hdc, X, Y, X + GRID_SIZE + 2, Y + GRID_SIZE + 2);
				gTile[i][j] = (int)Mode::SEARCHED;
			}
		}
	}
	SelectObject(hdc, OldBrush);
	RenderGrid(hdc);
}

void RenderGrid(HDC hdc)
{
	int X = 0;
	int Y = 0;
	HPEN OldPen = (HPEN)SelectObject(hdc, gGridPen);
	// 그리드의 마지막 선을 추가로 그리기 위해 <= 의 반복 조건
	for (int i = 0; i <= GRID_WIDTH; i++)
	{
		MoveToEx(hdc, X, 0, NULL);
		LineTo(hdc, X, GRID_HEIGHT * GRID_SIZE);
		X += GRID_SIZE;
	}

	for (int i = 0; i <= GRID_HEIGHT; i++)
	{
		MoveToEx(hdc, 0, Y, NULL);
		LineTo(hdc, GRID_WIDTH * GRID_SIZE, Y);
		Y += GRID_SIZE;
	}
	SelectObject(hdc, OldPen);
}

void RenderText(HDC hdc)
{
	int X = 0;
	int Y = 0;
	HFONT hFont = CreateFont(10, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, ANSI_CHARSET,
		OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, L"Arial");

	SelectObject(hdc, hFont);

	SetBkMode(hdc, TRANSPARENT);
	WCHAR G[10] = { 0 };
	WCHAR H[10] = { 0 };

	for (int i = 0; i < GRID_HEIGHT; i++)
	{
		for (int j = 0; j < GRID_WIDTH; j++)
		{
			if (gTileInfo[i][j].G != 0)
			{
				X = j * GRID_SIZE;
				Y = i * GRID_SIZE;
				swprintf_s(G, L"%.1f", gTileInfo[i][j].G);
				TextOut(hdc, X, Y, G, (int)wcslen(H));
				swprintf_s(H, L"%.1f", gTileInfo[i][j].H);
				TextOut(hdc, X, Y + 7, H, (int)wcslen(G));
			}
		}
	}
}