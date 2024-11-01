#include <iostream>
#include <unordered_map>
#include <tuple>

int main()
{
	std::unordered_map<int, std::tuple<int, int>> map;

	map.insert_or_assign(1, std::make_tuple(1, 1));
	map.insert_or_assign(1, std::make_tuple(2, 3));

	for (auto row : map)
	{
		printf("Hey : %d", std::get<0>(row.second));
	}
}