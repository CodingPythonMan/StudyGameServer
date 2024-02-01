#include "Lock.h"

Lock::Lock()
{
	AcquireSRWLockExclusive(&_lock);
}

Lock::~Lock()
{
	ReleaseSRWLockExclusive(&_lock);
}