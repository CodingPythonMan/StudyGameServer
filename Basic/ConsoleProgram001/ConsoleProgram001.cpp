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

int main()
{
	timeBeginPeriod(1);

	int frame = 0;
	unsigned int tick = 0;
	unsigned int curTime = timeGetTime();
	
	while (1)
	{
		if (PrintFrame(curTime, frame))
		{
			frame = 0;
			tick = 0;
		}

		//BusyJob();
		frame++;
		tick = timeGetTime() - curTime;

		Sleep(WAIT);
	}

	return 0;
}