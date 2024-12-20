#pragma once

#include "IOCPCore.h"
#include "IPAddress.h"

// ���� ������
class IOCPListener : public IOCPObject
{
public:
	IOCPListener() = default;
	~IOCPListener() override;

public:
	// �ܺο��� ���
	bool Accept(FInternetAddr Addr);
	void Close();

public:
	HANDLE GetHandle() override;
	void Dispatch(IOCPEvent* Event, int32 NumOfBytes = 0) override;

private:
	void Register(IOCPEvent* Event);
	void Process(IOCPEvent* Event);

protected:
	SOCKET Socket = INVALID_SOCKET;
	vector<IOCPEvent*> Events;
};

