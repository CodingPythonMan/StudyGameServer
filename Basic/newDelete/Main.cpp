//#include "MyNew.h"
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
	//Player* p = new Player;

	int a = -128;

	const int* p1 = &a;
	int const* p2 = &a;
	int* const p3 = &a;

	p1++;
	p2++;
	p3++;

	//*p = 4;
	*p2 = 5;
	//*p3 = 7;
	//p3++;
}