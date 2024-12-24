#include "pch.h"
#include "ClientPacketHandler.h"

#include "BufferReader.h"

void ClientPacketHandler::HandlePacket(BYTE* Buffer, int32 Len)
{
	BufferedReader Reader(Buffer, Len);
	PacketHeader Header;

	Reader >> Header;

	switch (Header.ID)
	{
	case S_TEST:
		Handle_S_TEST(Buffer, Len);
		break;
	default:
		break;
	}
}

// 패킷 설계 TEMP
struct BuffData
{
	uint64 BufferID;
	float RemainTime;
};

struct S_TEST
{
	uint64 Id;
	uint32 Hp;
	uint16 Attack;

	// 가변 데이터(문자열, 바이트 배열(이미지), 리스트 등)
	vector<BuffData> Data;
};

void ClientPacketHandler::Handle_S_TEST(BYTE* Buffer, int32 Len)
{
	PacketHeader* Header = reinterpret_cast<PacketHeader*>(Buffer);
	uint16 Size = Header->Size;

	Protocol::S_TEST Packet;
	/**
	 * PacketHeader 사이즈만큼이 각 인덱스 한 칸 크기
	 * [0]: 실제 헤더가 담긴 부분
	 * [1]~: 이후 데이터가 이어지는 부분
	 * 따라서 [1]부터의 주소를 (전체 패킷 사이즈 - 패킷 헤더 사이즈)만큼의 데이터 부분(직렬화된)으로 확인
	 */
	Packet.ParseFromArray(&Header[1], Size - sizeof(PacketHeader));

	uint64 Id = Packet.id();
	uint32 Hp = Packet.hp();
	uint16 Attack = Packet.attack();

	cout << "Id: " << Id << ", " << "Hp: " << Hp << ", " << "ATT: " << Attack << endl;

	for (int32 i = 0; i < Packet.buffs_size(); ++i)
	{
		const Protocol::BuffData& Data = Packet.buffs(i);
		cout << "Buff Info: " << Data.buffid() << " " << Data.remaintime() << endl;
	}

	// TODO: GameLogic
}
