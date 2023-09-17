#include <stdio.h>
#include <memory.h>
#include <Windows.h>
#include <stdbool.h>
#include "Console.h"
#include "Main.h"
#include "DataRead.h"
#include "Game.h"

#pragma comment(lib, "winmm.lib")

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

		// 벽 부딪힘
		if (_Missiles[i].Y < 0 || _Missiles[i].Y >= SCREEN_HEIGHT)
		{
			_MissileCount--;
			for (int j = i; j < _MissileCount; j++)
			{
				_Missiles[j] = _Missiles[j + 1];
			}
			i--;
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

void Draw_Monster(void)
{
	for (int i = 0; i < _MonsterCount; i++)
	{
		if (_Monsters[i]->Visible)
		{
			Sprite_Draw(_Monsters[i]->X, _Monsters[i]->Y, 'M');
		}
	}
}

void MovePattern_Set()
{
	char* movePatternInfo = d_Data_Read("MovePattern/MovePatternInfo.data");
	char* movePattern;

	int pos = 0;
	int fileCount = 0;
	char word[100] = "";
	memset(word, 0, 100);
	while (*(movePatternInfo+pos) != '\0')
	{
		if (*(movePatternInfo+pos) == '\n')
		{
			memcpy(word, movePatternInfo, pos);
			fileCount = atoi(word);
			movePatternInfo += pos+1;
			break;
		}
		pos++;
	}

	for (int i = 0; i < fileCount; i++)
	{
		for (;;)
		{
			if (*(movePatternInfo + pos) == '\n')
			{
				char file[100] = "MovePattern/";
				memset(word, 0, 100);
				memcpy(word, movePatternInfo, pos);
				strcat_s(file, sizeof(file), word);

				movePattern = d_Data_Read(file);

				int movePos = 0;
				int move = 1;
				MovePattern movePatternStruct;
				_MovePatterns[i] = &movePatternStruct;
				while(*(movePattern + movePos) != '\0')
				{
					if (*(movePattern + movePos) == ' ')
					{
						memset(word, 0, 100);
						memcpy(word, movePattern, movePos);
						movePattern += movePos + 1;
						movePos = 0;
						_MovePatterns[i]->_dX[move] = atoi(word);
					}
					
					if (*(movePattern + movePos) == '\n')
					{
						memset(word, 0, 100);
						memcpy(word, movePattern, movePos);
						movePattern += movePos + 1;
						movePos = 0;
						_MovePatterns[i]->_dY[move] = atoi(word);
						move++;
					}
					movePos++;
				}
				_MovePatterns[i]->_move = move;

				break;
			}
			pos++;
		}
	}
}

void Monster_Set()
{
	/*
	char* monsterInfo = d_Data_Read("Monster/MonsterInfo.data");
	char* monster;

	int pos = 0;
	int fileCount = 0;
	char word[100] = "";
	memset(word, 0, 100);
	while (*(monsterInfo + pos) != '\0')
	{
		if (*(monsterInfo + pos) == '\n')
		{
			memcpy(word, monster, pos);
			fileCount = atoi(word);
			monsterInfo += pos + 1;
			break;
		}
		pos++;
	}

	for (int i = 0; i < fileCount; i++)
	{
		for (;;)
		{
			if (*(monsterInfo + pos) == '\n')
			{
				char file[100] = "Monster/";
				memset(word, 0, 100);
				memcpy(word, monsterInfo, pos);
				strcat_s(file, sizeof(file), word);

				monster = d_Data_Read(file);

				int monsterPos = 0;
				int move = 1;
				Monster monsterStruct;
				_Monsters[i] = &monsterStruct;
				while (*(monster + monsterPos) != '\0')
				{
					if (*(monster + monsterPos) == '\n')
					{
						memset(word, 0, 100);
						memcpy(word, monster, monsterPos);
						monster += monsterPos + 1;
						monsterPos = 0;
						_MovePatterns[i]->_dY[move] = atoi(word);
						move++;
					}
					movePos++;
				}
				_MovePatterns[i]->_move = move;

				break;
			}
			pos++;
		}
	}*/
}

void Stage_Set()
{
	char* stageInfo = d_Data_Read("StageInfo.data");

}

void Monster_Move()
{
	for (int i = 0; i < _MonsterCount; i++)
	{
		if (_Monsters[i]->Visible)
		{
			/*
			if (_Monsters[i].left > 0)
			{
				_Monsters[i].left--;
				_Monsters[i].X -= 1;
			}
			else if (_Monsters[i].left == 0 && _Monsters[i].right > 0)
			{
				_Monsters[i].right--;
				_Monsters[i].X += 1;
			}
			else if (_Monsters[i].left == 0 && _Monsters[i].right == 0)
			{
				_Monsters[i].left = 4;
				_Monsters[i].right = 4;
			}*/
		}
	}
}

bool Check_Survive()
{
	for (int i = 0; i < _MissileCount; i++)
	{
		for (int j = 0; j < _MonsterCount; j++)
		{
			/*
			if (_Missiles[i].X == _Monsters[j].X && _Missiles[i].Y == _Monsters[j].Y)
			{
				_MissileCount--;
				for (int k = i; k < _MissileCount-1; k++)
				{
					_Missiles[k] = _Missiles[k + 1];
				}
				i--;

				_MonsterCount--;
				_Monsters[j].Visible = 0;
			}*/
		}
	}

	if (_MonsterCount == 0)
		return true;

	return false;
}

int main(void)
{
	timeBeginPeriod(1);

	//Game_Initialize();

	cs_Initial();
	//Map_Set();

	MovePattern_Set();
	Monster_Set();
	Stage_Set();

	for (int stage = 0; stage < _StageCount; stage++)
	{
		//Stage_Set(stage);

		Player_Initial();

		//--------------------------------------------------------------------
		// 게임의 메인 루프
		// 이 루프가  1번 돌면 1프레임 이다.
		//--------------------------------------------------------------------

		unsigned long delay = 0;
		unsigned long beforeTime = timeGetTime();
		int FrameCount = 0;
		DWORD Tick = timeGetTime();
		while (1)
		{
			
			// 키보드 입력
			bool keyResult = Game_KeyProcess();

			// 로직부
			if (keyResult == false)
				return 0;

			Missile_Move();
			Monster_Move();

			bool result = Check_Survive();
			if (result == true)
				break;

			// 랜더링
			Buffer_Clear();
			Draw_Player();
			Draw_Missile();
			Draw_Monster();
			Buffer_Flip();

			FrameCount++;
			if (timeGetTime() - Tick >= 1000)
			{
				printf("%d \n", FrameCount);
				FrameCount = 0;
				Tick = timeGetTime();
			}

			// 프레임 조절
			if (delay < 20)
			{
				Sleep(20 - delay);
				beforeTime += 20;
				delay = timeGetTime() - beforeTime;
			}
		}
	}
}