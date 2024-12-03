#include <iostream>
#include "ItemWork.h"

#define Max 4

void MemsetPractice()
{
	int arr[Max];
	memset(arr + 1, 0, sizeof(int) * (Max - 1));

	for (int i : arr)
	{
		printf("arr = %d\n", i);
	}
}

void GetItemList(std::vector<Item>& myItemList)
{
	ItemWork itemWork;

	itemWork.InsertItem(1, 2, 3);
	itemWork.InsertItem(3, 2, 1);
	itemWork.InsertItem(4, 4, 4);

	myItemList = itemWork.mItemList;
}

int main()
{
	std::vector<Item> myItemList;

	GetItemList(myItemList);

	for (auto& item : myItemList)
	{
		printf("x : %d, y : %d, z : %d\n", item.x, item.y, item.x);
	}
}