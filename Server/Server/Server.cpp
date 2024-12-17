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
	FSocketManager::Initialize();

	SOCKET ListenSocket = ::socket(AF_INET, SOCK_STREAM, 0);
	if (ListenSocket == INVALID_SOCKET)
	{
		return 0;
	}

	// https://learn.microsoft.com/ko-kr/windows/win32/api/winsock/nf-winsock-ioctlsocket
	u_long Mode = 1;  // Non-blocking Socket
	if (::ioctlsocket(ListenSocket, FIONBIO, &Mode) == INVALID_SOCKET)
	{
		return 0;
	}

	FSocketManager::SetReuseAddr(ListenSocket, true);
	if (!FSocketManager::Bind(ListenSocket, 7777))
	{
		return 0;
	}

	if (!FSocketManager::Listen(ListenSocket))
	{
		return 0;
	}

	SOCKADDR_IN ClientAddr;
	int32 AddrLen = sizeof(ClientAddr);

	while (true)
	{
		SOCKET ClientSocket = ::accept(ListenSocket, reinterpret_cast<SOCKADDR*>(&ClientAddr), &AddrLen);
		if (ClientSocket == INVALID_SOCKET)
		{
			if (::WSAGetLastError() == WSAEWOULDBLOCK)
			{
				// https://learn.microsoft.com/ko-kr/windows/win32/winsock/windows-sockets-error-codes-2
				cout << "접속한 클라이언트가 없습니다." << endl;
				continue;
			}
		}

		cout << "클라이언트 접속!" << endl;

		while (true)
		{
			char RecvBuffer[1000];
			int32 RecvLen = ::recv(ClientSocket, RecvBuffer, sizeof(RecvBuffer), 0);
			if (RecvLen == SOCKET_ERROR)
			{
				if (::WSAGetLastError() == WSAEWOULDBLOCK)
				{
					cout << "클라이언트가 아직 데이터를 전송하지 않았습니다." << endl;
					continue;
				}

				break;
			}

			cout << "전달받은 데이터: " << RecvBuffer << endl;
			cout << "데이터 길이: " << RecvLen << endl;
		}
	}

	FSocketManager::Clear();

	return 0;
}