#include <iostream>
#include <windows.h>
#include <process.h>
using namespace std;

#pragma comment(lib, "Synchronization.lib")

// 0은 미사용
long lock = 0;
long AddVar = 0;

void Lock(long* lock)
{
	long compare = 1;
	while (1)
	{
		if (InterlockedExchange(lock, 1) == 1)
			WaitOnAddress(lock, &compare, sizeof(long), INFINITE);
		else
			break;
	}
}

void Unlock(long* lock)
{
	*lock = 0;
	WakeByAddressSingle(lock);
}

unsigned int WINAPI AddFunc(LPVOID lpParam)
{
    Lock(&lock);

    for (int i = 0; i < 1000000; i++)
        AddVar++;

    Unlock(&lock);

    return 0;
}

int main()
{
    HANDLE threads[2];
    threads[0] = (HANDLE)_beginthreadex(nullptr, 0, AddFunc, nullptr, 0, nullptr);
    threads[1] = (HANDLE)_beginthreadex(nullptr, 0, AddFunc, nullptr, 0, nullptr);

    WaitForMultipleObjects(2, threads, true, INFINITE);

	printf("AddVar : %d\n", AddVar);

	return 0;
}