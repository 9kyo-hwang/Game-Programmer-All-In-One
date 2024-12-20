#include "pch.h"
#include "IOCPCore.h"
#include "IOCPEvent.h"

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

bool IOCPCore::Register(TSharedPtr<IOCPObject> Object) const
{
	// Session을 CP에 등록하는 것
	return ::CreateIoCompletionPort(
		Object->GetHandle(),
		Handle,
		0,  // 오브젝트 주소 키는 스마트포인터로 변경하면서 사용하는 쪽으로 방식 변경
		0
	);
}

bool IOCPCore::Dispatch(uint32 Timeout) const
{
	DWORD NumOfBytes = 0;
	ULONG_PTR CompletionKey = 0;
	IOCPEvent* Event = nullptr;    // OverlappedEx 이것에 의해 어떤 오브젝트인지 확인 가능

	if (::GetQueuedCompletionStatus(
		Handle, 
		OUT &NumOfBytes, 
		OUT &CompletionKey, // 더 이상 Object 주소를 Key로 사용하지 않음
		OUT reinterpret_cast<LPOVERLAPPED*>(&Event), 
		Timeout))
	{
		TSharedPtr<IOCPObject> Object = Event->Owner;
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
			TSharedPtr<IOCPObject> Object = Event->Owner;
			Object->Dispatch(Event, NumOfBytes);
			break;
		}
	}

	return true;
}
