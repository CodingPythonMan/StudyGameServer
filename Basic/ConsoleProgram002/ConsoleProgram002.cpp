#include <iostream>

class A
{
public:
	virtual void F1()
	{
		printf("A::F1 함수입니다.\n");
	}
};


class B : public A
{
public:
	void F1()
	{
		printf("B::F1 함수입니다.\n");
	}
};

class C : public B
{
public:
	void F1()
	{
		printf("C::F1 함수입니다.\n");
	}
};

int main()
{
	A* a = new C();
	a->F1();
}