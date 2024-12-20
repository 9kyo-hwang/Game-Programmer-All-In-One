#include "pch.h"
#include "IOCPSession.h"

IOCPSession::IOCPSession()
{
	// ������ accept �Լ��� ��ȯ���� ���������, �񵿱� ó���̹Ƿ� �̸� ����
	Socket = FSocketManager::CreateSocket();
}

IOCPSession::~IOCPSession()
{
	FSocketManager::Close(Socket);
}

HANDLE IOCPSession::GetHandle()
{
	return reinterpret_cast<HANDLE>(Socket);
}

void IOCPSession::Dispatch(IOCPEvent* Event, int32 NumOfBytes)
{
	// TODO
}
