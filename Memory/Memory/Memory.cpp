#include <iostream>

#define Max 4

int main()
{
	int arr[Max];
	memset(arr+1, 0, sizeof(int) * (Max-1));

	for (int i : arr)
	{
		printf("arr = %d\n", i);
	}
}