#include "CacheSimulator.h"

int main()
{
	int a = 0;
	CashSimulate(&a);
	int b = 0;

	a++;
	CashSimulate(&a);
	CashSimulate(&b);
	b++;

	a = b + 1;
	CashSimulate(&a);
}