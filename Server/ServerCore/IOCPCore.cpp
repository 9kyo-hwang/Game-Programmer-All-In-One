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
	// Session�� CP�� ����ϴ� ��
	return ::CreateIoCompletionPort(
		Object->GetHandle(),
		Handle,
		0,  // ������Ʈ �ּ� Ű�� ����Ʈ�����ͷ� �����ϸ鼭 ����ϴ� ������ ��� ����
		0
	);
}

bool IOCPCore::Dispatch(uint32 Timeout) const
{
	DWORD NumOfBytes = 0;
	ULONG_PTR CompletionKey = 0;
	IOCPEvent* Event = nullptr;    // OverlappedEx �̰Ϳ� ���� � ������Ʈ���� Ȯ�� ����

	if (::GetQueuedCompletionStatus(
		Handle, 
		OUT &NumOfBytes, 
		OUT &CompletionKey, // �� �̻� Object �ּҸ� Key�� ������� ����
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
