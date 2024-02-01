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
	HANDLE WorkerThreads[WORKER_THREAD_NUMBER + 1];
	for (int i = 0; i < WORKER_THREAD_NUMBER; i++)
	{
		WorkerThreads[i] = (HANDLE)_beginthreadex(nullptr, 0, Work, nullptr, 0, nullptr);
	}
	WorkerThreads[WORKER_THREAD_NUMBER] = (HANDLE)_beginthreadex(nullptr, 0, Monitor, nullptr, 0, nullptr);

	gEvent = CreateEvent(nullptr, false, false, nullptr);
	if (gEvent == NULL)
		return 0;

	int QuitNum = 0;
	while (1)
	{
		if (_kbhit())
		{
			WCHAR ControlKey = _getwch();

			// Q : 스레드 종료
			if (L'q' == ControlKey || L'Q' == ControlKey)
			{
				wprintf(L"Control Mode : Press Q - Quit Send \n");
				st_MSG_HEAD head;
				head.shType = dfJOB_QUIT;
				head.shPayloadLen = 0;

				AcquireSRWLockExclusive(&lock);

				messageQ.Enqueue((char*)&head, sizeof(st_MSG_HEAD));

				ReleaseSRWLockExclusive(&lock);
				SetEvent(gEvent);
				QuitNum++;

				if (QuitNum >= WORKER_THREAD_NUMBER)
				{
					//MonitorTerminate = true;
					break;
				}

				Sleep(50);
			}
		}

		// 잡 랜덤 생성
		short len = rand() % RANDOM_MAX_LEN;
		st_MSG_HEAD head;
		head.shType = rand() % dfJOB_QUIT;
		head.shPayloadLen = 0;
		wstring ranString;
		if (head.shType == dfJOB_ADD || head.shType == dfJOB_PRINT)
		{
			head.shPayloadLen = len * 2;
			ranString = GenerateRanString(len);
		}
		
		if (messageQ.GetFreeSize() >= head.shPayloadLen + sizeof(st_MSG_HEAD))
		{
			AcquireSRWLockExclusive(&lock);

			messageQ.Enqueue((char*)&head, sizeof(st_MSG_HEAD));

			if (head.shType == dfJOB_ADD || head.shType == dfJOB_PRINT)
			{
				messageQ.Enqueue((char*)&ranString[0], head.shPayloadLen);
			}

			ReleaseSRWLockExclusive(&lock);

			SetEvent(gEvent);
		}
		else
			__debugbreak();

		Sleep(50);
	}

	// 워커 스레드 종료 대기
	WaitForMultipleObjects(WORKER_THREAD_NUMBER+1, WorkerThreads, true, INFINITE);
}