#include "ActorPattern.h"

list<wstring>	gList;
RingBuffer		messageQ;
SRWLOCK			lock;

unsigned int WINAPI Work(LPVOID lpParam)
{
	return 0;
}