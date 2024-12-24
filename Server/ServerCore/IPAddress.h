#pragma once

class FInternetAddr
{
public:
	FInternetAddr() = default;
	FInternetAddr(SOCKADDR_IN InAddr);
	FInternetAddr(wstring InAddr, uint16 InPort);

	SOCKADDR_IN& GetAddr() { return SockAddr; }
	wstring GetIp();
	uint16 GetPort() const { return ::ntohs(SockAddr.sin_port); }

public:
	// string to IN_ADDR
	static IN_ADDR ToAddr(const WCHAR* InAddr);

private:
	SOCKADDR_IN SockAddr;
};

