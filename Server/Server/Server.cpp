#include "pch.h"

#include "ThreadManager.h"

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
	SOCKET Socket = INVALID_SOCKET;
	char RecvBuffer[BufferSize]{};  // 각 세션마다 RecvBuffer 필요(클라마다 보낼 데이터가 다르므로)
	int32 RecvBytes = 0;
};

enum class EIOType
{
	Read,
	Write,
	Accept,
	Connect
};

struct FOverlappedEx
{
	WSAOVERLAPPED Overlapped{};  // 반드시 첫 번째 위치
	EIOType IOType;  // 이벤트 구분을 위해 Overlapped를 확장함

	// TODO 
};

void WorkerThreadMain(HANDLE IOCPHandle)
{
	while (true)
	{
		// TODO

		// WorkerThread들은 Queue를 확인해서 완료된 이벤트가 있다면 그 중 하나가 처리
		DWORD OutBytesTransferred = 0;
		FSession* OutSession = nullptr;
		FOverlappedEx* OutOverlappedEx = nullptr;

		// MainThread에서 CreateIoCompletionPort를 호출할 때 세션 주소를, WSARecv를 호출할 때 Overlapped 주소를 넘겨줌
		// 이를 여기서 다시 반환받아 데이터를 확인할 수 있는 것
		if (!::GetQueuedCompletionStatus(IOCPHandle, &OutBytesTransferred, 
			reinterpret_cast<ULONG_PTR*>(&OutSession), reinterpret_cast<LPOVERLAPPED*>(&OutOverlappedEx), INFINITE))
		{
			continue;
		}

		if (OutBytesTransferred == 0)
		{
			continue;
		}

		cout << "Received Data Len: " << OutBytesTransferred << endl;
		cout << "Received Data IOCP: " << OutSession->RecvBuffer << endl;

		WSABUF Buffer
		{
			.len = BufferSize,
			.buf = OutSession->RecvBuffer,
		};

		DWORD RecvLen = 0;
		DWORD Flags = 0;
		::WSARecv(OutSession->Socket, &Buffer, 1, &RecvLen, &Flags, /*중요!*/&OutOverlappedEx->Overlapped, nullptr);

		switch (OutOverlappedEx->IOType)
		{
		case EIOType::Read:
			break;
		case EIOType::Write:
			break;
		case EIOType::Accept:
			break;
		case EIOType::Connect:
			break;
		}
	}
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
#if 0  // IOCP 방식을 사용하면 내부적으로 넌블로킹 처리를 해줌
	u_long Mode = 1;  // Non-blocking Socket
	if (::ioctlsocket(ListenSocket, FIONBIO, &Mode) == INVALID_SOCKET)
	{
		return 0;
	}
#endif

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
	 *
	 * 각 쓰레드마다 APC Queue를 들고 있음
	 */

	/**
	 * Overlapped는 각 쓰레드마다 APC Queue가 존재해, 쓰레드 간 일을 분배할 때 부적절
	 * IOCP는 WSARead/Send 등 네트워크 이벤트에 대한 결과를 통일된 Queue에 저장
	 * 완료된 이벤트는 Worker Thread가 Queue에서 꺼내 처리
	 * 아래 함수를 통해 Queue를 생성
	 */
	HANDLE IOCPHandle = ::CreateIoCompletionPort(INVALID_HANDLE_VALUE, nullptr, 0, 0);

	// Worker Threads 생성
	for (int32 i = 0; i < 5; ++i)
	{
		// 네트워크 이벤트가 발생해 완료된 일감은 WorkerThread들의 함수에서 별도로 처리
		GThreadManager->Launch([=]
			{
				WorkerThreadMain(IOCPHandle);
			});
	}

	vector<FSession*> SessionManager;

	// Main Thread는 Accept만 담당
	while (true)
	{
		SOCKADDR_IN ClientAddr{};
		int32 AddrLen = sizeof(ClientAddr);

		// 아직까지는 블록킹
		SOCKET ClientSocket = ::accept(ListenSocket, reinterpret_cast<SOCKADDR*>(&ClientAddr), &AddrLen);
		if (ClientSocket == INVALID_SOCKET)
		{
			return 0;
		}

		FSession* Session = new FSession();
		{
			Session->Socket = ClientSocket;
		}
		SessionManager.push_back(Session);

		cout << "Client connected!" << endl;

		/**
		 * Main Thread는 Completion Port Queue에 등록하는 역할만 함
		 * 등록할 소켓, 등록될 큐(IOCP handle), 세션을 구분할 수 있는 고유키,
		 * 만약에 해당 세션(클라이언트)이 Queue에 들어간 채로 종료된다면? WorkerThread에서 꺼내쓸 때는 "허상 포인터" -> 반드시 스마트 포인터
		*/
		::CreateIoCompletionPort(reinterpret_cast<HANDLE>(ClientSocket), IOCPHandle, /*Session Key*/reinterpret_cast<ULONG_PTR>(Session), 0);

		WSABUF Buffer
		{
			.len = BufferSize,
			.buf = Session->RecvBuffer,
		};

		FOverlappedEx* OverlappedEx = new FOverlappedEx();
		OverlappedEx->IOType = EIOType::Read;

		// Recv만 수행, 이벤트 완료에 대한 처리는 WorkerThread에 일임
		DWORD RecvLen = 0;
		DWORD Flags = 0;
		::WSARecv(ClientSocket, &Buffer, 1, &RecvLen, &Flags, /*Overlapped Pointer*/&OverlappedEx->Overlapped, nullptr);
	}

	FSocketManager::Clear();

	return 0;
}