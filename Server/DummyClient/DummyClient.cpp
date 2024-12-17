#include "pch.h"
#include <iostream>

/**
 * 소켓 생성
 * 서버에 연결 요청(TCP only)
 * 통신
 */

int main()
{
	WSADATA SocketData;
	if (::WSAStartup(/*Version Request: 2.2*/MAKEWORD(2, 2), &SocketData))  // 0x202와 동일
	{
		return 0;
	}

	SOCKET ClientSocket = ::socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (ClientSocket == INVALID_SOCKET)
	{
		cout << "Client Socket Create Failed :(" << endl;
		return 0;
	}

	SOCKADDR_IN ServerAddr{};
	{
		ServerAddr.sin_family = AF_INET;
		ServerAddr.sin_port = ::htons(7777);
		//ServerAddress.sin_addr.S_un.S_addr = ::htonl(INADDR_ANY);  Client는 서버 주소로 맞게 접속해야 함
		::inet_pton(AF_INET, "127.0.0.1", &ServerAddr.sin_addr);
	}

	while (true)
	{
		char SendBuffer[100] = "Hello! I am Client!";
		int32 ResultCode = ::sendto(ClientSocket, SendBuffer, sizeof(SendBuffer), 0, reinterpret_cast<SOCKADDR*>(&ServerAddr), sizeof(ServerAddr));
		if (ResultCode == SOCKET_ERROR)
		{
			cout << "Send Client To Server Error!" << endl;
			return 0;
		}

		this_thread::sleep_for(1s);
	}

	::closesocket(ClientSocket);

	return 0;
}
