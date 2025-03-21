#include "pch.h"
#include "ServerPacketHandler.h"
#include "BufferReader.h"
#include "GameSession.h"
#include "GameZone.h"
#include "IOCPSession.h"

void ServerPacketHandler::HandlePacket(SessionRef Session, BYTE* Buffer, int32 Len)
{
	BufferedReader Reader(Buffer, Len);
	PacketHeader Header;
	Reader.Peek(&Header);

	// Client로부터 받은 패킷이 어떤 것이냐에 따라 분기 처리
	switch (Header.ID)
	{
	case C_Move:
		Incoming_C_Move(Session, Buffer, Len);
		break;
	default:
		break;
	}
}

void ServerPacketHandler::Incoming_C_Move(SessionRef Session, BYTE* Buffer, int32 Len)
{
	PacketHeader* Header = reinterpret_cast<PacketHeader*>(Buffer);
	uint16 Size = Header->Size;

	Protocol::C_Move Packet;
	Packet.ParseFromArray(&Header[1], Size - sizeof(PacketHeader));

	// 컨텐츠 코드
	if (TSharedPtr<GameZone> Zone = Session->Zone.lock())
	{
		Zone->Incoming_C_Move(Packet);
	}
}

// [Size | ID][Id, Hp, Attack]
TSharedPtr<SendBuffer> ServerPacketHandler::Outgoing_S_TEST(uint64 Id, uint32 Hp, uint16 Attack, vector<BuffData> Buffs)
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

TSharedPtr<SendBuffer> ServerPacketHandler::Outgoing_S_EnterGame()
{
	Protocol::S_EnterGame Packet;
	Packet.set_success(true);
	Packet.set_accountid(0);  // from DB

	return MakeSendBuffer(Packet, S_EnterGame);
}

TSharedPtr<SendBuffer> ServerPacketHandler::Outgoing_S_LocalPlayer(const Protocol::ObjectInfo& Info)
{
	Protocol::S_LocalPlayer Packet;
	*Packet.mutable_info() = Info;
	return MakeSendBuffer(Packet, S_LocalPlayer);
}

TSharedPtr<SendBuffer> ServerPacketHandler::Outgoing_S_Move(const Protocol::ObjectInfo& Info)
{
	Protocol::S_Move Packet;
	*Packet.mutable_info() = Info;
	return MakeSendBuffer(Packet, S_Move);
}
