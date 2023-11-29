#include <iostream>
#include "MemoryPool.h"
#include "Player.h"
#include <Windows.h>

#pragma comment(lib, "winmm.lib")

MemoryPool<Player> PlayerPool(0, true);

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

int main()
{
	// 이 세가지 경우가 몇번 함수로 호출될 수 있는지 테스트 해보자.
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