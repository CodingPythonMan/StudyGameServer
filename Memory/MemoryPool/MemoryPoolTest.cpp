#include "MemoryPoolTest.h"

MemoryPool<Player> PlayerPool(3, true);

void Test1()
{
	Player* p1 = PlayerPool.Alloc();
	PlayerPool.Free(p1);

	Player* p2 = PlayerPool.Alloc();
	Player* p3 = PlayerPool.Alloc();
	Player* p4 = PlayerPool.Alloc();
	PlayerPool.Free(p3);
	PlayerPool.Free(p4);
	PlayerPool.Free(p2);
}

void Test2()
{
	Player* p1 = new Player;
	delete p1;

	Player* p2 = new Player;
	Player* p3 = new Player;
	Player* p4 = new Player;
	delete p3;
	delete p4;
	delete p2;
}

void Performance()
{
	timeBeginPeriod(1);
	unsigned int currentTime = timeGetTime();
	int CallCount = 0;
	while (timeGetTime() - currentTime < 1000)
	{
		Test1();
		CallCount++;
	}
	printf("[MemoryPool Test] : %d\n", CallCount);

	currentTime = timeGetTime();
	CallCount = 0;
	while (timeGetTime() - currentTime < 1000)
	{
		Test2();
		CallCount++;
	}
	printf("[Heap Test] : %d\n", CallCount);
}

void BasicTest()
{
	printf("Player Size : %d\n", (int)sizeof(Player));

	Player* p1 = PlayerPool.Alloc();
	p1->Move(3, 5);
	p1->MakeName("HHHaaa");

	Player* p3 = PlayerPool.Alloc();
	PlayerPool.Free(p3);
	p3 = PlayerPool.Alloc();
	PlayerPool.Free(p3);
	p3 = PlayerPool.Alloc();
	PlayerPool.Free(p3);
	p3 = PlayerPool.Alloc();
	p3->Move(3, 5);
	p3->MakeName("HHHaaa");
	PlayerPool.Free(p3);
	p3 = PlayerPool.Alloc();
	PlayerPool.Free(p3);
	p3 = PlayerPool.Alloc();
	PlayerPool.Free(p3);

	Player* p2 = PlayerPool.Alloc();
	Player* p4 = PlayerPool.Alloc();
	Player* p5 = PlayerPool.Alloc();
	Player* p6 = PlayerPool.Alloc();
	p2->Move(3, 5);
	p2->MakeName("HHHaaa");
	p1->Move(3, 5);
	p1->MakeName("HHHaaa");

	p5->Move(3, 5);
	p6->MakeName("HHHaaa");
	p4->MakeName("HHHaaa");

	PlayerPool.Free(p2);
	PlayerPool.Free(p1);
	PlayerPool.Free(p4);
	PlayerPool.Free(p5);
	PlayerPool.Free(p6);
}