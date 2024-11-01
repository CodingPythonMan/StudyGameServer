#include "CriticalService.h"
#include <iostream>

CriticalService::CriticalService()
{
	InitializeCriticalSection(&m_lock);
}

CriticalService::~CriticalService()
{
	DeleteCriticalSection(&m_lock);
}

void CriticalService::Start()
{
	(HANDLE)_beginthreadex(nullptr, 0, MyThread, this, 0, nullptr);
}

unsigned int __stdcall CriticalService::MyThread(LPVOID param)
{
	CriticalService* service = (CriticalService*)param;

	for (int i = 0; i < 1000000; i++)
	{
		EnterCriticalSection(&(service->m_lock));

		printf("안녕? 여기는 다음 번엔 올 수 없어.\n");
	}

	return 0;
}
