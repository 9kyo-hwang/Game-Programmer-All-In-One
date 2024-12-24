#include "pch.h"
#include "ServerPacketHandler.h"
#include "BufferReader.h"
#include "BufferWriter.h"
#include "IOCPSession.h"

void ServerPacketHandler::HandlePacket(BYTE* Buffer, int32 Len)
{
	BufferedReader Reader(Buffer, Len);
	PacketHeader Header;
	Reader.Peek(&Header);

	switch (Header.ID)
	{
	default:
		break;
	}
}

// [Size | ID][Id, Hp, Attack]
TSharedPtr<SendBuffer> ServerPacketHandler::Make_S_TEST(uint64 Id, uint32 Hp, uint16 Attack, vector<BufferData> Buffers)
{
	TSharedPtr<SendBuffer> Buffer = make_shared<SendBuffer>(4096);
	BufferedWriter Writer(Buffer->GetData(), Buffer->Max());
	PacketHeader* Header = Writer.Reserve<PacketHeader>();

	Writer << Id << Hp << Attack;
	Writer << static_cast<uint16>(Buffers.size());
	for (BufferData& Data : Buffers)
	{
		Writer << Data.BufferID << Data.RemainTime;
	}

	Header->Size = Writer.GetSize();
	Header->ID = S_TEST;  // 1;

	Buffer->Close(Writer.GetSize());

	return Buffer;
}
