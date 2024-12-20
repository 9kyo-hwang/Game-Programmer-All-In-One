#include "pch.h"
#include "IOCPCore.h"
#include "IOCPEvent.h"

IOCPCore GIOCPCore;

IOCPCore::IOCPCore()
{
	// IOCP Queue Create
	Handle = ::CreateIoCompletionPort(INVALID_HANDLE_VALUE, nullptr, 0, 0);
	assert(Handle != INVALID_HANDLE_VALUE);
}

IOCPCore::~IOCPCore()
{
	::CloseHandle(Handle);
}

bool IOCPCore::Register(IOCPObject* Object)
{
	// Session�� CP�� ����ϴ� ��
	return ::CreateIoCompletionPort(
		Object->GetHandle(),
		Handle,
		reinterpret_cast<ULONG_PTR>(Object),  // ������Ʈ �ּ� Ű
		0
	);
}

bool IOCPCore::Dispatch(uint32 Timeout)
{
	DWORD NumOfBytes = 0;
	IOCPObject* Object = nullptr;  // Key
	IOCPEvent* Event = nullptr;    // OverlappedEx

	if (::GetQueuedCompletionStatus(
		Handle, 
		OUT &NumOfBytes, 
		OUT reinterpret_cast<PULONG_PTR>(&Object), 
		OUT reinterpret_cast<LPOVERLAPPED*>(&Event), 
		Timeout))
	{
		Object->Dispatch(Event, NumOfBytes);
	}
	else
	{
		switch (::WSAGetLastError())
		{
		case WAIT_TIMEOUT:
			return false;
		default:
			// TODO: Print Log
			Object->Dispatch(Event, NumOfBytes);
			break;
		}
	}

	return true;
}
