#include "Player.h"
#include <cstdlib>

Player::Player()
{
	_Direct = Direction::RR;
	_MoveType = MoveType::MOVE_DIR_DD;
	// 위치는 랜덤하게 생성
	_X = RANGE_MOVE_LEFT + (rand() % (RANGE_MOVE_RIGHT - RANGE_MOVE_LEFT - 1)) + 1;
	_Y = RANGE_MOVE_TOP + (rand() % (RANGE_MOVE_BOTTOM - RANGE_MOVE_TOP - 1)) + 1;
	_HP = DEFAULT_HP;
	_IsMove = false;
}

Player::~Player()
{
}

bool Player::IsDead()
{
	bool result = false;

	if (_HP <= 0)
		result = true;

	return result;
}

void Player::MovePlayer()
{
	short moveX = 0;
	short moveY = 0;

	if (false == _IsMove)
	{
		switch (_MoveType)
		{
		case MoveType::MOVE_DIR_DD:
			moveY = 2;
			break;
		case MoveType::MOVE_DIR_LD:
			moveX = -3;
			moveY = 2;
			break;
		case MoveType::MOVE_DIR_LL:
			moveX = -3;
			break;
		case MoveType::MOVE_DIR_LU:
			moveX = -3;
			moveY = -2;
			break;
		case MoveType::MOVE_DIR_RD:
			moveX = 3;
			moveY = 2;
			break;
		case MoveType::MOVE_DIR_RR:
			moveX = 3;
			break;
		case MoveType::MOVE_DIR_RU:
			moveX = 3;
			moveY = -2;
			break;
		case MoveType::MOVE_DIR_UU:
			moveY = -2;
			break;
		}

		if (_X + moveX <= RANGE_MOVE_LEFT || _X + moveX >= RANGE_MOVE_RIGHT 
			|| _Y + moveY <= RANGE_MOVE_TOP || _Y + moveY >= RANGE_MOVE_BOTTOM)
			return;

		_X += moveX;
		_Y += moveY;
	}
}

bool Player::MovePos(short X, short Y, bool Move)
{
	if (X <= RANGE_MOVE_LEFT || X >= RANGE_MOVE_RIGHT || Y <= RANGE_MOVE_TOP || Y >= RANGE_MOVE_BOTTOM)
		return false;

	// 에러 범위 이상이라면 오류 체크
	if (_X - X > ERROR_RANGE || _X - X  < -ERROR_RANGE || _Y - Y > ERROR_RANGE || _Y - Y < -ERROR_RANGE)
		return true;

	_X = X;
	_Y = Y;
	_IsMove = Move;

	return false;
}

void Player::NotifyPlayer(short* X, short* Y, unsigned char* HP)
{
	*X = _X;
	*Y = _Y;
	if(HP != nullptr)
		*HP = _HP;
}

void Player::CheckOnAttackRange(Player* otherPlayer, short rangeX, short rangeY)
{


	short halfX = rangeX / 2;
	short halfY = rangeY / 2;


}