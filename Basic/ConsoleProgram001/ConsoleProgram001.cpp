#include <iostream>
#include <Windows.h>
using namespace std;

#define WAIT 20

#pragma comment(lib, "winmm.lib")

bool SkipDraw()
{

}

bool PrintFrame(int tick, int frame)
{
	if (timeGetTime() - tick >= 1000)
	{
		cout << "[FrameCount] : " << frame << "\n";
		return true;
	}

	return false;
}

void BusyJob()
{
	int total = 0;
	for (int i = 0; i < 10000; i++)
	{
		for (int j = 0; j < 10000; j++)
		{
			total += i;
		}
	}
}

void CalTimeGetTimeCallCount()
{
	unsigned int curTime = timeGetTime();
	int count = 0;

	while (1)
	{
		count++;
		if (timeGetTime() - curTime >= 1000)
		{
			break;
		}
	}

	cout << "count : " << count << "\n";
}

int main()
{
	timeBeginPeriod(1);

	int frame = 0;
	unsigned int tick = 0;
	unsigned int curTime = 0;
	unsigned int ourTime = timeGetTime();
	unsigned int frameTime = timeGetTime();

	while (1)
	{
		if (PrintFrame(frameTime, frame))
		{
			frame = 0;
			frameTime = timeGetTime();
		}

		//BusyJob();
		frame++;
		curTime = timeGetTime();
		tick = curTime - ourTime;
		ourTime += WAIT;

		Sleep(WAIT - tick);
	}

	return 0;
}