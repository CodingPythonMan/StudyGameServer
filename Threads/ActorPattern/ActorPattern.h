#pragma once

#include <list>
#include <string>
#include <process.h>
#include "RingBuffer.h"
#include "Lock.h"
using namespace std;

#define WORKER_THREAD_NUMBER 3

#define dfJOB_ADD	0
#define dfJOB_DEL	1
#define dfJOB_SORT	2
#define dfJOB_FIND	3
#define dfJOB_PRINT	4	//<< 출력 or 저장 / 읽기만 하는 느린 행동
#define dfJOB_QUIT	5	

//#pragma pack(push, 1)
struct st_MSG_HEAD
{
	short shType;
	short shPayloadLen;
};
//#pragma pack(pop)

unsigned int WINAPI Work(LPVOID lpParam);
unsigned int WINAPI Monitor(LPVOID lpParam);

bool MessageProc(short Type, wstring message);

//-----------------------------------------------
// 컨텐츠 부, 문자열 리스트
//-----------------------------------------------
extern list<wstring>	gList;

//-----------------------------------------------
// 스레드 메시지 큐 (사이즈 넉넉하게 크게 4~5만 바이트)
//-----------------------------------------------
extern RingBuffer		messageQ;

extern SRWLOCK			lock;
extern HANDLE			gEvent;
extern bool				MonitorTerminate;