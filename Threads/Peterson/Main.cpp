#include <iostream>
#include "Peterson.h"

bool flag[2] = { false, false };
int turn = 0;

int items = 0;
long lock = 0;

unsigned int WINAPI Thread001(LPVOID lpParam)
{
	bool a;
	int b;

	for (int i = 0; i < 100000000; i++)
	{
		flag[0] = true;
		turn = 0;
		while (1)
		{
			a = flag[1];
			b = turn;
			if (flag[1] != true || turn == 1)
			{
				break;
			}
		}

		long a = InterlockedExchange(&lock, 1);
		if (a == 2)
			printf("Thread1 break => Flag[1] : %d, turn : %d\n", a, b);

		items++;

		a = InterlockedExchange(&lock, 1);
		if (a == 2)
			printf("Thread1 break => Flag[1] : %d, turn : %d\n", a, b);

		lock = 0;
	
		flag[0] = false;
	}

	return 0;
}

unsigned int WINAPI Thread002(LPVOID lpParam)
{
	bool a;
	int b;

	for (int i = 0; i < 100000000; i++)
	{
		flag[1] = true;
		turn = 1;
		while (1)
		{
			a = flag[0];
			b = turn;
			if (flag[0] != true || turn == 0)
			{
				break;
			}
		}

		long a = InterlockedExchange(&lock, 2);
		if (a == 1)
			printf("Thread2 break => Flag[0] : %d, turn : %d\n", a, b);

		items++;

		a = InterlockedExchange(&lock, 2);
		if (a == 1)
			printf("Thread2 break => Flag[0] : %d, turn : %d\n", a, b);

		lock = 0;

		flag[1] = false;
	}

	return 0;
}

int main()
{

	HANDLE threads[2];
	threads[0] = (HANDLE*)_beginthreadex(nullptr, 0, Thread001, nullptr, 0, nullptr);
	threads[1] = (HANDLE*)_beginthreadex(nullptr, 0, Thread002, nullptr, 0, nullptr);
	//threads[0] = (HANDLE*)_beginthreadex(nullptr, 0, Peterson001, nullptr, 0, nullptr);
	//threads[1] = (HANDLE*)_beginthreadex(nullptr, 0, Peterson002, nullptr, 0, nullptr);

	WaitForMultipleObjects(2, threads, true, INFINITE);
	//WaitForSingleObject(threads[1], INFINITE);

	printf("items : %d\n", items);
	//printf("Items : %d\n", Items);
}