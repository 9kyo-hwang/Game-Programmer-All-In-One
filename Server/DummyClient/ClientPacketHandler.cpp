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

// ��Ŷ ���� TEMP
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

	// ���� ������(���ڿ�, ����Ʈ �迭(�̹���), ����Ʈ ��)
	vector<BuffData> Data;
};

void ClientPacketHandler::Handle_S_TEST(BYTE* Buffer, int32 Len)
{
	PacketHeader* Header = reinterpret_cast<PacketHeader*>(Buffer);
	uint16 Size = Header->Size;

	Protocol::S_TEST Packet;
	/**
	 * PacketHeader �����ŭ�� �� �ε��� �� ĭ ũ��
	 * [0]: ���� ����� ��� �κ�
	 * [1]~: ���� �����Ͱ� �̾����� �κ�
	 * ���� [1]������ �ּҸ� (��ü ��Ŷ ������ - ��Ŷ ��� ������)��ŭ�� ������ �κ�(����ȭ��)���� Ȯ��
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
