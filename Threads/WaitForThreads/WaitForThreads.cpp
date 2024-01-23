#include <iostream>
#include <process.h>
#include <windows.h>
using namespace std;

#pragma comment(lib, "winmm.lib")

int gData = 0;
int gConnect = 0;
bool gShutdown = false;

long UpdateThreadCall = 0;

CRITICAL_SECTION cs;
SRWLOCK lock;

unsigned int WINAPI AcceptThread(LPVOID lpParam)
{
	int random;

	while (gShutdown == false)
	{
		random = 10 + (rand() % 991);
		InterlockedIncrement((long*)&gConnect);
		Sleep(random);
	}

	return 0;
}

unsigned int WINAPI DisconnectThread(LPVOID lpParam)
{
	int random;

	while (gShutdown == false)
	{
		random = 10 + (rand() % 991);
		InterlockedDecrement((long*)&gConnect);
		Sleep(random);
	}

	return 0;
}

unsigned int WINAPI UpdateThread(LPVOID lpParam)
{
	while (gShutdown == false)
	{
		//EnterCriticalSection(&cs);
		//AcquireSRWLockShared(&lock);
		AcquireSRWLockExclusive(&lock);
		gData++;
		if (gData % 1000 == 0)
			printf("gData : %d\n", gData);
		ReleaseSRWLockExclusive(&lock);
		//ReleaseSRWLockShared(&lock);
		//LeaveCriticalSection(&cs);

		InterlockedIncrement(&UpdateThreadCall);

		Sleep(10);
	}

	return 0;
}

int main()
{
	timeBeginPeriod(1);

	HANDLE hThreads[5];
	hThreads[0] = (HANDLE)_beginthreadex(nullptr, 0, AcceptThread, 0, 0, nullptr);

	hThreads[1] = (HANDLE)_beginthreadex(nullptr, 0, DisconnectThread, 0, 0, nullptr);

	InitializeCriticalSection(&cs);
	InitializeSRWLock(&lock);
	hThreads[2] = (HANDLE)_beginthreadex(nullptr, 0, UpdateThread, 0, 0, nullptr);
	hThreads[3] = (HANDLE)_beginthreadex(nullptr, 0, UpdateThread, 0, 0, nullptr);
	hThreads[4] = (HANDLE)_beginthreadex(nullptr, 0, UpdateThread, 0, 0, nullptr);

	for(int i=0; i<20; i++)
	{
		printf("gConnect : %d\n", gConnect);
		Sleep(1000);
	}
	gShutdown = true;

	// 스레드 종료 확인을 해야한다. 이걸 어떻게 할지 생각해보자.
	// 메인에서 스레드의 종료 확인은 WaitForMultipleObjects 사용.
	WaitForMultipleObjects(5, hThreads, true, INFINITE);

	DeleteCriticalSection(&cs);

	printf("gData : %d\n", gData);
	printf("UpdateThreadCall : %d\n", UpdateThreadCall);

	return 0;
}