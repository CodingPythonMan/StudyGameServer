#include <iostream>
#include <Windows.h>

struct data {
	int a : 4;
	int b : 5;
	int c : 1;
};

int main()
{
	data d;
	d.a = 4;
	d.b = 2;
	d.c = 1;
}