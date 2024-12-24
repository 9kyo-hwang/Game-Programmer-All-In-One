#pragma once

enum
{
	S_TEST = 1  // �׽�Ʈ ��Ŷ
};

struct BufferData
{
	uint16 BufferID;
	float RemainTime;
};

// ������ Packet Generator
class ServerPacketHandler
{
public:
	static void HandlePacket(BYTE* Buffer, int32 Len);
	static TSharedPtr<SendBuffer> Make_S_TEST(uint64 Id, uint32 Hp, uint16 Attack, vector<BufferData> Buffers);
};

