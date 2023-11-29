#include <iostream>
#include "MemoryPool.h"
#include "Player.h"

int main()
{
	MemoryPool<Player> PlayerPool(3, true);
}