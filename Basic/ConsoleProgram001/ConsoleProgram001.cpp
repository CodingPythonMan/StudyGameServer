#include <iostream>
using namespace std;

int main()
{
	int n;
	cin >> n;

	int* loop = new int[1000];
	string* str = new string[1000];

	for (int i = 0; i < n; i++)
	{
		cin >> loop[i];
		cin >> str[i];
	}

	for (int i = 0; i < n; i++)
	{
		for (size_t k = 0; k < str[i].length(); k++)
		{
			for (int j = 0; j < loop[i]; j++)
			{
				cout << str[i][k];
			}
		}
		cout << "\n";
	}

	return 0;
}