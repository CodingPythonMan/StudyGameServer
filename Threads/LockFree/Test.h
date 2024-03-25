#pragma once
#include "LockFreeStack.h"
#include "LockFreeQueue.h"
#include <process.h>

void LockFreeStackTestCode();
unsigned int StackMonitorThread(LPVOID lpParam);
unsigned int StackWorkerThread(LPVOID lpParam);

void LockFreeQueueTestCode();
unsigned int QueueMonitorThread(LPVOID lpParam);
unsigned int QueueWorkerThread(LPVOID lpParam);