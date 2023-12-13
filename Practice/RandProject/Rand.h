#include <iostream>
#include <conio.h>
using namespace std;

struct st_ITEM
{
	char	Name[30];
	int	Rate;		// 일반 랜덤 뽑기와 같은 용도
	int	WinTime;	// 이 아이템이 나올 뽑기 회차.
	// 0 이면 일반 아이템
	// 0 이 아니면 그 회차에만 나옴
};

void Gatcha();

// 큰 숫자 Rand 만들기
int MakeIntRand();