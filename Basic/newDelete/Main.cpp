#include "MyNew.h"
#include <stdio.h>

struct Player {
	Player() : _x(0), _y(0)
	{

	}

	~Player()
	{
		printf("소멸자 호출");
	}

	int _x;
	int _y;
};

int main()
{
	Player* p = new Player;
	
	delete p;
}