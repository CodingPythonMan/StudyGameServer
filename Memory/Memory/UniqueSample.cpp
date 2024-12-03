#include "UniqueSample.h"

void UniqueSample::UseVector(std::unique_ptr<Wow> ptr)
{
	for (int id : ptr->idList)
	{
		printf("id : %d\n", id);
	}
}

void UniqueSample::CreateUniqueVector()
{
	std::unique_ptr<Wow> ptr = std::make_unique<Wow>();
	{
		std::vector<int> idList;

		idList.push_back(1);
		idList.push_back(2);
		idList.push_back(3);

		ptr->idList = idList;
	}

	UseVector(std::move(ptr));
}