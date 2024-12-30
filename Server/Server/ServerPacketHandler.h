#pragma once

enum
{
	S_TEST = 1,
	S_EnterGame = 2,

	S_LocalPlayer = 4,
	S_SpawnActor = 5,
	S_DestroyActor = 6,
};

struct BuffData
{
	uint16 BuffId;
	float RemainTime;
};

// ������ Packet Generator
class ServerPacketHandler
{
public:
	static void HandlePacket(SessionRef Session, BYTE* Buffer, int32 Len);

	// 패킷 받기

	// 패킷 보내기
	static TSharedPtr<SendBuffer> Make_S_TEST(uint64 Id, uint32 Hp, uint16 Attack, vector<BuffData> Buffs);
	static TSharedPtr<SendBuffer> Make_S_EnterGame();
	static TSharedPtr<SendBuffer> Make_S_LocalPlayer(const Protocol::ObjectInfo& Info);
	static TSharedPtr<SendBuffer> Make_S_SpawnActor(const Protocol::S_SpawnActor& Packet) { return MakeSendBuffer(Packet, S_SpawnActor); }
	static TSharedPtr<SendBuffer> Make_S_DestroyActor(const Protocol::S_DestroyActor& Packet) { return MakeSendBuffer(Packet, S_DestroyActor); }

	template<typename T>
	static TSharedPtr<SendBuffer> MakeSendBuffer(T& Packet, uint16 PacketId);
};

template <typename T>
TSharedPtr<SendBuffer> ServerPacketHandler::MakeSendBuffer(T& Packet, uint16 PacketId)
{
	const uint16 DataSize = static_cast<uint16>(Packet.ByteSizeLong());
	const uint16 PacketSize = DataSize + sizeof(PacketHeader);

	TSharedPtr<SendBuffer> Buffer = make_shared<SendBuffer>(PacketSize);
	PacketHeader* Header = reinterpret_cast<PacketHeader*>(Buffer->GetData());
	Header->Size = PacketSize;
	Header->ID = PacketId;

	assert(Packet.SerializeToArray(&Header[1], DataSize));

	Buffer->Close(PacketSize);

	return Buffer;
}  