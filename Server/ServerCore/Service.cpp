#include "pch.h"
#include "Service.h"

Service::Service(EServices InType, FInternetAddr InAddr, TSharedPtr<IOCPCore> InCore, SessionFactory InFactory,
	int32 InMaxSessionCount)
		: Type(InType)
		, Addr(InAddr)
		, Core(InCore)
		, MaxSessionCount(InMaxSessionCount)
		, Factory(InFactory)
{
}

Service::~Service()
{
}

void Service::Close()
{
	// TODO
}

TSharedPtr<IOCPSession> Service::CreateSession()
{
	TSharedPtr<IOCPSession> Session = Factory();
	if (Core->Register(Session))
	{
		return Session;
	}

	return nullptr;
}

void Service::AddSession(TSharedPtr<IOCPSession> Session)
{
	LOCK;
	SessionCount++;
	Sessions.insert(Session);
}

void Service::ReleaseSession(TSharedPtr<IOCPSession> Session)
{
	LOCK;
	assert(Sessions.erase(Session) != 0);  // the number of elements erased
	--SessionCount;
}

ClientService::ClientService(FInternetAddr InAddr, TSharedPtr<IOCPCore> InCore, SessionFactory InFactory, 
	int32 InMaxSessionCount)
	: Super(EServices::Client, InAddr, InCore, InFactory, InMaxSessionCount)
{
}

ClientService::~ClientService()
{
}

bool ClientService::Start()
{
	// TODO
	return true;
}

ServerService::ServerService(FInternetAddr InAddr, TSharedPtr<IOCPCore> InCore, SessionFactory InFactory,
	int32 InMaxSessionCount)
	: Super(EServices::Server, InAddr, InCore, InFactory, InMaxSessionCount)
{
}

ServerService::~ServerService()
{
}

bool ServerService::Start()
{
	if (!CanStart())
	{
		return false;
	}

	Listener = make_shared<IOCPListener>();
	if (!Listener)
	{
		return false;
	}

	// enable_shared_from_this가 Service에 붙어있어, shared_from_this를 호출하면 Service로 반환됨
	// 이를 ServerService로 변환해주기 위해 static_pointer_cast 사용
	TSharedPtr<ServerService> Service = static_pointer_cast<ServerService>(shared_from_this());
	if (!Listener->Accept(Service))
	{
		return false;
	}

	return true;
}

void ServerService::Close()
{
	// TODO
}
