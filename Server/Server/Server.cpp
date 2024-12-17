#include "pch.h"

/**
 * 소켓 생성
 * 포트 bind
 * 소켓 listen(TCP only)
 * 클라이언트 accept(TCP only)
 * 통신
 */

int main()
{
	WSADATA SocketData;
	if (::WSAStartup(/*Version Request: 2.2*/0x202, &SocketData))  // MAKEWORD(2, 2)와 동일
	{
		return 0;
	}

	/**
	 * 1. 소켓 생성
	 * af: AF_INET(IPv4), AF_INET6(IPv6)
	 * type: SOCK_STREAM(TCP), SOCK_DGRAM(UDP)
	 * protocol: 0
	 * return: descriptor
	 */
	//int32 ErrorCode = ::WSAGetLastError();
	SOCKET ListenSocket = ::socket(/*IPv4 vs IPv6*/AF_INET, /*TCP vs UDP*/SOCK_DGRAM, /*Protocol*/IPPROTO_UDP);
	if (ListenSocket == INVALID_SOCKET)
	{
		cout << "Listen Socket Create Failed :(" << endl;
		return 0;
	}
	

	/**
	 * 2. 주소/포트 바인딩
	 */
	SOCKADDR_IN ServerAddr{};
	{
		ServerAddr.sin_family = AF_INET;
		ServerAddr.sin_port = ::htons(7777);  // 80: HTTP
		ServerAddr.sin_addr.s_addr = ::htonl(INADDR_ANY);
	};
	if (::bind(ListenSocket, reinterpret_cast<SOCKADDR*>(&ServerAddr), sizeof(ServerAddr)) == SOCKET_ERROR)
	{
		cout << "Listen Socket Binding Failed :(" << endl;
		return 0;
	}

	while (true)
	{
		SOCKADDR_IN ClientAddr{};
		int32 ClientAddrLen = sizeof(ClientAddr);

		char RecvBuffer[100];
		int32 RecvLength = ::recvfrom(ListenSocket, RecvBuffer, sizeof(RecvBuffer), 0, reinterpret_cast<SOCKADDR*>(&ClientAddr), &ClientAddrLen);
		if (RecvLength <= 0)
		{
			return 0;
		}

		cout << "Server Received Data: " << RecvBuffer << endl;
		cout << "Server Received Data Length: " << RecvLength << endl;
	}

	::closesocket(ListenSocket);
	::WSACleanup();

	return 0;
}