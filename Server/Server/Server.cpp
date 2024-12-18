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

constexpr int32 BufferSize = 1000;

struct FSession  // 서버에 있는 각자의 클라 정보
{
	SOCKET Socket = INVALID_SOCKET;
	char RecvBuffer[BufferSize]{};  // 각 세션마다 RecvBuffer 필요(클라마다 보낼 데이터가 다르므로)
	int32 RecvBytes = 0;
};

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

	vector<WSAEVENT> WsaEvents;
	vector<FSession> Sessions; Sessions.reserve(100);

	WSAEVENT ListenEvent = ::WSACreateEvent();
	WsaEvents.push_back(ListenEvent);
	Sessions.push_back(FSession{ ListenSocket });  // Event - Session 1:1 매핑을 위해 더미 세션 추가

	// Socket - Event 매핑. Accept, Close 이벤트를 감지할 것
	if (::WSAEventSelect(ListenSocket, ListenEvent, FD_ACCEPT | FD_CLOSE) == SOCKET_ERROR)
	{
		return 0;
	}

	while (true)
	{
		// 이벤트 배열로부터 완료된 이벤트 인덱스를 반환
		int32 Index = ::WSAWaitForMultipleEvents(WsaEvents.size(), &WsaEvents[0], false, WSA_INFINITE, false);
		if (Index == WSA_WAIT_FAILED)
		{
			continue;
		}

		Index -= WSA_WAIT_EVENT_0;  // 보정

		WSANETWORKEVENTS NetworkEvents;  // 발생한 이벤트 가져오기
		if (::WSAEnumNetworkEvents(Sessions[Index].Socket, WsaEvents[Index], &NetworkEvents) == SOCKET_ERROR)
		{
			continue;
		}

		// Accept 이벤트가 발생했다면
		if (NetworkEvents.lNetworkEvents & FD_ACCEPT)
		{
			if (NetworkEvents.iErrorCode[FD_ACCEPT_BIT] != 0)  // Error Check
			{
				continue;
			}

			SOCKADDR_IN ClientAddr{};
			int32 AddrLen = sizeof(ClientAddr);

			SOCKET ClientSocket = ::accept(ListenSocket, reinterpret_cast<SOCKADDR*>(&ClientAddr), &AddrLen);
			if (ClientSocket == INVALID_SOCKET)
			{
				continue;
			}

			cout << "Client Connected" << endl;
			WSAEVENT ClientEvent = WSACreateEvent();
			WsaEvents.push_back(ClientEvent);
			Sessions.push_back(FSession{ ClientSocket });

			// 클라이언트가 연결됐으므로 클라이언트 소켓 - 이벤트 매핑
			if (::WSAEventSelect(ClientSocket, ClientEvent, FD_READ | FD_WRITE | FD_CLOSE) == SOCKET_ERROR)
			{
				return 0;
			}
		}

		// Read 이벤트가 발생했다면 클라이언트 세션 체크
		if (NetworkEvents.lNetworkEvents & FD_READ)
		{
			if (NetworkEvents.iErrorCode[FD_READ_BIT] != 0)  // Error Check
			{
				continue;
			}

			FSession& Session = Sessions[Index];
			int32 RecvLen = ::recv(Session.Socket, Session.RecvBuffer, BufferSize, 0);
			if (RecvLen == SOCKET_ERROR && ::WSAGetLastError() != WSAEWOULDBLOCK)
			{
				if (RecvLen <= 0)
				{
					continue;
				}
			}

			cout << "Receive Data: " << Session.RecvBuffer << endl;
			cout << "Receive Data Len: " << RecvLen << endl;
		}
	}

	FSocketManager::Clear();

	return 0;
}