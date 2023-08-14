#include <iostream>
#include <Windows.h>
#include <conio.h>

int g_Timing[] = { 5, 10, 14, 17, 20, 25, 29, 31, 33 };

// 타이밍 데이터
// 뒤로 갈 수록 큰 값을 넣는게 규칙

// # 화면 지우기 system("cls")

// # 시간구하기 - clock() 함수
// 프로세스가 시작된 후의 시간, 1초는 CLOCKS_PER_SEC 값 (1000ms)
// 리턴값은 clock_t (long) 타입

// # 키보드 눌렸는지 확인 - _kbhit() 함수
// 키보드가 눌렸는지 확인 후 눌렸으면 true 리턴
// 단, 키를 누른 후 콘솔키에서 값을 빼주지 않으면 다음 루프에서도 계속 true 리턴

// _getch() 함수를 사용하여 콘솔키 입력을 뽑을 수 있음.
// 본래 사용법 char keyChar = _getch();
// 우리는 _getch() 의 리턴값을 사용하지 않지만 콘솔키를 빼기 위해 씀.

// # abs(X) 절대값 함수

// - 컨텐츠 부
// 1. 화면 상단에 시간이 표시 됨. 초 : 밀리세컨드 (00:000 으로 자리수 맞춤)
// 2. 아래에는 각 키 타이밍의 정보와 해당 타이밍에 성공여부 결과를 표시
// 3. 아무런 키를 누르지 않고 지정 시간을 1초 이상 넘으면 자동으로 fail 처리
// 4. 사용자가 키를 누르면 해당 시간을 체크하여 오차 범위에 따라서 지정 타이밍의 결과가 화면에 표시됨.

// CLOCKS_PER_SEC / 4 한 수치를 1단계로 최대 4단계 까지의 오차로 Great, Good, NoGood, Bad 단계 측정
// 오차라는 것은, + - 모두 해당됨.

int main()
{
	const int timingSize = sizeof(g_Timing) / sizeof(int);
	double timing[timingSize];
	char* grade[timingSize];
	for (int i = 0; i < timingSize; i++)
	{
		timing[i] = g_Timing[i];
	}

	int index = 0;

    while (1)
    {
		// 입력부
		clock_t time = clock();
		double sec = static_cast<double>(time) / CLOCKS_PER_SEC;
		
		if (_kbhit())
		{
			_getch();
			// 컨텐츠 로직
			timing[index] = sec;
			index++;
			
			if (timingSize <= index)
			{
				break;
			}
		}

		// 로직부
		for (int i = 0; i < timingSize; i++)
		{
			double g_time = 
			abs(1);
		}
		

		// 랜더부
		printf("%06.3f Sec\n\n", sec);
		for (const int time : g_Timing)
		{
			printf("%d Sec : \n", time);
		}
		system("cls");
    }
}
