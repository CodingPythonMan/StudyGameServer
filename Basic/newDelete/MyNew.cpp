#include "MyNew.h"
#undef new

MyNew myNew;

MyNew::MyNew()
{
	for (int i = 0; i < MAX_ALLOC_INFO; i++)
	{
		memset(allocInfos, 0, sizeof(AllocInfo) * MAX_ALLOC_INFO);
	}
	for (int i = 0; i < 100; i++)
	{
		memset(logs + i, 0, 100);
	}
	totalAlloc = 0;
	totalLog = 0;
}

MyNew::~MyNew()
{
	
}

void MyNew::AddInfo(void* Ptr, int Size, char FileName[64], int Line, bool Array)
{
	allocInfos[totalAlloc].Ptr = Ptr;
	allocInfos[totalAlloc].Size = Size;
	memcpy(allocInfos[totalAlloc].FileName, FileName, strlen(FileName));
	allocInfos[totalAlloc].Line = Line;
	allocInfos[totalAlloc].Array = Array;

	totalAlloc++;
}

void MyNew::AddLog(MyNewError error, void* ptr, const char* file, int line)
{
	switch (error)
	{
	case MyNewError::NOALLOC:
		strcpy_s(logs[totalLog], "[NOALLOC [0x\0");
		int len = strlen(logs[totalLog]);
		

		
		break;
	case MyNewError::ARRAY:


		break;
	case MyNewError::LEAK:

		break;
	}
}

// 위 방식의 new 호출 방법
void* operator new(size_t size, const char* File, int Line)
{
	// 1. 메모리 할당
	void* ptr = malloc(size);

	myNew.AddInfo(ptr, size, (char*)File, Line, false);

	return ptr;
}

void* operator new[](size_t size, const char* File, int Line)
{
	void* ptr = malloc(size);

	myNew.AddInfo(ptr, size, (char*)File, Line, true);

	return ptr;
}

void operator delete (void* p, const char* File, int Line)
{
}
void operator delete[](void* p, const char* File, int Line)
{
}

//실제로 사용할 delete
void operator delete(void* p)
{
	for (int i = 0; i < myNew.totalAlloc; i++)
	{
		// 할당된 곳에 존재
		if (myNew.allocInfos[i].Ptr == p && myNew.allocInfos[i].Array == false)
		{
			myNew.allocInfos[i].Ptr = nullptr;
			free(p);
			return;
		}
		else if (myNew.allocInfos[i].Ptr == p && myNew.allocInfos[i].Array == true)
		{
			myNew.AddLog(MyNewError::ARRAY, p);
		}
	}


}

void operator delete[](void* p)
{

}