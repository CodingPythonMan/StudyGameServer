#pragma once
#include <windows.h>

class Lock
{
public:
	Lock();
	virtual ~Lock();

private:
	SRWLOCK _lock;
};

#define ExclusiveLock Lock lock;