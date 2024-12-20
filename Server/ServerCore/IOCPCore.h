#pragma once

// 관찰 대상
// 상속받는 모든 오브젝트는 스마트 포인터로 관찰할 수 있음(this 포인터 추출)
class IOCPObject : public TSharedFromThis<IOCPObject>
{
public:
	virtual ~IOCPObject() {}
	virtual HANDLE GetHandle() abstract;
	virtual void Dispatch(struct IOCPEvent* Event, int32 NumOfBytes = 0) abstract;
};

class IOCPCore
{
public:
	IOCPCore();
	~IOCPCore();

	HANDLE GetHandle() const { return Handle; }
	bool Register(TSharedPtr<IOCPObject> Object) const;
	bool Dispatch(uint32 Timeout = INFINITE) const;

private:
	HANDLE Handle;
};