#include "CacheSimulator.h"

int main()
{
	int* pointer;

	int a = 0;
	pointer = &a;
	CashSimulate(pointer);
	int b = 0;

	a++;
	pointer = &a;
	CashSimulate(pointer);
	b++;

	a = b + 1;
	pointer = &a;
	CashSimulate(pointer);
}