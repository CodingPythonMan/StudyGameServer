#include "MutexService.h"

MutexService::MutexService()
{
}

MutexService::~MutexService()
{
}

void MutexService::Start()
{
}

unsigned int __stdcall MutexService::MyThread(LPVOID param)
{
	MutexService* service = (MutexService*)param;

	for (int i = 0; i < 1000000; i++)
	{
		service->m_lock.lock();

		printf("안녕? 여기는 다음 번엔 올 수 없어.\n");
	}
	
	return 0;
}
