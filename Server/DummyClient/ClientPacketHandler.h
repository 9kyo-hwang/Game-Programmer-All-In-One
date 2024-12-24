#pragma once

enum
{
	S_TEST = 1
};

class ClientPacketHandler
{
public:
	static void HandlePacket(BYTE* Buffer, int32 Len);
	static void Handle_S_TEST(BYTE* Buffer, int32 Len);

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

