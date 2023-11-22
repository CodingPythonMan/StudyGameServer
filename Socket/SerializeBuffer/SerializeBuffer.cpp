#include "SerializeBuffer.h"
#include <cstring>

Packet::Packet()
{
	Buffer = new char[MAX_PACKET_SIZE];
	BufferSize = MAX_PACKET_SIZE;
	DataSize = 0;
	ReadPos = 0;
	WritePos = 0;
}

Packet::Packet(int bufferSize)
{
	Buffer = new char[bufferSize];
	BufferSize = bufferSize;
	DataSize = 0;
	ReadPos = 0;
	WritePos = 0;
}

Packet::~Packet()
{
	delete[] Buffer;
}

void Packet::Clear(void)
{
	memset(Buffer, 0, BufferSize);
}

int Packet::MoveWritePos(int size)
{
	if (size > 0 && DataSize + size < BufferSize)
	{
		WritePos += size;

	}
	else
	{
		size = 0;
	}

	return size;
}

int Packet::MoveReadPos(int size)
{
	if (size > 0)
	{

	}
	else
	{
		size = 0;
	}
}

Packet& Packet::operator=(Packet& srcPacket)
{
	Buffer = srcPacket.Buffer;

}

Packet& Packet::operator<<(unsigned char value)
{


	WritePos++;
}

Packet& Packet::operator<<(char value)
{
	
}

Packet& Packet::operator<<(short value)
{
	// TODO: insert return statement here
}

Packet& Packet::operator<<(unsigned short value)
{
	// TODO: insert return statement here
}

Packet& Packet::operator<<(int value)
{
	// TODO: insert return statement here
}

Packet& Packet::operator<<(long value)
{
	// TODO: insert return statement here
}

Packet& Packet::operator<<(float value)
{
	// TODO: insert return statement here
}

Packet& Packet::operator<<(__int64 value)
{
	// TODO: insert return statement here
}

Packet& Packet::operator<<(double value)
{
	// TODO: insert return statement here
}

Packet& Packet::operator>>(unsigned char& value)
{
	// TODO: insert return statement here
}

Packet& Packet::operator>>(char& value)
{
	// TODO: insert return statement here
}

Packet& Packet::operator>>(short& value)
{
	// TODO: insert return statement here
}

Packet& Packet::operator>>(unsigned short& value)
{
	// TODO: insert return statement here
}

Packet& Packet::operator>>(int& value)
{
	// TODO: insert return statement here
}

Packet& Packet::operator>>(unsigned int& value)
{
	// TODO: insert return statement here
}

Packet& Packet::operator>>(float& value)
{
	// TODO: insert return statement here
}

Packet& Packet::operator>>(__int64& value)
{
	// TODO: insert return statement here
}

Packet& Packet::operator>>(double& value)
{
	// TODO: insert return statement here
}

int Packet::GetData(char* chpDest, int size)
{
	return 0;
}

int Packet::PutData(char* chpSrc, int srcSize)
{
	return 0;
}
