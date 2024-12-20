#pragma once

// Overlapped Ex ����

enum class ENetworkEvents : uint8
{
	Connect,
	Disconnect,
	Accept,
	Recv,
	Send
};

// ��� �����̹Ƿ� virtual �Լ��� �߰��ϸ� �ȵ�!(�޸� ���� �����)
struct IOCPEvent : public OVERLAPPED
{
	IOCPEvent(ENetworkEvents InType);

	void Initialize();

	ENetworkEvents Type;
	class IOCPSession* Session = nullptr;  // Accept Only
};