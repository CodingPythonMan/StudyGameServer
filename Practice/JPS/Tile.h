#pragma once

#include <windows.h>
#include "Pos.h"

#define GRID_SIZE 16
#define GRID_WIDTH 100
#define GRID_HEIGHT 50

// 모드 정하기
enum class Mode
{
	OBSTACLE = 1,
	START,
	END,
	OPENLIST,
	CLOSELIST,
	SEARCHED,
	SEARCH
};

extern char gTile[GRID_HEIGHT][GRID_WIDTH];            // 0 장애물 없음 / 1 장애물 있음

extern HBRUSH gOpenBrush;
extern HBRUSH gCloseBrush;
extern HBRUSH gRouteBrush;
extern HPEN gGridPen;

void RenderOpen(HDC hdc);
void RenderClose(HDC hdc);

void RenderSearch(HDC hdc);
void RenderGrid(HDC hdc);