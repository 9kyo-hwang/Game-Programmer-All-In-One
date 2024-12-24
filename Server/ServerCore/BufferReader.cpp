#include "pch.h"
#include "BufferReader.h"

BufferedReader::BufferedReader()
{
}

BufferedReader::BufferedReader(BYTE* InBuffer, uint32 InSize, uint32 InPos)
	: Buffer(InBuffer)
	, Size(InSize)
	, Pos(InPos)
{
}

BufferedReader::~BufferedReader()
{
}

bool BufferedReader::Peek(void* Dest, uint32 Len)
{
	if (GetFreeSize() < Len)
	{
		return false;
	}

	::memcpy(Dest, &Buffer[Pos], Len);
	return true;
}

bool BufferedReader::Read(void* Dest, uint32 Len)
{
	if (!Peek(Dest, Len))
	{
		return false;
	}

	Pos += Len;
	return true;
}