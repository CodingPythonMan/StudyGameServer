#pragma once
enum class MessageType : int {
	AssignID,
	CreateStar,
	DeleteStar,
	MoveStar,
};

struct AssignID {
	int Type;
	int ID;
};

struct CreateStar {
	int Type;
	int ID;
	int X;
	int Y;
};

struct DeleteStar {
	int Type;
	int ID;
};

struct MoveStar {
	int Type;
	int ID;
	int X;
	int Y;
};