#include <iostream>
#include "MyProfiler.h"

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


    ProfileBegin(L"TotalSum");
    printf("total : %d\n", TotalSum(1000));
    ProfileEnd(L"TotalSum");
}