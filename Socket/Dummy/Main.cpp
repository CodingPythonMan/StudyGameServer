#include "Dummy.h"
#include <iostream>
using namespace std;

int main()
{
	cout << "[Dummy System] 에 오신 것을 환영합니다.\n";
	int menu = 0;

	while (1)
	{
		cout << "[메뉴]\n";
		cout << "\t1. TCP Fighter 더미 만듣기(40) \n";
		cin >> menu;
		
		switch (menu)
		{
		case 1:
			cout << "1번 선택되었습니다. \n";
			
		default:
			return;
		}
	}
}