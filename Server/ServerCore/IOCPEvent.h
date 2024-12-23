#pragma once

// Overlapped Ex 역할

enum class ENetworkEvents : uint8
{
	Connect,
	Disconnect,
	Accept,
	Recv,
	Send
};

// 상속 구조이므로 virtual 함수를 추가하면 안됨!(메모리 구조 변경됨)
struct IOCPEvent : public OVERLAPPED
{
	IOCPEvent(ENetworkEvents InType);

	void Initialize();

	ENetworkEvents Type;
	TSharedPtr<class IOCPObject> Owner = nullptr;  // Ref Counting을 하기 위해
	TSharedPtr<class IOCPSession> Session = nullptr;  // Accept Only

	vector<BYTE> Buffer;  // Session의 SendEvent를 위한 TEMP
};