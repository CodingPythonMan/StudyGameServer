#include <iostream>
#include <process.h>
#include <windows.h>
using namespace std;

#include "FastSpinLock.h"

int SharedObject = 0;
long lock = 0;
FastSpinLock lockObj;

unsigned int WINAPI Thread_Basic(LPVOID lpParam)
{
	while (1)
	{
		long a = InterlockedExchange(&lock, 1);
		if(a == 0)
			break;
	}

	for(int i=0; i<500000; i++)
		SharedObject++;

	lock = 0;

	return 0;
}

unsigned int WINAPI Thread_Fast(LPVOID lpParam)
{
	lockObj.Lock();

	for (int i = 0; i < 500000; i++)
		SharedObject++;

	lockObj.Unlock();

	return 0;
}

void UseNormalSpinLock()
{
	HANDLE hThreads[3];
	for (int i = 0; i < 3; i++)
		hThreads[i] = (HANDLE)_beginthreadex(nullptr, 0, Thread_Basic, 0, 0, nullptr);

	WaitForMultipleObjects(3, hThreads, true, INFINITE);

	cout << "SharedObject °ª : " << SharedObject << "\n";
}

void UseFastSpinLock()
{
	HANDLE hThreads[3];

	for(int i=0; i<3; i++)
		hThreads[i] = (HANDLE)_beginthreadex(nullptr, 0, Thread_Fast, 0, 0, nullptr);

	WaitForMultipleObjects(3, hThreads, true, INFINITE);

	cout << "SharedObject °ª : " << SharedObject << "\n";
}

int main()
{
	//UseNormalSpinLock();
	UseFastSpinLock();
}