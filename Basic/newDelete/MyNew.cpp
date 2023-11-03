#include "MyNew.h"

#define new MyNew::operator new(__FILE__, __LINE__)
MyNew myNew;

MyNew::MyNew()
{
	printf("생성!\n");
}

MyNew::~MyNew()
{
	printf("종료!\n");
}
