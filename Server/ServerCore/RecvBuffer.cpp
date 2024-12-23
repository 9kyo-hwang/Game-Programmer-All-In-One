#include "pch.h"
#include "RecvBuffer.h"

RecvBuffer::RecvBuffer(int32 NewSize)
	: Size(NewSize)
{
	Count = Size * BufferCount;
	Buffer.resize(Count);
}

RecvBuffer::~RecvBuffer()
{
}

void RecvBuffer::Clean()
{
	int32 DataSize = GetDataSize();
	if (DataSize == 0)
	{
		Tail = Head = 0;
	}
	else if (GetFreeSize() < Size)
	{
		::memcpy(&Buffer[0], &Buffer[Tail], DataSize);
		Tail = 0;
		Head = DataSize;
	}
}

// Tail(Read Cursor) 이동
bool RecvBuffer::OnRead(int32 NumOfBytes)
{
	if (NumOfBytes > GetDataSize())
	{
		return false;
	}

	Tail += NumOfBytes;
	return true;
}

// Head(Write Cursor) 이동
bool RecvBuffer::OnWrite(int32 NumOfBytes)
{
	if (NumOfBytes > GetFreeSize())
	{
		return false;
	}

	Head += NumOfBytes;
	return true;
}
