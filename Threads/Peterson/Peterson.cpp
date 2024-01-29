#include "Peterson.h"

bool Flag[2] = { false, false };
int Turn = 0;

int Items = 0;

long c = 0;

unsigned int WINAPI Peterson001(LPVOID lpParam)
{
	for (int i = 0; i < 100000000; i++)
	{
		Flag[0] = true;
		Turn = 0;
		while (1)
		{
			//InterlockedExchange(&c, 1);
			if (Flag[1] != true || Turn == 1)
			{
				break;
			}
		}

		Items++;

		Flag[0] = false;
	}

	return 0;
}

unsigned int WINAPI Peterson002(LPVOID lpParam)
{
	for (int i = 0; i < 100000000; i++)
	{
		Flag[1] = true;
		Turn = 1;
		while (1)
		{
			//InterlockedExchange(&c, 1);
			if (Flag[0] != true || Turn == 0)
			{
				break;
			}
		}

		Items++;

		Flag[1] = false;
	}

	return 0;
}