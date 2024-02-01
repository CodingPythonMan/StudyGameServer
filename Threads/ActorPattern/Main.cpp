#include "ActorPattern.h"
#include <conio.h>

#define RANDOM_MAX_LEN 30

wstring GenerateRanString(short len)
{
	wstring tempData;
	tempData.reserve(len);
	
	for (int i = 0; i < len; i++)
		tempData.push_back('a' + rand() % 26);

	return tempData;
}

int main()
{
	HANDLE WorkerThreads[WORKER_THREAD_NUMBER];
	for (int i = 0; i < WORKER_THREAD_NUMBER; i++)
	{
		WorkerThreads[i] = (HANDLE)_beginthreadex(nullptr, 0, Work, nullptr, 0, nullptr);
	}

	while (1)
	{
		// L : 컨트롤 Lock / U : 컨트롤 Unlock / G : 서버 종료
		if (_kbhit())
		{
			WCHAR ControlKey = _getwch();

			// 키보드 제어 풀림 상태에서 특정 기능
			if (L'q' == ControlKey || L'Q' == ControlKey)
			{
				wprintf(L"Control Mode : Press Q - Quit Send \n");
			}
		}

		// 잡 랜덤 생성
		short len = rand() % RANDOM_MAX_LEN;
		st_MSG_HEAD head;
		head.shType = rand() % dfJOB_QUIT;
		head.shPayloadLen = len * 2;
		wstring ranString = GenerateRanString(len);

		if (messageQ.GetFreeSize() >= head.shPayloadLen + sizeof(st_MSG_HEAD))
		{
			AcquireSRWLockExclusive(&lock);

			messageQ.Enqueue((char*)&head, sizeof(st_MSG_HEAD));
			messageQ.Enqueue((char*)&ranString[0], head.shPayloadLen);

			ReleaseSRWLockExclusive(&lock);
		}
		else
			__debugbreak();

		Sleep(50);
	}

	// 워커 스레드 종료 대기
	WaitForMultipleObjects(WORKER_THREAD_NUMBER, WorkerThreads, true, INFINITE);
}