#include <iostream>
#include <conio.h>

struct st_ITEM
{
    char Name[30];
    int Rate;
};

st_ITEM g_Gatcha[] = {
	{"칼",		20},
	{"방패",		20},
	{"신발",		20},
	{"물약",		20},
	{"초강력레어무기",	5},
	{"초강력방패",		5},
	{"초강력레어레어레어신발", 1}
};

void Gatcha()
{
	// 1. 전체 아이템들의 비율 총 합을 구함
	int total = 0;
	int index = 0;
	for (const auto& item : g_Gatcha)
	{
		total += item.Rate;
	}

	// 2. rand() 함수로 확률을 구함
	// 여기서 확률은 1/100 이 아니며, 1/총합비율임

	// int iRand = 확률 구하기
	int random = (rand() % total) + 1;

	// 3. 전체 아이템 테이블을 돌면서
	// 위에서 구한 Rand 값에 해당 지점의 아이템을 찾는다.
	while (random > g_Gatcha[index].Rate)
	{
		random -= g_Gatcha[index].Rate;
		index++;
	}
	printf("가챠 결과 : %s\n", g_Gatcha[index].Name);
}


int main()
{
	while (1)
	{
		_getch();
		Gatcha();
	}
}