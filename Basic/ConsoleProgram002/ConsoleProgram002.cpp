#include <iostream>

class A
{
public:
	A()
	{
		printf("A 생성\n");
	}

	~A()
	{
		printf("A 소멸\n");
	}

	virtual void F1()
	{
		printf("A::F1 함수입니다.\n");
		delete this;
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

A g_A;

int main()
{
	A* a = new A();
	a->F1();
}