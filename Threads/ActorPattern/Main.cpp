#include "ActorPattern.h"

int main()
{
	HANDLE WorkerThreads[WORKER_THREAD_NUMBER];
	for (int i = 0; i < WORKER_THREAD_NUMBER; i++)
	{
		WorkerThreads[i] = (HANDLE)_beginthreadex(nullptr, 0, Work, nullptr, 0, nullptr);
	}

	// 워커 스레드 종료 대기
	WaitForMultipleObjects(WORKER_THREAD_NUMBER, WorkerThreads, true, INFINITE);
}