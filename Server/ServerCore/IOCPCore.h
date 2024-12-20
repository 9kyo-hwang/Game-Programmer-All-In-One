#pragma once

// °üÂû ´ë»ó
class IOCPObject
{
public:
	virtual ~IOCPObject(){}
	virtual HANDLE GetHandle() = 0;
	virtual void Dispatch(struct IOCPEvent* Event, int32 NumOfBytes = 0) = 0;
};

class IOCPCore
{
public:
	IOCPCore();
	~IOCPCore();

	HANDLE GetHandle() const { return Handle; }
	bool Register(IOCPObject* Object);
	bool Dispatch(uint32 Timeout = INFINITE);

private:
	HANDLE Handle;
};

// TEMP
extern IOCPCore GIOCPCore;