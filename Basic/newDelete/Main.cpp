#include "MyNew.h"

struct Player {
	Player() : _x(0), _y(0)
	{
		
	}

	~Player()
	{
		
	}

	int _x;
	int _y;
};

void TestCode()
{
	Player* p = new Player;
}

int main()
{
	for (int i = 0; i < 101; i++)
		TestCode();
}