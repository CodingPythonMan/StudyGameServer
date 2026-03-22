#include <iostream>
#include "ConcatArray.h"

int main()
{
	ConcatArray array;

	std::vector<int> ex{ 1,2,3,1 };
	array.getConcatenation(ex);

	for (auto i : ex)
	{
		std::cout << i << "\n";
	}
}