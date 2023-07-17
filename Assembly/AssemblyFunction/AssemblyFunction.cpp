// AssemblyFunction.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
//

#include <iostream>

void Test()
{
    int x = 0;
    int* y = nullptr;
    y = &x;
    y += 3;
    *y = 10;
}

int main()
{
    int t = 0;
    std::cout << "Hello World!\n";

    Test();
}
