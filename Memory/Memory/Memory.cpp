#include <iostream>
#include <vector>

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

int main()
{
	std::vector<int> idList;

	idList.push_back(1);
	idList.push_back(2);
	idList.push_back(3);

	std::vector<int> tempList;

	tempList = idList;
}