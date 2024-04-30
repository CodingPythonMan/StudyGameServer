#include "FastSpinLock.h"

void FastSpinLock::Lock()
{
	while (1)
	{
		long a = InterlockedExchange(&lock, 1);
		if (a == 0)
			break;
	}
}

void FastSpinLock::Unlock()
{
	lock = 0;
}