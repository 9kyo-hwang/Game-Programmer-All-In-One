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
struct BufferData
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
	vector<BufferData> Data;
};

void ClientPacketHandler::Handle_S_TEST(BYTE* Buffer, int32 Len)
{
	BufferedReader Reader(Buffer, Len);
	PacketHeader Header;
	Reader >> Header;

	uint64 Id;
	uint32 Hp;
	uint16 Attack;
	Reader >> Id >> Hp >> Attack;  // �־��� ���� �״��!

	cout << "[Id, Hp, Attack]: " << Id << " " << Hp << " " << Attack << endl;

	uint16 Count;
	Reader >> Count;

	vector<BufferData> Data;
	Data.resize(Count);

	for (int32 i = 0; i < Count; ++i)
	{
		Reader >> Data[i].BufferID >> Data[i].RemainTime;
	}

	// TODO: GameLogic
}
