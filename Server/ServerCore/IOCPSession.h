#pragma once
#include "IOCPCore.h"
#include "IOCPEvent.h"
#include "IPAddress.h"

// Client 대표자
class IOCPSession : public IOCPObject
{
	friend class IOCPListener;
	friend class IOCPCore;
	friend class ServiceBase;

public:
	IOCPSession();
	~IOCPSession() override;

public:  // 외부에서 사용할 것
	void Send(BYTE* Buffer, int32 Len);
	bool Connect();
	void Disconnect(const WCHAR* Cause);

	TSharedPtr<ServiceBase> GetService() { return Service.lock(); }
	void SetService(TSharedPtr<ServiceBase> InService) { Service = InService; }

public:  // 정보
	FInternetAddr GetIp() const { return Addr; }
	void SetIp(FInternetAddr InAddr) { Addr = InAddr; }
	SOCKET GetSocket() { return Socket; }
	bool IsConnected() { return bIsConnected; }
	TSharedPtr<IOCPSession> GetSession() { return static_pointer_cast<IOCPSession>(shared_from_this()); }

private:  // Interface 구현
	HANDLE GetHandle() override;
	void Dispatch(IOCPEvent* Event, int32 NumOfBytes) override;

private:  // 전송 관련
	bool RegisterConnect();		// IOCP Queue에 Connect 이벤트 등록
	bool RegisterDisconnect();  // IOCP Queue에 Disconnect 이벤트 등록
	void RegisterRecv();		// IOCP Queue에 Receive 이벤트 등록
	void RegisterSend(IOCPEvent* SendEvent);	// IOCP Queue에 Send 이벤트 등록

	void ProcessConnect();
	void ProcessDisconnect();
	void ProcessRecv(int32 NumOfBytes);
	void ProcessSend(IOCPEvent* SendEvent, int32 NumOfBytes);

	void HandleError(int32 ErrorCode);

protected:  // 컨텐츠단(GameSession 등)에서 재정의할 함수
	virtual void OnConnected() {}
	virtual int32 OnRecv(BYTE* Buffer, int32 Len) { return Len; }
	virtual void OnSend(int32 Len) {}
	virtual void OnDisconnected() {}

public:
	//TEMP
	BYTE RecvBuffer[1000]{};

private:
	TWeakPtr<ServiceBase> Service;
	SOCKET Socket = INVALID_SOCKET;
	FInternetAddr Addr{};  // Client 주소
	TAtomic<bool> bIsConnected = false;

private:
	MUTEX;

	// Recv 관련

	// Send 관련

private:  // 세션 당 IOCPEvent를 들고 있도록
	IOCPEvent ConnectEvent{ENetworkEvents::Connect};
	IOCPEvent DisconnectEvent{ENetworkEvents::Disconnect};
	/**
	 * 데이터 수신은 전적으로 전송하는 대상 클라이언트에 의해 결정됨
	 * 그래서 복잡하게 여러 개 이벤트를 동적으로 만들고 관리할 필요없이,
	 * 1개의 이벤트만 걸어놓고 수신받으면 처리한 후 다시 걸어주는 행위를 하는 것으로도 충분함
	 */
	IOCPEvent RecvEvent{ENetworkEvents::Recv};
};
