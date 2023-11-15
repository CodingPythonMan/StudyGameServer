#pragma once

#include "resource.h"

// 헤더
struct stHEADER
{
	unsigned short Len;
};

// 패킷
struct st_DRAW_PACKET
{
	int		iStartX;
	int		iStartY;
	int		iEndX;
	int		iEndY;
};