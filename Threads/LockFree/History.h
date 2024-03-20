#pragma once
#include <windows.h>

struct History {
	int Action;
	LONG64 newNode;
	LONG64 lastNode;
	LONG64 _top;
};