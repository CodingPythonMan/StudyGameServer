#include <iostream>
#include <Windows.h>
#include <timeapi.h>

int main()
{
	int FrameCount = 0;
	DWORD Tick = timeGetTime();

	while (1)
	{
		FrameCount++;
		if (timeGetTime() - Tick >= 1000)
		{
			printf("%d \n", FrameCount);
			FrameCount = 0;
			Tick = timeGetTime();
		}

		Sleep(20);
	}
}