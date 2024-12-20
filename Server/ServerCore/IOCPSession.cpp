#include "pch.h"
#include "IOCPSession.h"

IOCPSession::IOCPSession()
{
	// 원래는 accept 함수의 반환값을 사용했으나, 비동기 처리이므로 미리 생성
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
