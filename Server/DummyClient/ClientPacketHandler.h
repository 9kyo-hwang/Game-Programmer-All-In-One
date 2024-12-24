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
};

