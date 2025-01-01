#pragma once

// 보안 상의 이유로 패킷 번호는 주기적으로 변경해줘야 함
enum
{
	S_TEST = 1,
	S_EnterGame = 2,

	S_LocalPlayer = 4,
	S_SpawnActor = 5,
	S_DestroyActor = 6,

	C_Move = 10,
	S_Move = 11,
};

class ClientPacketHandler
{
public:
	static void HandlePacket(SessionRef Session, BYTE* Buffer, int32 Len);

	static void Incoming_S_TEST(SessionRef Session, BYTE* Buffer, int32 Len);
	static void Incoming_S_EnterGame(SessionRef Session, BYTE* Buffer, int32 Len);

	static void Incoming_S_LocalPlayer(SessionRef Session, BYTE* Buffer, int32 Len);
	static void Incoming_S_SpawnActor(SessionRef Session, BYTE* Buffer, int32 Len);
	static void Incoming_S_DestroyActor(SessionRef Session, BYTE* Buffer, int32 Len);
	static void Incoming_S_Move(SessionRef Session, BYTE* Buffer, int32 Len);

	static TSharedPtr<SendBuffer> Outgoing_C_Move();

	template<typename T>
	static TSharedPtr<SendBuffer> MakeSendBuffer(T& Packet, uint16 PacketId);
};

template <typename T>
TSharedPtr<SendBuffer> ClientPacketHandler::MakeSendBuffer(T& Packet, uint16 PacketId)
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

