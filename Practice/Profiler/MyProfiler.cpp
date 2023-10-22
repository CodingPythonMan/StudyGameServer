#include "MyProfiler.h"

void ProfileBegin(WCHAR* szName)
{
	QueryPerformanceCounter(&Start);

	QueryPerformanceCounter(&End);
}

void ProfileEnd(WCHAR* szName)
{
}

void ProfileDataOutText(WCHAR* szFileName)
{
}

void ProfileReset(void)
{
}
