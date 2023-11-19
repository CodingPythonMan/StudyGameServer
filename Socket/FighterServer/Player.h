#include "Direction.h"

#define DEFAULT_HP 100

#define RANGE_MOVE_TOP	50
#define RANGE_MOVE_LEFT	10
#define RANGE_MOVE_RIGHT	630
#define RANGE_MOVE_BOTTOM	470

class Player
{
public:
	Player();
	~Player();

	bool IsDead();
	bool MovePlayer();

private:
	Direction Direct;
	int X;
	int Y;
	int HP;
};