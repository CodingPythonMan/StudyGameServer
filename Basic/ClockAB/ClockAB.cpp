#include <iostream>
#include <windows.h>
#pragma comment(lib, "winmm.lib")
using namespace std;

#define WAIT 20

int CallA = 0;
int CallB = 0;

void A()
{
	CallA++;
	//cout << "A()" << "\n";
}

void B()
{
	CallB++;
	//cout << "B()" << "\n";
}

int main()
{
	timeBeginPeriod(1);

	unsigned int curTime = timeGetTime();
	unsigned int ourTime = curTime;
	unsigned int frameTime = curTime;
	int Frame = 0;
	printf("startTime : %d\n", curTime);
	while (1)
	{
		A();

		if (ourTime <= curTime)
		{
			B();
			Frame++;
			ourTime += WAIT;
		}

		curTime = timeGetTime();
		if (curTime - frameTime >= 1000)
		{
			printf("curTime : %d, frameTime : %d\n", curTime, frameTime);
			printf("Frame : %d\n", Frame);
			printf("CallA : %d\n", CallA);
			printf("CallB : %d\n", CallB);

			// 1초동안 목표치보다 적게 돌았다면, 더 많은 Frame 돌기
			// 그런데 1초 동안이 아니라 여러초동안 안 돌았을 수 있음.
			Frame = 0;
			frameTime = curTime;
		}
	}
}