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
	ReadPos = 0;
	WritePos = 0;
}

int Packet::MoveWritePos(int size)
{
	if (size > 0)
	{
		WritePos += size;
		DataSize += size;

		if (WritePos + size > BufferSize)
		{
			__debugbreak();
		}
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
		ReadPos += size;
		DataSize -= size;

		if (ReadPos + size > BufferSize)
		{
			__debugbreak();
		}
	}
	else
	{
		size = 0;
	}

	return size;
}

Packet& Packet::operator=(Packet& srcPacket)
{
	Buffer = srcPacket.Buffer;
	ReadPos = srcPacket.ReadPos;
	WritePos = srcPacket.WritePos;
	BufferSize = srcPacket.BufferSize;
	DataSize = srcPacket.DataSize;
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
	return size;
}

int Packet::PutData(char* chpSrc, int size)
{

	return size;
}
