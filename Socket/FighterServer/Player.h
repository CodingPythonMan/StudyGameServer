#include "Direction.h"

#define DEFAULT_HP 100

#define RANGE_MOVE_TOP	50
#define RANGE_MOVE_LEFT	10
#define RANGE_MOVE_RIGHT	630
#define RANGE_MOVE_BOTTOM	470

#define ERROR_RANGE		50

//---------------------------------------------------------------
// 공격범위.
//---------------------------------------------------------------
#define ATTACK1_RANGE_X		80
#define ATTACK2_RANGE_X		90
#define ATTACK3_RANGE_X		100
#define ATTACK1_RANGE_Y		10
#define ATTACK2_RANGE_Y		10
#define ATTACK3_RANGE_Y		20

class Player
{
public:
	Player();
	~Player();

	bool IsDead();
	void MovePlayer();
	bool MovePos(short X, short Y, bool Move);
	void NotifyPlayer(short* X, short* Y, unsigned char* HP = nullptr);
	void CheckOnAttackRange(Player* otherPlayer, short rangeX, short rangeY);

	MoveType _MoveType;
	Direction _Direct;
private:
	short _X;
	short _Y;
	unsigned char _HP;
	bool _IsMove;
};