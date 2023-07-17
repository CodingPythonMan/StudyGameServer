// AssemblyFunction.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
//

#include <iostream>

void __stdcall Test(int a, int b)
{
    int x = a;
    int y = b;
    x++;
}

void __cdecl Test2(int a, int b)
{
	int x = a;
	int y = b;
	x++;
}

int main()
{
    Test(10, 20);
    Test2(10, 20);

    std::cout << "Hello World!\n";
}
