#pragma once
#include <functional>
#include "IOCPCore.h"
#include "IOCPSession.h"
#include "Listener.h"

enum class EServices : uint8
{
	Server,
	Client,
};

/**
 * make_shared<IOCPSession>()�� ���� �ʰ� Factory�� ����ϴ� ����?
 * �������ܿ��� ���ϴ� �ڵ带 ������ �ڵ忡 �߰��� �� ����(GameSession�� ������ �ڵ忡 �߰��� �� ����)
 * ��� �������ܿ��� �ڽ��� ���ϴ� ��� �ۼ��� �ݹ��Լ��� �Ѱ��ִ� ������ �ذ�
 */
using SessionFactory = function<TSharedPtr<IOCPSession>(void)>;

// Smart Pointer�� this�� �ѱ� �� �ֵ���
class Service : public TSharedFromThis<Service>
{
public:
	Service(EServices InType, FInternetAddr InAddr, TSharedPtr<IOCPCore> InCore, SessionFactory InFactory, int32 InMaxSessionCount = 1);
	virtual ~Service();

	virtual bool Start() abstract;  // Client, Server�� ���� �ٸ��� ����
	bool CanStart() const { return Factory != nullptr; }
	virtual void Close();
	void SetSessionFactory(SessionFactory Func) { Factory = Func; }

	TSharedPtr<IOCPSession> CreateSession();
	void AddSession(TSharedPtr<IOCPSession> Session);
	void ReleaseSession(TSharedPtr<IOCPSession> Session);
	int32 GetCurrentSessionCount() const { return SessionCount; }
	int32 GetMaxSessionCount() const { return MaxSessionCount; }

public:
	EServices GetType() const { return Type; }
	FInternetAddr GetAddr() const { return Addr; }
	TSharedPtr<IOCPCore>& GetCore() { return Core; }

protected:
	MUTEX;
	
	EServices Type;
	FInternetAddr Addr{};
	TSharedPtr<IOCPCore> Core;

	unordered_set<TSharedPtr<IOCPSession>> Sessions;  // Ref Count ����̹Ƿ�, ���⿡ ��������� �ּ��� Count = 1�� ����
	int32 SessionCount = 0;
	int32 MaxSessionCount = 0;
	SessionFactory Factory;
};

class ClientService : public Service
{
	using Super = Service;

public:
	ClientService(FInternetAddr InAddr, TSharedPtr<IOCPCore> InCore, SessionFactory InFactory, int32 InMaxSessionCount = 1);
	~ClientService() override;

	bool Start() override;
};

class ServerService : public Service
{
	using Super = Service;

public:
	ServerService(FInternetAddr InAddr, TSharedPtr<IOCPCore> InCore, SessionFactory InFactory, int32 InMaxSessionCount = 1);
	~ServerService() override;

	bool Start() override;
	void Close() override;

private:
	TSharedPtr<IOCPListener> Listener = nullptr;
};