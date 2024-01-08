#include <iostream>
using namespace std;

struct alignas(32) DATA1 {
	char a;
	short b;
	int c;
	long d;
};

struct DATA2 {
	char a;
	short b;
	alignas(32) int c;
	long d;
};

int Test(int a, int b)
{
	return 8;
}

int main()
{
	cout << "DATA1 sizeof : " << sizeof(DATA1) << "\n";
	cout << "DATA2 sizeof : " << sizeof(DATA2) << "\n";

	//int test = Test(10, 20);
}