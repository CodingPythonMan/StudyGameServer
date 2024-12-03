#pragma once
#include <vector>

struct Item
{
	int x;
	int y;
	int z;
};

class ItemWork
{
public:
	void InsertItem(int x, int y, int z);

	std::vector<Item> mItemList;
};