#include "FastSpinLock.h"

void FastSpinLock::Lock()
{
	int spinCount = 0;

	while (InterlockedCompareExchange(&lock, 1, 0) != 0)
	{
		if (spinCount < 100)
		{
			YieldProcessor();
			spinCount++;
		}
		else
		{
			// spin 횟수가 많아지면 cpu 를 잠시 양보
			Sleep(1);
		}
	} 	
}

void FastSpinLock::Unlock()
{
	lock = 0;
}