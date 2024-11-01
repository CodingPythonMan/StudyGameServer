#include "FastSpinLock.h"

void FastSpinLock::Lock()
{
	do
	{
		Sleep(0);
		Sleep(0);

		YieldProcessor();
	} 
	while (InterlockedCompareExchange(&lock, 1, 0) != 0);
}

void FastSpinLock::Unlock()
{
	lock = 0;
}