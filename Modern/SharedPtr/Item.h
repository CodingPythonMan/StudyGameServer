#pragma once
#include "Smart.h"

class Item : public Smart<Item>
{
public:
	void Reset();
};

