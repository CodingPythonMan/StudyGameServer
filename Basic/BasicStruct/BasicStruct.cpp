#include <iostream>
#include <Windows.h>

struct stData
{
	char a;
	alignas(32) __int64 b;
	int c;
	__int64 d;
	char e;
	__int64 f;
};

void Test()
{
	stData a;
	a.a = 0;
}

int main()
{
	Test();
}