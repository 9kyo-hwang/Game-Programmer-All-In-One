#include "pch.h"

/**
 * 소켓 생성
 * 포트 bind
 * 소켓 listen(TCP only)
 * 클라이언트 accept(TCP only)
 * 통신
 */

/*
* Select
* 1. Read[] Write[] Except[] 관찰 대상 선정
* 2. ::select(ReadSet, WriteSet, ExceptSet); -> 관찰 시작
* 3. 소켓이 1개라도 준비되면 즉시 리턴, 나머지 소켓은 알아서 제거
* 4. 남은 소켓을 체크한 후 이어서 진행
*/

/*
 * WSAEventSelect
 * 소켓과 관련된 네트워크 이벤트를 [이벤트 객체]로 탐지
 * 생성: WSACreateEvent(Manual Reset + Non-Signaled 상태로 시작)
 * 삭제: WSACloseEvent
 * 이벤트 감지: WSAWaitForMultiplesEvent
 * 이벤트 확인: WSAEnumNetworkEvents
 */

/*
 * Overlapped IO
 * 
 */

constexpr int32 BufferSize = 1000;

struct FSession  // 서버에 있는 각자의 클라 정보
{
	WSAOVERLAPPED Overlapped{};  // 반드시 맨 위에 위치시켜야 함
	// ***
	SOCKET Socket = INVALID_SOCKET;
	char RecvBuffer[BufferSize]{};  // 각 세션마다 RecvBuffer 필요(클라마다 보낼 데이터가 다르므로)
	int32 RecvBytes = 0;
};

void CALLBACK RecvCallback(DWORD Error, DWORD RecvLen, LPWSAOVERLAPPED Overlapped, DWORD Flags)
{
	cout << "Received Data Length Callback = " << RecvLen << endl;

	// TODO

	// Overlapped 포인터를 넘겨받았는데, RecvLen 뿐만 아니라 Session의 나머지 데이터는 어떻게 확인할 수 있는가?
	// Overlapped를 Session의 맨 첫 번째에 위치시켰기 때문에, Session으로 캐스팅해서 데이터를 받아올 수 있음.
	FSession* Session = reinterpret_cast<FSession*>(Overlapped);
}

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

	// 
	/**
	 * Overlapped 함수 등록(WSARecv, WSASend)
	 * - 성공 시 결과를 이용해 처리
	 * - 실패 시 이유 확인
	 */

	/**
	 *
	 */

	while (true)
	{
		SOCKADDR_IN ClientAddr{};
		int32 AddrLen = sizeof(ClientAddr);

		SOCKET ClientSocket;
		while (true)
		{
			ClientSocket = ::accept(ListenSocket, reinterpret_cast<SOCKADDR*>(&ClientAddr), &AddrLen);
			if (ClientSocket != INVALID_SOCKET)
			{
				break;
			}

			if (::WSAGetLastError() != WSAEWOULDBLOCK)
			{
				// 진짜로 문제가 있는 상황
				return 0;
			}
		}

		FSession Session
		{
			.Socket = ClientSocket,
		};

		cout << "Client Connected!" << endl;

		// 데이터 받기
		while (true)
		{
			WSABUF Buffer{};
			Buffer.buf = Session.RecvBuffer;  // 버퍼가 날라가면 "절대 안됨"
			Buffer.len = BufferSize;

			DWORD RecvLen = 0;
			DWORD Flags = 0;

			// 한 번에 버퍼를 여러개 받을 수 있도록 설계돼있음
			// 이제 Callback 함수를 인자로 넘겨줌
			if (::WSARecv(ClientSocket, &Buffer, 1, &RecvLen, &Flags, &Session.Overlapped, RecvCallback) == SOCKET_ERROR)
			{
				// 아직 Pending 상태(Send가 실제로 일어나지 않은 상태)
				if (::WSAGetLastError() == WSA_IO_PENDING)
				{
					// Alertable Wait: 특정 이벤트가 발생하면 일어나자마자 Callback 수행
					::SleepEx(INFINITE, true);

					// C#의 경우 동일하게 콜백 함수를 인자로 받는데, 콜백 함수 실행을 위해 쓰레드 풀에서 별도의 쓰레드를 할당함
					// C++은 별도의 쓰레드를 할당하지 않고, APC Queue에 의해 SleepEx -> Callback -> 복귀
				}
				else
				{
					// 문제가 있음
					break;
				}
			}

			//cout << "Data Received: " << Session.RecvBuffer << endl;
		}
	}

	FSocketManager::Clear();

	return 0;
}