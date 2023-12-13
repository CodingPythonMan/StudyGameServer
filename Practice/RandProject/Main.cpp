#include "Rand.h"

void Test1()
{
	while (1)
	{
		_getch();
		Gatcha();
	}
}

int main()
{
	for(int i=0; i<10000; i++)
		cout << MakeIntRand() << "\n";
}