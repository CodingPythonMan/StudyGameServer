#pragma once
#include <stdlib.h>
#include <stdio.h>

struct AllocInfo
{
	void* Ptr;
	int Size;
	char FileName[64];
	int Line;
	bool Array;
};

class MyNew {
private:


public:
	MyNew();
	virtual ~MyNew();
};

// 위 방식의 new 호출 방법
void* operator new(size_t size, const char* File = __FILE__, int Line = __LINE__);

void* operator new[](size_t size, char* File, int Line);

void operator delete (void* p, const char* File, int Line);

void operator delete[](void* p, const char* File, int Line);

//실제로 사용할 delete
void operator delete(void* p);

void operator delete[](void* p);

#define new new(__FILE__, __LINE__)