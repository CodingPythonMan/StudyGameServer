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
			// spin Ƚ���� �������� cpu �� ��� �纸
			Sleep(1);
		}
	} 	
}

void FastSpinLock::Unlock()
{
	lock = 0;
}