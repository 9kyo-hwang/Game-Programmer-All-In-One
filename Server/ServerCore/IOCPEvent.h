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
	TSharedPtr<class IOCPObject> Owner = nullptr;  // Ref Counting�� �ϱ� ����
	TSharedPtr<class IOCPSession> Session = nullptr;  // Accept Only

	vector<BYTE> Buffer;  // Session�� SendEvent�� ���� TEMP
};