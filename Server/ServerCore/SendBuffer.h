#pragma once

class SendBuffer : TSharedFromThis<SendBuffer>
{
public:
	SendBuffer(int32 NewSize);
	~SendBuffer();

	BYTE* GetData() { return Buffer.data(); }
	int32 Num() const { return WriteSize; }
	int32 Max() const { return static_cast<int32>(Buffer.size()); }

	void Copy(void* Data, int32 Len);

private:
	vector<BYTE> Buffer;
	int32 WriteSize = 0;
};

