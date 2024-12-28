#include "pch.h"
#include "ClientPacketHandler.h"
#include "BufferReader.h"
#include "ServerSession.h"

void ClientPacketHandler::HandlePacket(SessionRef Session, BYTE* Buffer, int32 Len)
{
	BufferedReader Reader(Buffer, Len);
	PacketHeader Header;

	Reader >> Header;

	switch (Header.ID)
	{
	case S_TEST:
		Handle_S_TEST(Session, Buffer, Len);
		break;
	case S_EnterGame:
		Handle_S_EnterGame(Session, Buffer, Len);
	default:
		break;
	}
}

// 패킷 설계 TEMP
struct BuffData
{
	uint64 BuffId;
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

void ClientPacketHandler::Handle_S_TEST(SessionRef Session, BYTE* Buffer, int32 Len)
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

void ClientPacketHandler::Handle_S_EnterGame(SessionRef Session, BYTE* Buffer, int32 Len)
{
	// 반복되는 파트를 따로 떼버리거나 툴로 만들면 참 좋을텐데...
	PacketHeader* Header = reinterpret_cast<PacketHeader*>(Buffer);
	uint16 Size = Header->Size;

	Protocol::S_EnterGame Packet;
	Packet.ParseFromArray(&Header[1], Size - sizeof(PacketHeader));

	bool bSuccess = Packet.success();
	uint64 AccountId = Packet.accountid();

	// 만약 서버에 즉시 회신하고 싶다면 아래와 같이 수행
	// Session->Send()
}
