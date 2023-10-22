#include "stdafx.h"

void printString(char* p)
{
	p[0] = ' ';
	printf(p);
}

int main()
{
	int a = 5;
	char string[10];
	char sssss[] = "22222";

	printString(sssss);
}