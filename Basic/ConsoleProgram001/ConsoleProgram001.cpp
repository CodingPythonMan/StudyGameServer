#include <iostream>
#include <Windows.h>

void Test()
{
	char buffer[4096 * 10];
	buffer[40960 - 1] = 0;
	Test();
}

int main()
{
	Test();
}