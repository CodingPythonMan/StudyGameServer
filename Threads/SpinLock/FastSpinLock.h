#pragma once
#include <windows.h>

class FastSpinLock {
public:
	void Lock();
	void Unlock();

private:
	volatile long lock = 0;

};