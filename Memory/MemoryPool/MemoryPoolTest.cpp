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
	printf("첫번째 Alloc 주소 : 0x%p\n", p1);

	p1->Move(3, 5);
	p1->MakeName("HHHaaa");

	Player* p3 = PlayerPool.Alloc();
	printf("두번째 Alloc 주소 : 0x%p\n", p3);
	PlayerPool.Free(p3);
	printf("두번째 Free 주소 : 0x%p\n", p3);
	p3 = PlayerPool.Alloc();
	printf("두번째 Alloc 주소 : 0x%p\n", p3);
	PlayerPool.Free(p3);
	printf("두번째 Free 주소 : 0x%p\n", p3);
	p3 = PlayerPool.Alloc();
	printf("두번째 Alloc 주소 : 0x%p\n", p3);
	PlayerPool.Free(p3);
	printf("두번째 Free 주소 : 0x%p\n", p3);
	p3 = PlayerPool.Alloc();
	printf("두번째 Alloc 주소 : 0x%p\n", p3);
	p3->Move(3, 5);
	p3->MakeName("HHHaaa");
	PlayerPool.Free(p3);
	printf("두번째 Free 주소 : 0x%p\n", p3);
	p3 = PlayerPool.Alloc();
	printf("두번째 Alloc 주소 : 0x%p\n", p3);
	PlayerPool.Free(p3);
	printf("두번째 Free 주소 : 0x%p\n", p3);
	p3 = PlayerPool.Alloc();
	printf("두번째 Alloc 주소 : 0x%p\n", p3);
	PlayerPool.Free(p3);
	printf("두번째 Free 주소 : 0x%p\n", p3);

	Player* p2 = PlayerPool.Alloc();
	printf("두번째 Alloc 주소 : 0x%p\n", p2);
	Player* p4 = PlayerPool.Alloc();
	printf("세번째 Alloc 주소 : 0x%p\n", p4);
	Player* p5 = PlayerPool.Alloc();
	printf("네번째 Alloc 주소 : 0x%p\n", p5);
	Player* p6 = PlayerPool.Alloc();
	printf("다섯번째 Alloc 주소 : 0x%p\n", p6);
	p2->Move(3, 5);
	p2->MakeName("HHHaaa");
	p1->Move(3, 5);
	p1->MakeName("HHHaaa");

	p5->Move(3, 5);
	p6->MakeName("HHHaaa");
	p4->MakeName("HHHaaa");

	PlayerPool.Free(p2);
	printf("두번째 Free 주소 : 0x%p\n", p2);
	PlayerPool.Free(p1);
	printf("첫번째 Free 주소 : 0x%p\n", p1);
	PlayerPool.Free(p4);
	printf("세번째 Free 주소 : 0x%p\n", p4);
	PlayerPool.Free(p5);
	printf("네번째 Free 주소 : 0x%p\n", p5);
	PlayerPool.Free(p6);
	printf("다섯번째 Free 주소 : 0x%p\n", p6);
}