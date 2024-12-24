#include "pch.h"
#include "SendBuffer.h"

SendBuffer::SendBuffer(int32 NewSize)
{
	Buffer.resize(NewSize);
}

SendBuffer::~SendBuffer()
{
}

void SendBuffer::Copy(void* Data, int32 Len)
{
	assert(Max() >= Len);
	::memcpy(Buffer.data(), Data, Len);
	WriteSize = Len;
}

void SendBuffer::Close(uint32 NewWriteSize)
{
	WriteSize = NewWriteSize;
}
