#include "MyNew.h"

struct Player {
	Player() : _x(0), _y(0)
	{
		printf("생성자 호출\n");
	}

	~Player()
	{
		printf("소멸자 호출\n");
	}

	int _x;
	int _y;
};

int main()
{
	Player* p = new Player;
	
	delete p;
}