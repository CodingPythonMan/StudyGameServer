#include "Test.h"

bool Test::PutTestData(int Data)
{
	MyQueue.push(Data);

	return false;
}

bool Test::PutReliableData(int Data)
{
	ReliableQueue.push(Data);

	return false;
}