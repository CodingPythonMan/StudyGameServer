#include "Proxy.h"

void CMDCreateMyCharacter(const MyList<Session*>& SessionList, const Session* session, const int& ID, const unsigned char& Direct, const short& X, const short& Y, const unsigned char& HP)
{
	Packet packet;
	PACKET_HEADER header;
	header.ByCode = 0x89;
	header.BySize = sizeof(const int&) +sizeof(const unsigned char&) +sizeof(const short&) +sizeof(const short&) +sizeof(const unsigned char&) ;
	header.ByType = cmdcreatemycharacter;
	packet.PutData((char*)&header, HEADER_SIZE);

	packet << ID << Direct << X << Y << HP;
	SendMessage(SessionList, session, packet);
}

void CMDCreateOtherCharacter(const MyList<Session*>& SessionList, const Session* session, const int& ID, const unsigned char& Direct, const short& X, const short& Y, const unsigned char& HP)
{
	Packet packet;
	PACKET_HEADER header;
	header.ByCode = 0x89;
	header.BySize = sizeof(const int&) +sizeof(const unsigned char&) +sizeof(const short&) +sizeof(const short&) +sizeof(const unsigned char&) ;
	header.ByType = cmdcreateothercharacter;
	packet.PutData((char*)&header, HEADER_SIZE);

	packet << ID << Direct << X << Y << HP;
	SendMessage(SessionList, session, packet);
}

void CMDDeleteCharacter(const MyList<Session*>& SessionList, const Session* session)
{
	Packet packet;
	PACKET_HEADER header;
	header.ByCode = 0x89;
	header.BySize = 0;
	header.ByType = cmddeletecharacter;
	packet.PutData((char*)&header, HEADER_SIZE);

	SendMessage(SessionList, session, packet);
}

void QRYMoveStart(const MyList<Session*>& SessionList, const Session* session)
{
	Packet packet;
	PACKET_HEADER header;
	header.ByCode = 0x89;
	header.BySize = 0;
	header.ByType = qrymovestart;
	packet.PutData((char*)&header, HEADER_SIZE);

	SendMessage(SessionList, session, packet);
}

void REPMoveStart(const MyList<Session*>& SessionList, const Session* session)
{
	Packet packet;
	PACKET_HEADER header;
	header.ByCode = 0x89;
	header.BySize = 0;
	header.ByType = repmovestart;
	packet.PutData((char*)&header, HEADER_SIZE);

	SendMessage(SessionList, session, packet);
}

void QRYMoveStop(const MyList<Session*>& SessionList, const Session* session)
{
	Packet packet;
	PACKET_HEADER header;
	header.ByCode = 0x89;
	header.BySize = 0;
	header.ByType = qrymovestop;
	packet.PutData((char*)&header, HEADER_SIZE);

	SendMessage(SessionList, session, packet);
}

void REPMoveStop(const MyList<Session*>& SessionList, const Session* session)
{
	Packet packet;
	PACKET_HEADER header;
	header.ByCode = 0x89;
	header.BySize = 0;
	header.ByType = repmovestop;
	packet.PutData((char*)&header, HEADER_SIZE);

	SendMessage(SessionList, session, packet);
}

void QRYAttack001(const MyList<Session*>& SessionList, const Session* session)
{
	Packet packet;
	PACKET_HEADER header;
	header.ByCode = 0x89;
	header.BySize = 0;
	header.ByType = qryattack001;
	packet.PutData((char*)&header, HEADER_SIZE);

	SendMessage(SessionList, session, packet);
}

void REPAttack001(const MyList<Session*>& SessionList, const Session* session)
{
	Packet packet;
	PACKET_HEADER header;
	header.ByCode = 0x89;
	header.BySize = 0;
	header.ByType = repattack001;
	packet.PutData((char*)&header, HEADER_SIZE);

	SendMessage(SessionList, session, packet);
}

void SendMessage(const MyList<Session*>& SessionList, const Session* session, const Packet& packet)
{
}
