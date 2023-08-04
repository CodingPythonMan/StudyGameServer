#include <iostream>
#include <Windows.h>

int main()
{
    int* p = (int*)malloc(1000000);

    Sleep(1000);
    for (int i = 0; i < 1000000; i++)
    {
        Sleep(1);
        p[i] = 0;
    }
}
 