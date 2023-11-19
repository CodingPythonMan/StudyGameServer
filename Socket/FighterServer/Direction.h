#pragma once

enum class Direction : unsigned char {
	LL = 0,
	RR
};

enum class MoveType : unsigned char {
	MOVE_DIR_LL = 0,
	MOVE_DIR_LU,
	MOVE_DIR_UU,
	MOVE_DIR_RU,
	MOVE_DIR_RR,
	MOVE_DIR_RD,
	MOVE_DIR_DD,
	MOVE_DIR_LD
};