#include "Rand.h"

// 뽑기의 회차가 누적 계산이 되고 있어야 함.
int iCount = 0;

st_ITEM g_Gatcha[] = {
	{"칼",				20, 0},
	{"방패",				20, 0},
	{"신발",				20, 0},
	{"물약",				20, 0},
	{"초강력레어무기",		5, 0},
	{"초강력방패",			5, 0},
	{"초강력레어레어레어신발1", 1, 50},
	{"초강력레어레어레어신발2", 1, 51},
	{"초강력레어레어레어신발3", 1, 10}

	// 마지막 3개의 아이템은 일반 확률로는 나오지 않으며
	// 뒤에 입력된 WinTime 회차때만 100% 로 나옴.
};

void Gatcha()
{
	// 뽑기 회차 증가. (이는 전역적으로 계산 되어야 함)
	iCount++;

	// 1. 전체 아이템들의 비율 총 합을 구함.
	// 단, WinTime 이 정해진 아이템은 확률의 대상 자체가 아니기 때문에 제외.
	int total = 0;
	int max = 0;
	for (const auto& item : g_Gatcha)
	{
		if (item.WinTime > 0)
		{
			if (max < item.WinTime)
			{
				max = item.WinTime;
			}
			continue;
		}
		
		total += item.Rate;
	}

	// 2. 본 뽑기 회차에 대한 지정 아이템이 있는지 확인
	// WinTime 과 iCount 가 같은 아이템을 찾는다.
	// 있다면..그 아이템을 뽑고 중단.
	for (const auto& item : g_Gatcha)
	{
		if (item.WinTime == iCount)
		{
			printf("%d 가차 결과 : %s\n", iCount, item.Name);
			if (iCount == max)
			{
				iCount = 0;
			}
			
			return;
		}
	}

	// 3. rand() 함수로 확률을 구함
	// 여기서 확률은 1 / 100 이 아니며, 1 / 총합비율 임.

	// int iRand = 확률구하기;
	int random = (rand() % total) + 1;

	// 4. 전체 아이템 테이블을 돌면서
	// 위에서 구한 Rand 값에 해당 지점의 아이템을 찾는다.
	for (const auto& item : g_Gatcha)
	{
		if (random <= item.Rate)
		{
			printf("%d 가차 결과 : %s\n", iCount, item.Name);
			break;
		}

		random -= item.Rate;
	}

	// 5. 뽑기 회차를 초기화 해야할지 판단하여 초기화.
}

int MakeBigNumRand()
{
	return 5;
}