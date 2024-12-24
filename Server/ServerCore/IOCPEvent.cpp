#include "pch.h"
#include "IOCPEvent.h"

IOCPEvent::IOCPEvent(ENetworkEvents InType)
	: Type(InType)
{
	Initialize();
}

void IOCPEvent::Initialize()
{
	hEvent = nullptr;
	Internal = 0;
	InternalHigh = 0;
	Offset = 0;
	OffsetHigh = 0;
}
