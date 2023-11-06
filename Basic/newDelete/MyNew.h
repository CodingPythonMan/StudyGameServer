#pragma once
#include <cstdlib>
#include <cstdio>
#include <string>
#include <ctime>

#define MAX_ALLOC_INFO 100

enum class MyNewError {
	NOALLOC,
	ARRAY,
	LEAK
};

class MyNew {
	struct AllocInfo
	{
		void* Ptr;
		int Size;
		char FileName[64];
		int Line;
		bool Array;
	};
	
public:
	MyNew();
	~MyNew();

	void AddInfo(void* Ptr, int Size, char FileName[64], int Line, bool Array);
	void AddLog(MyNewError error, void* ptr, int index = 0);
	void ResetInfoIndex(int index);

	int totalAlloc;
	AllocInfo allocInfos[MAX_ALLOC_INFO];

	int totalLog;
	char logs[100][100];
};

// 위 방식의 new 호출 방법
void* operator new(size_t size, const char* File = __FILE__, int Line = __LINE__);

void* operator new[](size_t size, const char* File = __FILE__, int Line = __LINE__);

void operator delete (void* p, const char* File, int Line);

void operator delete[](void* p, const char* File, int Line);

//실제로 사용할 delete
void operator delete(void* p);

void operator delete[](void* p);

#define new new(__FILE__, __LINE__)