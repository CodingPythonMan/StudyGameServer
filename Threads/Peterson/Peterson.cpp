#include "Peterson.h"

bool Flag[2] = { false, false };
int Turn = 0;

int Items = 0;

unsigned int WINAPI Peterson001(LPVOID lpParam)
{
	for (int i = 0; i < 100000000; i++)
	{
		bool AnotherFlag = Flag[1];
		Flag[0] = true;
		Turn = 0;
		while (1)
		{
			if (AnotherFlag != true || Turn == 1)
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
		bool AnotherFlag = Flag[0];
		Flag[1] = true;
		Turn = 1;
		while (1)
		{
			AnotherFlag = Flag[0];
			if (AnotherFlag != true || Turn == 0)
			{
				break;
			}
		}

		Items++;

		Flag[1] = false;
	}

	return 0;
}