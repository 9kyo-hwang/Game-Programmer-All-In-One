#pragma once
#include "IOCPCore.h"

// Client ��ǥ��
class IOCPSession : public IOCPObject
{
public:
	IOCPSession();
	~IOCPSession() override;

public:
	FInternetAddr GetIp() const { return Addr; }
	void SetIp(FInternetAddr InAddr) { Addr = InAddr; }
	SOCKET GetSocket() const { return Socket; }

public:  // Interface
	HANDLE GetHandle() override;
	void Dispatch(IOCPEvent* Event, int32 NumOfBytes) override;

public:
	//TEMP
	char RecvBuffer[1000];

private:
	SOCKET Socket = INVALID_SOCKET;
	FInternetAddr Addr{};  // Client �ּ�
	TAtomic<bool> bIsConnected = false;
};

