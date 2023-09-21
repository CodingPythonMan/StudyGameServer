#include <iostream>

class Base
{
public:
	Base() { F2(); }
	void F2() { F1(); }
	virtual void F1() = 0;
};

class Me : public Base 
{
public:
	void F1()
	{
		printf("Me::F1 호출");
	}
};

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

	void F2()
	{
		printf("A::F2 함수입니다.\n");
	}
};


class B : public A
{
public:
	void F1()
	{
		printf("B::F1 함수입니다.\n");
	}
	
	void F2()
	{
		printf("아하");
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
	//A* a = new B();
	//a->F1();

	//Me m;

	// nullptr 을 찌르고 있어도 멤버를 안쓰는 멤버 함수는 호출이 된다.
	A* a = nullptr;
	a->F2();
}