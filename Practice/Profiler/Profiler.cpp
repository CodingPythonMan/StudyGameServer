#include <iostream>
#include "MyProfiler.h"

void SleepFunc()
{
    Sleep(20);
}

int TotalSum(int a)
{
    int total = 0;
    for (int i = 0; i < a; i++)
        total += i;

    return total;
}

int main()
{
    ProfileInit();

    for (int i = 0; i < 10000; i++)
    {
		ProfileBegin(L"TotalSum");
		TotalSum(1000);
		ProfileEnd(L"TotalSum");
    }

    for (int i = 0; i < 5; i++)
    {
		ProfileBegin(L"SleepFunc");
        SleepFunc();
		ProfileEnd(L"SleepFunc");
    }
    
    ProfileDataOutText(L"Profiler.txt");
}