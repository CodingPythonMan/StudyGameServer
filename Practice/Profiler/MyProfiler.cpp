#include "MyProfiler.h"
#include <stdio.h>

LARGE_INTEGER Start;
LARGE_INTEGER End;
LARGE_INTEGER Freq;

#define MAX_PROFILE 20

ProfileInfo ProfileInfos[MAX_PROFILE];

void ProfileInit()
{
	QueryPerformanceFrequency(&Freq);
}

void ProfileBegin(const WCHAR* szName)
{
	QueryPerformanceCounter(&Start);
}

void ProfileEnd(const WCHAR* szName)
{
	QueryPerformanceCounter(&End);

	double deltaTime = (End.QuadPart - Start.QuadPart) / (double)Freq.QuadPart;
	
	for (int i = 0; i < MAX_PROFILE; i++)
	{
		if (ProfileInfos[i]._flag > 1 && wcscmp(szName, ProfileInfos[i]._name) == 0)
		{

		}
	}
}

void ProfileDataOutText(const WCHAR* szFileName)
{

}

void ProfileReset(void)
{

}