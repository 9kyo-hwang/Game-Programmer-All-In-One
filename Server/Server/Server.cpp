#include "pch.h"

/**
 * 소켓 생성
 * 포트 bind
 * 소켓 listen
 * 클라이언트 accept
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
	SOCKET ListenSocket = ::socket(/*IPv4 vs IPv6*/AF_INET, /*TCP vs UDP*/SOCK_STREAM, /*Protocol*/0);
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

	/*
	* 3. Listen
	*/
	if (::listen(ListenSocket, /*backlog. 대기 클라이언트 수*/SOMAXCONN) == SOCKET_ERROR)
	{
		cout << "Socket Listening Failed :(" << endl;
		return 0;
	}

	while (true)
	{
		SOCKADDR_IN ClientAddress{};
		int32 ClientAddressLength = sizeof(ClientAddress);
		
		SOCKET ClientSocket = ::accept(ListenSocket, reinterpret_cast<SOCKADDR*>(&ClientAddress), &ClientAddressLength);
		if (ClientSocket == INVALID_SOCKET)
		{
			cout << "Client Socket is Invalid :(" << endl;
			return 0;
		}

		char Buffer[16];
		::inet_ntop(AF_INET, &ClientAddress.sin_addr, Buffer, sizeof(Buffer));
		cout << "Client Connected! IP = " << Buffer << endl;

		while (true)
		{
			char RecvBuffer[100];
			int32 RecvLength = ::recv(ClientSocket, RecvBuffer, sizeof(RecvBuffer), 0);
			if (RecvLength <= 0)
			{
				return 0;
			}

			cout << "Server Received Data: " << RecvBuffer << endl;
			cout << "Server Received Data Length: " << RecvLength << endl;

			int32 ResultCode = ::send(ClientSocket, RecvBuffer, RecvLength, 0);
			if (ResultCode == SOCKET_ERROR)
			{
				return 0;
			}
		}
	}

	::closesocket(ListenSocket);
	::WSACleanup();

	return 0;
}