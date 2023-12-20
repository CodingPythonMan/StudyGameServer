#pragma once

#include <windows.h>

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
	ROUTE
};

extern char gTile[GRID_HEIGHT][GRID_WIDTH];            // 0 장애물 없음 / 1 장애물 있음
extern double gTileF[GRID_HEIGHT][GRID_WIDTH];            // 0 장애물 없음 / 1 장애물 있음

extern HBRUSH gOpenBrush;
extern HBRUSH gCloseBrush;
extern HBRUSH gRouteBrush;

void RenderOpen(HDC hdc);
void RenderClose(HDC hdc);
void RenderRoute(HDC hdc);