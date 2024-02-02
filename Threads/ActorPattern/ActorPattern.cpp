#include "ActorPattern.h"
#include <algorithm>
#include <iostream>

list<wstring>			gList;
RingBuffer				messageQ;

SRWLOCK					lock;
HANDLE					gEvent;
bool					MonitorTerminate;
map<int, int>	ThreadMap;

long Job[dfJOB_QUIT + 1] = {0};

unsigned int WINAPI Work(LPVOID lpParam)
{
	// Enqueue 할 때까지 기다린다.
	bool Terminate = false;
	int ThreadID = GetCurrentThreadId();
	ThreadMap.insert({ ThreadID, 0 });

	while (Terminate == false)
	{
		WaitForSingleObject(gEvent, INFINITE);

		ThreadMap[ThreadID]++;

		// Dequeue 진행
		ExclusiveLock;
		do
		{
			if (messageQ.GetUseSize() < sizeof(st_MSG_HEAD))
				break;

			st_MSG_HEAD head;
			messageQ.Peek((char*)&head, sizeof(st_MSG_HEAD));

			short size = head.shPayloadLen + sizeof(st_MSG_HEAD);
			if (messageQ.GetUseSize() < size)
				break;
		
			char* message = new char[size];
			messageQ.Dequeue(message, size);
			wstring realMessage(message + sizeof(st_MSG_HEAD), message + size);

			Terminate = MessageProc(head.shType, realMessage);

			delete[] message;
		} while (messageQ.GetUseSize() >= sizeof(st_MSG_HEAD));
	}
	
	return 0;
}

unsigned int WINAPI Monitor(LPVOID lpParam)
{
	int total = 0;

	while (MonitorTerminate == false)
	{	
		wprintf(L"Message Job Queue Use : %d\n", messageQ.GetUseSize());
		for (int i = 0; i < dfJOB_QUIT; i++)
		{
			wprintf(L"JobDoing %d Count : %d\n", i, Job[i]);
			total += Job[i];
		}
			
		for(auto iter = ThreadMap.begin(); iter != ThreadMap.end(); ++iter)
			wprintf(L"Thread ID %d Count : %d\n", iter->first, iter->second);

		wprintf(L"TPS Count : %d\n", total);

		Sleep(1000);
		total = 0;
	}

	return 0;
}

bool MessageProc(short Type, wstring message)
{
	switch (Type)
	{
	case dfJOB_ADD:
		gList.push_back(message);
		InterlockedIncrement(&Job[dfJOB_ADD]);
		break;
	case dfJOB_DEL:
		if(false == gList.empty())
			gList.pop_back();
		InterlockedIncrement(&Job[dfJOB_DEL]);
		break;
	case dfJOB_SORT:
		gList.sort();
		InterlockedIncrement(&Job[dfJOB_SORT]);
		break;
	case dfJOB_FIND:
	{
		auto iter = find(gList.begin(), gList.end(), message);
		InterlockedIncrement(&Job[dfJOB_FIND]);
	}
	break;
	case dfJOB_PRINT:
		wcout << message << "\n";
		InterlockedIncrement(&Job[dfJOB_PRINT]);
		break;
	case dfJOB_QUIT:
		return true;
		break;
	}

	return false;
}