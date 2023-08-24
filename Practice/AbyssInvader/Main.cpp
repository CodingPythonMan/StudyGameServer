#include <stdio.h>
#include <memory.h>
#include <Windows.h>
#include <stdbool.h>
#include "Console.h"
#include "Main.h"

void Player_Initial(void)
{
	//-------------------------------------------------------------------
	// 플레이어 최초 데이터 초기화.
	// HP : 3 
	// 39,19 위치에서 시작 함.
	//-------------------------------------------------------------------
	_Player.HP = 3;
	_Player.Visible = true;

	_Player.X = 39;
	_Player.Y = 19;
}

void Buffer_Flip(void)
{
	for (int count = 0; count < SCREEN_HEIGHT; count++)
	{
		cs_MoveCursor(0, count);
		printf(ScreenBuffer[count]);
	}
}

void Buffer_Clear(void)
{
	for (int count = 0; count < SCREEN_HEIGHT; count++)
	{
		memset(ScreenBuffer[count], ' ', SCREEN_WIDTH);
		ScreenBuffer[count][SCREEN_WIDTH - 1] = (char)NULL;
	}
}

void Sprite_Draw(int X, int Y, char Sprite)
{
	if (X < 0 || Y < 0 || X >= SCREEN_WIDTH - 1 || Y >= SCREEN_HEIGHT)
		return;

	ScreenBuffer[Y][X] = Sprite;
}

bool KeyProcess(void)
{
	// 왼쪽 방향키.
	if (GetAsyncKeyState(VK_LEFT))
	{
		_Player.X -= 1;
	}
	// 오른쪽 방향키.
	if (GetAsyncKeyState(VK_RIGHT))
	{
		_Player.X += 1;
	}
	// 위쪽 방향키.
	if (GetAsyncKeyState(VK_UP) & 0x8001)
	{
		_Player.Y--;
	}
	// 아래쪽 방향키.
	if (GetAsyncKeyState(VK_DOWN) & 0x8001)
	{
		_Player.Y++;
	}

	//-------------------------------------------------------------
	// 플레이어 이동 반경 제한.
	// 게임 화면에서 플레이어가 이동 가능한 구역을 제한한다.
	//-------------------------------------------------------------

	_Player.X = max(_Player.X, 0);
	_Player.X = min(_Player.X, 79);
	_Player.Y = max(_Player.Y, 0);
	_Player.Y = min(_Player.Y, 23);


	// 콘트롤 키. (미사일 키)
	if (GetAsyncKeyState(VK_CONTROL))
	{
		_Missiles[_MissileCount].X = _Player.X;
		_Missiles[_MissileCount].Y = _Player.Y - 1;
		_Missiles[_MissileCount].Visible = 1;

		_MissileCount++;
	}

	// ESC 키. (종료)
	if (GetAsyncKeyState(VK_ESCAPE) & 0x8001)
	{
		return false;
	}

	return true;
}

void Draw_Player(void)
{
	if (_Player.Visible)
		Sprite_Draw(_Player.X, _Player.Y, '#');

}

void Missile_Move(void)
{
	for (int i = 0; i < _MissileCount; i++)
	{
		if (_Missiles[i].Visible)
		{
			_Missiles[i].Y += -1;
		}

		// 몬스터 충돌 판정 필요

		// 벽 부딪힘
		if (_Missiles[i].Y < 0 || _Missiles[i].Y >= SCREEN_HEIGHT)
		{
			_MissileCount--;
			for (int j = 0; j < _MissileCount; j++)
			{
				_Missiles[j] = _Missiles[j + 1];
			}
		}
	}
}

void Draw_Missile(void)
{
	for (int i = 0; i < _MissileCount; i++)
	{
		if (_Missiles[i].Visible)
		{
			Sprite_Draw(_Missiles[i].X, _Missiles[i].Y, '|');
		}
	}
}

int main(void)
{
	cs_Initial();

	Player_Initial();
	//Map_Set();
	//Monster_Set();

	//--------------------------------------------------------------------
	// 게임의 메인 루프
	// 이 루프가  1번 돌면 1프레임 이다.
	//--------------------------------------------------------------------
	while (1)
	{
		// 키보드 입력
		bool keyResult = KeyProcess();

		// 로직부
		if (keyResult == false)
			return 0;

		Missile_Move();

		// 랜더링
		Buffer_Clear();
		Draw_Player();
		Draw_Missile();
		Buffer_Flip();

		// 프레임 조절
		Sleep(200);
	}
}