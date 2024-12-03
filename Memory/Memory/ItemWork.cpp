#include "ItemWork.h"

void ItemWork::InsertItem(int x, int y, int z)
{
	Item item;
	item.x = x;
	item.y = y;
	item.z = z;

	mItemList.push_back(item);
}