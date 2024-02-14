#include <iostream>
#include <list>
using namespace std;

int DataArr[3];

struct Player
{
	int SessionID;
	int Content[3];
};

int main()
{
	int stackArr[3];
	list<Player*> PlayerList;

	for (int i = 0; i < 4; i++)
	{
		Player* player = new Player;

		PlayerList.push_back(player);
		stackArr[i]++;
		DataArr[i]++;
	}
}