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

struct Info {
	double G;
	double H;
};

extern char gTile[GRID_HEIGHT][GRID_WIDTH];            // 0 장애물 없음 / 1 장애물 있음
extern Info gTileInfo[GRID_HEIGHT][GRID_WIDTH];            // 0 장애물 없음 / 1 장애물 있음

extern HBRUSH gOpenBrush;
extern HBRUSH gCloseBrush;
extern HBRUSH gRouteBrush;
extern HBRUSH gStartBrush;
extern HBRUSH gEndBrush;

void RenderOpen(HDC hdc);
void RenderClose(HDC hdc);
void RenderRoute(HDC hdc);
void RenderStart(HDC hdc);
void RenderEnd(HDC hdc);

void RenderText(HDC hdc);