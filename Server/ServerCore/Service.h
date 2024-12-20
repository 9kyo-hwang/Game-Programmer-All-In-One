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
 * make_shared<IOCPSession>()을 하지 않고 Factory를 사용하는 이유?
 * 컨텐츠단에서 원하는 코드를 엔진단 코드에 추가할 수 없음(GameSession을 엔진단 코드에 추가할 수 없음)
 * 대신 컨텐츠단에서 자신이 원하는 대로 작성한 콜백함수를 넘겨주는 식으로 해결
 */
using SessionFactory = function<TSharedPtr<IOCPSession>(void)>;

// Smart Pointer를 this로 넘길 수 있도록
class Service : public TSharedFromThis<Service>
{
public:
	Service(EServices InType, FInternetAddr InAddr, TSharedPtr<IOCPCore> InCore, SessionFactory InFactory, int32 InMaxSessionCount = 1);
	virtual ~Service();

	virtual bool Start() abstract;  // Client, Server에 따라 다르게 구현
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

	unordered_set<TSharedPtr<IOCPSession>> Sessions;  // Ref Count 기반이므로, 여기에 들어있으면 최소한 Count = 1은 보장
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