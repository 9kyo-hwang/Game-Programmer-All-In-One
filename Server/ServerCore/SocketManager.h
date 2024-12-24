#pragma once
#include "IPAddress.h"

class FSocketManager
{
public:
	static LPFN_CONNECTEX ConnectEx;
	static LPFN_DISCONNECTEX DisconnectEx;
	static LPFN_ACCEPTEX AcceptEx;

public:
	static void Initialize();
	static void Clear();

	static bool BindWindowsFunction(SOCKET Socket, GUID Guid, LPVOID* Function);
	static SOCKET CreateSocket();

	static bool SetLinger(SOCKET Socket, bool bShouldLinger, uint16 Timeout);
	static bool SetReuseAddr(SOCKET Socket, bool bAllowReuse);
	static bool SetReceiveBufferSize(SOCKET Socket, int32 Size);
	static bool SetSendBufferSize(SOCKET Socket, int32 Size);
	static bool SetNoDelay(SOCKET Socket, bool bIsNoDelay);
	static bool SetUpdateAcceptContext(SOCKET AcceptSocket, SOCKET ListenSocket);

	static bool Bind(SOCKET Socket, FInternetAddr Addr);
	static bool Bind(SOCKET Socket, SOCKADDR_IN Addr);
	static bool Bind(SOCKET Socket, uint16 Port);
	static bool Listen(SOCKET Socket, int32 MaxBacklog = SOMAXCONN);
	static void Close(SOCKET& Socket);
};

template<typename OptionType>
static bool SetSocketOption(SOCKET Socket, int32 Level, int32 OptionName, OptionType OptionValue)
{
	if (::setsockopt(Socket, Level, OptionName, reinterpret_cast<char*>(&OptionValue), sizeof(OptionType)) == SOCKET_ERROR)
	{
		return false;
	}

	return true;
}