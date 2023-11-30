#include <iostream>
#include "SerializeBuffer.h"

#define PACKET_CODE 0x89
#define MOVE_START 0
#define HEADER_SIZE 3

struct Header {
	unsigned char ByCode;
	unsigned char BySize;
	unsigned char ByType;
};

void MoveStart(Packet* srcPacket, unsigned char dir, short x, short y)
{
	Header header;
	header.ByCode = PACKET_CODE;
	header.BySize = 5;
	header.ByType = MOVE_START;

	srcPacket->PutData((char*)&header, HEADER_SIZE);

	*srcPacket << dir;
	*srcPacket << x;
	*srcPacket << y;
}

void SendPacket(Packet* dstPacket)
{
	unsigned char dir;
	short x;
	short y;

	Header header;
	dstPacket->GetData((char*)&header, HEADER_SIZE);

	*dstPacket >> dir;
	*dstPacket >> x;
	*dstPacket >> y;

	printf("[Send Packet] : Dir : %d, X : %d, Y : %d \n", dir, x, y);
}

int main()
{
	// 皋技瘤 积己. 皋技瘤 积己 饶
	unsigned char dir = 1;
	short x = 30;
	short y = 40;

	Packet packet;
	MoveStart(&packet, dir, x, y);

	Packet aPacket;
	aPacket = packet;

	SendPacket(&packet);
}