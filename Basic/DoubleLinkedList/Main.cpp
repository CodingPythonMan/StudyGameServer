#include <iostream>
#include "MyList.h"

int main()
{
	MyList<int> ListInt;
	ListInt.push_front(3);
	ListInt.push_back(4);

	MyList<int>::iterator iter;
}