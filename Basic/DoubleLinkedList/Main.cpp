#include <iostream>
#include "MyList.h"

int main()
{
	MyList<int> ListInt;
	ListInt.push_front(3);
	ListInt.push_back(4);

	// 순회 샘플 코드
	MyList<int>::iterator iter;
	for (iter = ListInt.begin(); iter != ListInt.end(); ++iter)
	{
		printf("%d", *iter);
	}
}