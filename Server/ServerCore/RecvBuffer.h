#pragma once

class RecvBuffer
{
public:
	RecvBuffer(int32 NewSize);
	~RecvBuffer();

	void Clean();
	bool OnRead(int32 NumOfBytes);
	bool OnWrite(int32 NumOfBytes);

	BYTE* GetReadPos() { return &Buffer[Tail]; }
	BYTE* GetWritePos() { return &Buffer[Head]; }
	int32 GetDataSize() const { return Head - Tail; }
	int32 GetFreeSize() const { return Count - Head; }

private:
	const int32 BufferCount = 10;  // �˳��� ����� �α� ����

	int32 Count = 0;
	int32 Size = 0;
	int32 Head = 0;  // Write
	int32 Tail = 0;  // Read
	vector<BYTE> Buffer;
};

