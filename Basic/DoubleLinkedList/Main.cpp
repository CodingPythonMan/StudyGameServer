#include <iostream>
#include "MyList.h"

class Player
{
	int x;
	int y;

public:
	void PlayGame()
	{
		x = 4;
		y = 3;
		printf("%d %d", x, y);
	}
};

int main()
{
	MyList<int> ListInt;
	ListInt.push_front(3);
	ListInt.push_back(4);
	ListInt.push_back(5);
	ListInt.push_front(6);

	ListInt.pop_front();
	ListInt.pop_front();
	ListInt.pop_front();
	/*
	ListInt.pop_front();

	ListInt.push_front(3);
	ListInt.push_back(4);
	ListInt.push_back(5);
	ListInt.push_front(6);

	ListInt.clear();*/

	ListInt.remove(5);

	// 鉴雀 基敲 内靛
	MyList<int>::iterator iter;
	for (iter = ListInt.begin(); iter != ListInt.end(); ++iter)
	{
		printf("%d\n", *iter);
	}

	// 按眉 昏力 基敲 内靛
	MyList<Player*> ListPlayer;
	ListPlayer.push_back(new Player);
	ListPlayer.push_back(new Player);
	
	for (MyList<Player*>::iterator iter = ListPlayer.begin(); iter != ListPlayer.end(); ++iter)
	{
		Player* p = *iter;
		//p->PlayGame();
	}
}