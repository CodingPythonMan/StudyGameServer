#include <iostream>
#include <Windows.h>
using namespace std;

#define WAIT 20

#pragma comment(lib, "winmm.lib")

bool SkipDraw()
{

}

void BusyJob()
{
	int total = 0;
	for (int i = 0; i < 10000; i++)
	{
		for (int j = 0; j < 1000; j++)
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
	unsigned int frameTime = curTime;

	while (1)
	{
		unsigned int time = timeGetTime();
		if (time - frameTime >= 1000)
		{
			cout << "[FrameCount] : " << frame << "\n";
			frame = 0;
			frameTime = time;
		}

		BusyJob();
		frame++;
		curTime = timeGetTime();
		tick = curTime - ourTime;
		ourTime += WAIT;

		if (tick <= WAIT)
			Sleep(WAIT - tick);
		else
			Sleep(1);
	}

	return 0;
}