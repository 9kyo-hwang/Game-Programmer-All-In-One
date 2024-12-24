#include "pch.h"
#include "BufferWriter.h"

BufferedWriter::BufferedWriter()
{
}

BufferedWriter::BufferedWriter(BYTE* InBuffer, uint32 InSize, uint32 InPos)
	: Buffer(InBuffer)
	, Size(InSize)
	, Pos(InPos)
{
}

BufferedWriter::~BufferedWriter()
{
}

bool BufferedWriter::Write(void* Src, uint32 Len)
{
	if (GetFreeSize() < Len)
	{
		return false;
	}

	::memcpy(&Buffer[Pos], Src, Len);
	Pos += Len;
	return true;
}
