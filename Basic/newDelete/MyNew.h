#pragma once
#include <stdlib.h>
#include <stdio.h>

void* operator new(size_t size, const char* File = __FILE__, int Line = __LINE__)
{
	//void* ptr = malloc(size);

	void* ptr = nullptr;

	printf("ahaha\n");
	printf("File: %s Line: %d\n", File, Line);

	return ptr;
}

void* operator new[](size_t size, char* File, int Line)
{
	void* ptr = malloc(size);

	return ptr;
}

/*
void operator delete(void* p, char* File, int Line)
{

}

void operator delete[](void* p, char* File, int Line)
{

}*/

//실제로 사용할 delete
void operator delete(void* p)
{

}

void operator delete[](void* p)
{

}

// 위 방식의 new 호출 방법


#define new new(__FILE__, __LINE__)