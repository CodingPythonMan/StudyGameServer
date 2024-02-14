#include <process.h>
#include <Windows.h>
#include <iostream>
using namespace std;

CRITICAL_SECTION cs;

void LockPlayer()
{
    EnterCriticalSection(&cs);
}

unsigned int OverlapCriticalSection001(LPVOID lpParam)
{
    for (int i = 0; i < 3; i++)
    {
        LockPlayer();
    }

    LeaveCriticalSection(&cs);
    LeaveCriticalSection(&cs);
    LeaveCriticalSection(&cs);
    //LeaveCriticalSection(&cs); // Leave 자체는 여러번해도 된다.

    return 0;
}

unsigned int OverlapCriticalSection002(LPVOID lpParam)
{
    Sleep(1000);
    LockPlayer();

	return 0;
}

int main()
{
    HANDLE threads[2];

    InitializeCriticalSection(&cs);

    threads[0] = (HANDLE)_beginthreadex(nullptr, 0, OverlapCriticalSection001, nullptr, 0, nullptr);
    threads[1] = (HANDLE)_beginthreadex(nullptr, 0, OverlapCriticalSection002, nullptr, 0, nullptr);

    WaitForMultipleObjects(2, threads, true, INFINITE);

    DeleteCriticalSection(&cs);
}