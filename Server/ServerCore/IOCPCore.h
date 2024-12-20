#pragma once

// ���� ���
// ��ӹ޴� ��� ������Ʈ�� ����Ʈ �����ͷ� ������ �� ����(this ������ ����)
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