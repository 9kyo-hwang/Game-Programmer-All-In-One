#pragma once
#include "IOCPCore.h"
#include "IOCPEvent.h"
#include "IPAddress.h"
#include "RecvBuffer.h"

class SendBuffer;

// Client ��ǥ��
class IOCPSession : public IOCPObject
{
	friend class IOCPListener;
	friend class IOCPCore;
	friend class ServiceBase;

	const int32 BufferSize = 0x10000;  // 64KB

public:
	IOCPSession();
	~IOCPSession() override;

public:  // �ܺο��� ����� ��
	void Send(TSharedPtr<SendBuffer> InSendBuffer);
	bool Connect();
	void Disconnect(const WCHAR* Cause);

	TSharedPtr<ServiceBase> GetService() { return Service.lock(); }
	void SetService(TSharedPtr<ServiceBase> InService) { Service = InService; }

public:  // ����
	FInternetAddr GetIp() const { return Addr; }
	void SetIp(FInternetAddr InAddr) { Addr = InAddr; }
	SOCKET GetSocket() { return Socket; }
	bool IsConnected() { return bConnected; }
	TSharedPtr<IOCPSession> GetSession() { return static_pointer_cast<IOCPSession>(shared_from_this()); }

private:  // Interface ����
	HANDLE GetHandle() override;
	void Dispatch(IOCPEvent* Event, int32 NumOfBytes) override;

private:  // ���� ����
	bool RegisterConnect();		// IOCP Queue�� Connect �̺�Ʈ ���
	bool RegisterDisconnect();  // IOCP Queue�� Disconnect �̺�Ʈ ���
	void RegisterRecv();		// IOCP Queue�� Receive �̺�Ʈ ���
	void RegisterSend();		// IOCP Queue�� Send �̺�Ʈ ���

	void ProcessConnect();
	void ProcessDisconnect();
	void ProcessRecv(int32 NumOfBytes);
	void ProcessSend(int32 NumOfBytes);

	void HandleError(int32 ErrorCode);

protected:  // ��������(GameSession ��)���� �������� �Լ�
	virtual void OnConnected() {}
	virtual int32 OnRecv(BYTE* InBuffer, int32 Len) { return Len; }
	virtual void OnSend(int32 Len) {}
	virtual void OnDisconnected() {}

private:
	TWeakPtr<ServiceBase> Service;
	SOCKET Socket = INVALID_SOCKET;
	FInternetAddr Addr{};  // Client �ּ�
	TAtomic<bool> bConnected = false; 

private:
	MUTEX;

	// Recv ����
	RecvBuffer RecvBuffer;

	// Send ����
	queue<TSharedPtr<SendBuffer>> SendQueue;  // �� Send���� ������ ����� �ٸ��Ƿ�, Queue�� �ְ� �ϳ��� ó��
	TAtomic<bool> bSendRegistered = false;  // Send Event�� 1���ۿ� ���� ������ �����嵵 1������ ���� �� �ֵ��� ����

private:  // ���� �� IOCPEvent�� ��� �ֵ���
	IOCPEvent ConnectEvent{ ENetworkEvents::Connect };
	IOCPEvent DisconnectEvent{ ENetworkEvents::Disconnect };
	/**
	 * ������ ������ �������� �����ϴ� ��� Ŭ���̾�Ʈ�� ���� ������
	 * �׷��� �����ϰ� ���� �� �̺�Ʈ�� �������� ����� ������ �ʿ����,
	 * 1���� �̺�Ʈ�� �ɾ���� ���Ź����� ó���� �� �ٽ� �ɾ��ִ� ������ �ϴ� �����ε� �����
	 */
	IOCPEvent RecvEvent{ ENetworkEvents::Recv };
	IOCPEvent SendEvent{ ENetworkEvents::Send };  // ��� Send�� �ϳ��� ��� �ִ� ���� �� ��
};

struct PacketHeader
{
	uint16 Size;	// ��ŷ�� ������...
	uint16 ID;		// �������� ID(1=�α���, 2=�̵���û, ...)
};

class PacketSession : public IOCPSession
{
public:
	PacketSession();
	~PacketSession() override;

	TSharedPtr<PacketSession> GetPacketSession() { return static_pointer_cast<PacketSession>(shared_from_this()); }

protected:
	int32 OnRecv(BYTE* InBuffer, int32 Len) sealed;  // PacketSession ���ķδ� �� �Լ��� ������� ���ϵ��� ������
	virtual void OnRecvPacket(BYTE* InBuffer, int32 Len) abstract;  // ��Ŷ ������ �Ϸ�� ���� �Ѱ���
};