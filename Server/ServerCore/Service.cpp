#include "pch.h"
#include "Service.h"

ServiceBase::ServiceBase(EServices InType, FInternetAddr InAddr, TSharedPtr<IOCPCore> InCore, SessionFactory InFactory,
	int32 InMaxSessionCount)
		: Type(InType)
		, Addr(InAddr)
		, Core(InCore)
		, MaxSessionCount(InMaxSessionCount)
		, Factory(InFactory)
{
}

ServiceBase::~ServiceBase()
{
}

void ServiceBase::Close()
{
	// TODO
}

TSharedPtr<IOCPSession> ServiceBase::CreateSession()
{
	TSharedPtr<IOCPSession> Session = Factory();
	Session->SetService(shared_from_this());  // �� �ڽ��� ������� ���� ���񽺷� ����ؾ� ��

	if (Core->Register(Session))
	{
		return Session;
	}

	return nullptr;
}

void ServiceBase::AddSession(TSharedPtr<IOCPSession> Session)
{
	LOCK;
	SessionCount++;
	Sessions.insert(Session);
}

void ServiceBase::ReleaseSession(TSharedPtr<IOCPSession> Session)
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
	if (!CanStart())
	{
		return false;
	}

	const int32 SessionCount = GetMaxSessionCount();
	for (int32 i = 0; i < SessionCount; ++i)
	{
		// ��û�� ���� ������ŭ ���� Connect 
		TSharedPtr<IOCPSession> Session = CreateSession();
		if (!Session->Connect())
		{
			return false;
		}
	}

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

	// enable_shared_from_this�� Service�� �پ��־�, shared_from_this�� ȣ���ϸ� Service�� ��ȯ��
	// �̸� ServerService�� ��ȯ���ֱ� ���� static_pointer_cast ���
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
