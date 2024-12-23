#pragma once
#include "IOCPCore.h"
#include "IOCPEvent.h"
#include "IPAddress.h"

// Client ��ǥ��
class IOCPSession : public IOCPObject
{
	friend class IOCPListener;
	friend class IOCPCore;
	friend class ServiceBase;

public:
	IOCPSession();
	~IOCPSession() override;

public:  // �ܺο��� ����� ��
	void Send(BYTE* Buffer, int32 Len);
	bool Connect();
	void Disconnect(const WCHAR* Cause);

	TSharedPtr<ServiceBase> GetService() { return Service.lock(); }
	void SetService(TSharedPtr<ServiceBase> InService) { Service = InService; }

public:  // ����
	FInternetAddr GetIp() const { return Addr; }
	void SetIp(FInternetAddr InAddr) { Addr = InAddr; }
	SOCKET GetSocket() { return Socket; }
	bool IsConnected() { return bIsConnected; }
	TSharedPtr<IOCPSession> GetSession() { return static_pointer_cast<IOCPSession>(shared_from_this()); }

private:  // Interface ����
	HANDLE GetHandle() override;
	void Dispatch(IOCPEvent* Event, int32 NumOfBytes) override;

private:  // ���� ����
	bool RegisterConnect();		// IOCP Queue�� Connect �̺�Ʈ ���
	bool RegisterDisconnect();  // IOCP Queue�� Disconnect �̺�Ʈ ���
	void RegisterRecv();		// IOCP Queue�� Receive �̺�Ʈ ���
	void RegisterSend(IOCPEvent* SendEvent);	// IOCP Queue�� Send �̺�Ʈ ���

	void ProcessConnect();
	void ProcessDisconnect();
	void ProcessRecv(int32 NumOfBytes);
	void ProcessSend(IOCPEvent* SendEvent, int32 NumOfBytes);

	void HandleError(int32 ErrorCode);

protected:  // ��������(GameSession ��)���� �������� �Լ�
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
	FInternetAddr Addr{};  // Client �ּ�
	TAtomic<bool> bIsConnected = false;

private:
	MUTEX;

	// Recv ����

	// Send ����

private:  // ���� �� IOCPEvent�� ��� �ֵ���
	IOCPEvent ConnectEvent{ENetworkEvents::Connect};
	IOCPEvent DisconnectEvent{ENetworkEvents::Disconnect};
	/**
	 * ������ ������ �������� �����ϴ� ��� Ŭ���̾�Ʈ�� ���� ������
	 * �׷��� �����ϰ� ���� �� �̺�Ʈ�� �������� ����� ������ �ʿ����,
	 * 1���� �̺�Ʈ�� �ɾ���� ���Ź����� ó���� �� �ٽ� �ɾ��ִ� ������ �ϴ� �����ε� �����
	 */
	IOCPEvent RecvEvent{ENetworkEvents::Recv};
};
