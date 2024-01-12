#include <iostream>
#include "SerializeBuffer.h"

#define PACKET_CODE 0x89
#define MOVE_START 0
#define HEADER_SIZE 3

#pragma pack(push, 1)
struct Header {
	unsigned char ByCode;
	unsigned char BySize;
	unsigned char ByType;
};
#pragma pack(pop)

void Test()
{
	unsigned char Wow[8] = { 0x89, 0x05, 0x0a, 0x00, 0xf9, 0x18, 0xe2, 0x00 };

	Packet packet;
	for (int i = 0; i < 8; i++)
		packet << Wow[i];

	std::cout << 1;
}

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
	short x = 129;
	short y = 6400;

	//Test();

	Packet packet;
	MoveStart(&packet, dir, x, y);

	Packet aPacket;
	aPacket = packet;

	SendPacket(&packet);
}