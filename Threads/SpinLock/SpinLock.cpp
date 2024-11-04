#include <iostream>
#include <process.h>
#include <windows.h>
using namespace std;

#pragma comment(lib, "winmm.lib")

#include "FastSpinLock.h"

int SharedObject = 0;
long lock = 0;
FastSpinLock lockObj;

LARGE_INTEGER Freq;

unsigned int WINAPI Thread_Basic(LPVOID lpParam)
{
	for (int i = 0; i < 500000; i++)
	{
		while (1)
		{
			long a = InterlockedExchange(&lock, 1);
			if (a == 0)
			{
				break;
			}
		}

		for (int i = 0; i < 10; i++)
		{
			SharedObject++;
			SharedObject--;
			SharedObject++;
		}

		lock = 0;
	}

	return 0;
}

unsigned int WINAPI Thread_Fast(LPVOID lpParam)
{
	for (int i = 0; i < 500000; i++)
	{
		lockObj.Lock();

		for (int i = 0; i < 10; i++)
		{
			SharedObject++;
			SharedObject--;
			SharedObject++;
		}

		lockObj.Unlock();
	}

	return 0;
}

void UseNormalSpinLock()
{
	HANDLE hThreads[3];

	LARGE_INTEGER Start, End;
	QueryPerformanceCounter(&Start);
	for (int i = 0; i < 3; i++)
	{
		hThreads[i] = (HANDLE)_beginthreadex(nullptr, 0, Thread_Basic, 0, 0, nullptr);
	}

	WaitForMultipleObjects(3, hThreads, true, INFINITE);
	QueryPerformanceCounter(&End);

	double normal = (double)(End.QuadPart - Start.QuadPart) / (double)(Freq.QuadPart);

	cout << "SharedObject 값 : " << SharedObject << "\n";
	cout << "[Normal Spinlock] 걸린 시간 값 : " << normal << "\n";
}

void UseFastSpinLock()
{
	HANDLE hThreads[3];

	LARGE_INTEGER Start, End;
	QueryPerformanceCounter(&Start);

	for (int i = 0; i < 3; i++)
	{
		hThreads[i] = (HANDLE)_beginthreadex(nullptr, 0, Thread_Fast, 0, 0, nullptr);
	}

	WaitForMultipleObjects(3, hThreads, true, INFINITE);
	QueryPerformanceCounter(&End);

	double fast = (double)(End.QuadPart - Start.QuadPart) / (double)(Freq.QuadPart);

	cout << "SharedObject 값 : " << SharedObject << "\n";
	cout << "[Fast Spinlock] 걸린 시간 값 : " << fast << "\n";
}

int main()
{
	timeBeginPeriod(1);

	QueryPerformanceFrequency(&Freq);

	UseNormalSpinLock();
	UseFastSpinLock();
}