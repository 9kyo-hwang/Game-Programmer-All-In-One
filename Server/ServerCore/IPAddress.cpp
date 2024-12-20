#include "pch.h"
#include "IPAddress.h"

FInternetAddr::FInternetAddr(SOCKADDR_IN InAddr)
	: SockAddr(InAddr)
{
}

FInternetAddr::FInternetAddr(wstring InAddr, uint16 InPort)
{
	::memset(&SockAddr, 0, sizeof(SockAddr));
	SockAddr.sin_family = AF_INET;
	SockAddr.sin_addr = ToAddr(InAddr.c_str());
	SockAddr.sin_port = ::htons(InPort);
}

wstring FInternetAddr::GetIp()
{
	WCHAR Buffer[100]{};
	::InetNtopW(AF_INET, &SockAddr.sin_addr, Buffer, Len32(Buffer));
	return wstring(Buffer);
}

IN_ADDR FInternetAddr::ToAddr(const WCHAR* InAddr)
{
	IN_ADDR Addr{};
	::InetPtonW(AF_INET, InAddr, &Addr);
	return Addr;
}
