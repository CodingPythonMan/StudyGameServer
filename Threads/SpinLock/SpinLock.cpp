#include <iostream>
#include <process.h>
#include <windows.h>
using namespace std;

int SharedObject = 0;
long lock = 0;

unsigned int WINAPI Spinlock01(LPVOID lpParam)
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

unsigned int WINAPI Spinlock02(LPVOID lpParam)
{
	while (1)
	{
		long a = InterlockedExchange(&lock, 1);
		if (a == 0)
			break;
	}

	for (int i = 0; i < 500000; i++)
		SharedObject++;

	lock = 0;

	return 0;
}

int main()
{
	HANDLE hThreads[2];
	hThreads[0] = (HANDLE)_beginthreadex(nullptr, 0, Spinlock01, 0, 0, nullptr);
	hThreads[1] = (HANDLE)_beginthreadex(nullptr, 0, Spinlock02, 0, 0, nullptr);

	WaitForMultipleObjects(2, hThreads, true, INFINITE);

	cout << "SharedObject °ª : " << SharedObject << "\n";
}