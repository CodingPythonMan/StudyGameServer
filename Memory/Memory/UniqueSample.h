#pragma once
#include <vector>
#include <memory>

struct Wow
{
	std::vector<int> idList;
};

class UniqueSample
{
	void UseVector(std::unique_ptr<Wow> ptr);

	void CreateUniqueVector();
};