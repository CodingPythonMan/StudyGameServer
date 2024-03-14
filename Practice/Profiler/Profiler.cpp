#include <iostream>
#include <process.h>
#include "MyProfiler.h"

Profiler* profiler = Profiler::GetInstance();
CRITICAL_SECTION cs;

int TotalSum(int a)
{
    int total = 0;
    for (int i = 0; i < a; i++)
        total += i;

    return total;
}

void PrintHelloWorld()
{
    printf("Hello World! \n");
}

void BusyWork()
{
    for (int i = 0; i < 10000; i++)
    {
        if (i == 5000)
        {
            printf("BusyWork!...\n");
        }
    }
}

unsigned int WINAPI WorkerThread(LPVOID lpParam)
{
    srand(GetCurrentThreadId());

    for(int i=0; i< 100; i++)
    {
        int Job = i % 3;

        switch (Job)
        {
        case 0:
            profiler->ProfileBegin(L"TotalSum");
            TotalSum(1000);
            profiler->ProfileEnd(L"TotalSum");
            break;
        case 1:
            profiler->ProfileBegin(L"PrintHelloWorld");
            PrintHelloWorld();
            profiler->ProfileEnd(L"PrintHelloWorld");
            break;
        case 2:
            profiler->ProfileBegin(L"BusyWork");
            BusyWork();
            profiler->ProfileEnd(L"BusyWork");
            break;
        }

        Sleep(100);
    }

    EnterCriticalSection(&cs);
    profiler->ProfileDataOutText(L"WorkerThread.txt");
    LeaveCriticalSection(&cs);

    return 0;
}

unsigned int WINAPI AcceptThread(LPVOID lpParam)
{
	srand(GetCurrentThreadId());

	for(int i=0; i< 30; i++)
	{
		int Job = rand() % 3;

		switch (Job)
		{
		case 0:
            profiler->ProfileBegin(L"TotalSum");
			TotalSum(1000);
            profiler->ProfileEnd(L"TotalSum");
			break;
		case 1:
            profiler->ProfileBegin(L"PrintHelloWorld");
			PrintHelloWorld();
            profiler->ProfileEnd(L"PrintHelloWorld");
			break;
		case 2:
            profiler->ProfileBegin(L"BusyWork");
			BusyWork();
            profiler->ProfileEnd(L"BusyWork");
			break;
		}

        Sleep(500);
	}

    profiler->ProfileDataOutText(L"AcceptThread.txt");

    return 0;
}

int main()
{
    InitializeCriticalSection(&cs);

    HANDLE Threads[6];
    for (int i = 0; i < 5; i++)
    {
        Threads[i] = (HANDLE)_beginthreadex(nullptr, 0, WorkerThread, nullptr, 0, nullptr);
    }
    Threads[5] = (HANDLE)_beginthreadex(nullptr, 0, AcceptThread, nullptr, 0, nullptr);

    WaitForMultipleObjects(6, Threads, true, INFINITE);

    DeleteCriticalSection(&cs);
}