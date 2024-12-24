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
TSharedPtr<SendBuffer> ServerPacketHandler::Make_S_TEST(uint64 Id, uint32 Hp, uint16 Attack, vector<BuffData> Buffs)
{
	Protocol::S_TEST Packet;
	Packet.set_id(10);
	Packet.set_hp(100);
	Packet.set_attack(10);
	{
		Protocol::BuffData* Data = Packet.add_buffs();
		Data->set_buffid(100);
		Data->set_remaintime(1.2f);
		{
			Data->add_victims(10);
		}
	}
	{
		Protocol::BuffData* Data = Packet.add_buffs();
		Data->set_buffid(200);
		Data->set_remaintime(2.2f);
		{
			Data->add_victims(20);
		}
	}

	return MakeSendBuffer(Packet, S_TEST);
}
