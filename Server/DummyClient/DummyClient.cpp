#include "pch.h"

/**
 * 소켓 생성
 * 서버에 연결 요청(TCP only)
 * 통신
 */

int main()
{
	FSocketManager::Initialize();

	SOCKET ClientSocket = ::socket(AF_INET, SOCK_STREAM, 0);
	if (ClientSocket == INVALID_SOCKET)
	{
		return 0;
	}

	u_long Mode = 1;  // Non-blocking Socket
	if (::ioctlsocket(ClientSocket, FIONBIO, &Mode) == INVALID_SOCKET)
	{
		return 0;
	}

	SOCKADDR_IN ServerAddr{};
	ServerAddr.sin_family = AF_INET;
	::inet_pton(AF_INET, "127.0.0.1", &ServerAddr.sin_addr);
	ServerAddr.sin_port = ::htons(7777);

	while (true)
	{
		if (::connect(ClientSocket, reinterpret_cast<SOCKADDR*>(&ServerAddr), sizeof(ServerAddr)) == SOCKET_ERROR)
		{
			if (::WSAGetLastError() == WSAEWOULDBLOCK)
			{
				cout << "아직 서버에 접속하지 못했습니다." << endl;
				continue;
			}

			if (::WSAGetLastError() == WSAEISCONN)
			{
				cout << "이미 연결된 상태입니다." << endl;
				break;
			}
		}
	}

	while (true)
	{
		char SendBuffer[100] = "Hello, I am a Client!";
		int32 SendLen = sizeof(SendBuffer);

		if (::send(ClientSocket, SendBuffer, SendLen, 0) == SOCKET_ERROR)
		{
			if (::WSAGetLastError() == WSAEWOULDBLOCK)
			{
				cout << "넌블록킹...?" << endl;
				continue;
			}
		}

		cout << "전송한 데이터 길이: " << SendLen << endl;
		this_thread::sleep_for(1s);
	}

	FSocketManager::Clear();

	return 0;
}
