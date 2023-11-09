#include <iostream>
#include <Windows.h>
using namespace std;

#define WAIT 20

#pragma comment(lib, "winmm.lib")

int main()
{
	timeBeginPeriod(1);

	int frame = 0;
	unsigned int tick = 0;
	unsigned int curTime = timeGetTime();
	unsigned int ourTime = curTime;
	float frameTime = 0.0f;

	bool skipDraw = false;

	while (1)
	{
		// Logic
		int total = 0;
		for (int i = 0; i < 100; i++)
			total += i;

		// Frame Count
		frame++;
		float timeDelta = (timeGetTime() - curTime) * 0.001f;
		frameTime += timeDelta;
		if (frameTime >= 1.0f)
		{
			cout << "[FrameCount] : " << frame << "\n";
			frame = 0;
			frameTime = 0.0f;
		}

		// Render
		if (skipDraw == false)
		{
			//printf("tick : %d, curTime : %d, ourTime : %d, frame : %d\n", tick, curTime, ourTime, frame);
		}

		// Cal
		curTime = timeGetTime();
		tick = curTime - ourTime;
		ourTime += WAIT;

		// Skip
		if (tick <= WAIT)
		{
			Sleep(WAIT - tick);
			skipDraw = false;
		}
		else
			skipDraw = true;
	}

	return 0;
}