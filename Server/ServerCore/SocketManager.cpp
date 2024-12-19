#include "pch.h"
#include "SocketManager.h"

LPFN_CONNECTEX FSocketManager::ConnectEx = nullptr;
LPFN_DISCONNECTEX FSocketManager::DisconnectEx = nullptr;
LPFN_ACCEPTEX FSocketManager::AcceptEx = nullptr;

void FSocketManager::Initialize()
{
	WSADATA OutWsaData;
	assert(::WSAStartup(MAKEWORD(2, 2), &OutWsaData) == 0);

	// 런타임에 주소를 얻어오는 API
	SOCKET DummySocket = CreateSocket();
	assert(BindWindowsFunction(DummySocket, WSAID_CONNECTEX, reinterpret_cast<LPVOID*>(&ConnectEx)));
	assert(BindWindowsFunction(DummySocket, WSAID_DISCONNECTEX, reinterpret_cast<LPVOID*>(&DisconnectEx)));
	assert(BindWindowsFunction(DummySocket, WSAID_ACCEPTEX, reinterpret_cast<LPVOID*>(&AcceptEx)));

	Close(DummySocket);
}

void FSocketManager::Clear()
{
	::WSACleanup();
}

bool FSocketManager::BindWindowsFunction(SOCKET Socket, GUID Guid, LPVOID* Function)
{
	DWORD OutBytes = 0;
	return SOCKET_ERROR != ::WSAIoctl(Socket, SIO_GET_EXTENSION_FUNCTION_POINTER,
		&Guid, sizeof(Guid),
		Function, sizeof(*Function),
		&OutBytes, nullptr, nullptr);
}

SOCKET FSocketManager::CreateSocket()
{
	return ::WSASocket(
		AF_INET,
		SOCK_STREAM,
		IPPROTO_TCP,
		nullptr,
		0,
		WSA_FLAG_OVERLAPPED
	);
}

bool FSocketManager::SetLinger(SOCKET Socket, bool bShouldLinger, uint16 Timeout)
{
	LINGER OptionValue
	{
		.l_onoff = bShouldLinger,
		.l_linger = Timeout
	};

	return SetSocketOption(Socket, SOL_SOCKET, SO_LINGER, OptionValue);
}

bool FSocketManager::SetReuseAddr(SOCKET Socket, bool bAllowReuse)
{
	return SetSocketOption(Socket, SOL_SOCKET, SO_REUSEADDR, bAllowReuse);
}

bool FSocketManager::SetReceiveBufferSize(SOCKET Socket, int32 Size)
{
	return SetSocketOption(Socket, SOL_SOCKET, SO_RCVBUF, Size);
}

bool FSocketManager::SetSendBufferSize(SOCKET Socket, int32 Size)
{
	return SetSocketOption(Socket, SOL_SOCKET, SO_SNDBUF, Size);
}

bool FSocketManager::SetNoDelay(SOCKET Socket, bool bIsNoDelay)
{
	return SetSocketOption(Socket, SOL_SOCKET, TCP_NODELAY, bIsNoDelay);
}

bool FSocketManager::SetUpdateAcceptContext(SOCKET AcceptSocket, SOCKET ListenSocket)
{
	return SetSocketOption(AcceptSocket, SOL_SOCKET, SO_UPDATE_ACCEPT_CONTEXT, ListenSocket);
}

bool FSocketManager::Bind(SOCKET Socket, SOCKADDR_IN Addr)
{
	return SOCKET_ERROR != ::bind(Socket, reinterpret_cast<SOCKADDR*>(&Addr), sizeof(SOCKADDR_IN));
}

bool FSocketManager::Bind(SOCKET Socket, uint16 Port)
{
	SOCKADDR_IN Address;
	Address.sin_family = AF_INET;
	Address.sin_port = ::htons(Port);
	Address.sin_addr.s_addr = ::htonl(INADDR_ANY);

	return SOCKET_ERROR != ::bind(Socket, reinterpret_cast<const SOCKADDR*>(&Address), sizeof(Address));
}

bool FSocketManager::Listen(SOCKET Socket, int32 MaxBacklog)
{
	return SOCKET_ERROR != ::listen(Socket, MaxBacklog);
}

void FSocketManager::Close(SOCKET& Socket)
{
	if (Socket != INVALID_SOCKET)
	{
		::closesocket(Socket);
	}

	Socket = INVALID_SOCKET;
}
