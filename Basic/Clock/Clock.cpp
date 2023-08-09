#include <iostream>
#include <Windows.h>

#pragma comment(lib, "winmm.lib")

DWORD attackTick;

void Attack()
{
    if (GetTickCount() - attackTick < 300)
        return;

    attackTick = GetTickCount();
}

int main()
{
    timeBeginPeriod(1);

    GetTickCount();

    while (1)
    {
        DWORD t1 = timeGetTime();
        DWORD t2 = GetTickCount();

        printf("%d / %d \n", t1, t2);
    }
    clock();
}