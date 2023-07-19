// AssemblyFunction.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
//

#include <iostream>
#include <Windows.h>

void Test()
{
    int x = 0;
    int* p = &x;
    *p = 0;
    *(p + 2) = 0;
}

void a()
{
    int debug = 0xaabbccddee;
    int x = 0;
    int y = 0;

    Test();
    if (debug != 0xaabbccddee)
    {
        DebugBreak();
        int x = 0;
    }

    x++;
    y++;
}

int main()
{
    int t = 0;
    a();
}