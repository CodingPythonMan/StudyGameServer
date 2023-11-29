#include "Player.h"
#include <iostream>

Player::Player()
{
	_X = 50;
	_Y = 50;
	_Name = new char[30];
}

Player::~Player()
{
	delete[] _Name;
}

void Player::MakeName(char* Name)
{
	memcpy(_Name, Name, 30);
}

void Player::Move(int X, int Y)
{
	_X = X;
	_Y = Y;
}

void Player::NotifyPos()
{
	printf("_X : %d, _Y : %d\n", _X, _Y);
}