#pragma once

class BufferedWriter
{
public:
	BufferedWriter();
	BufferedWriter(BYTE* InBuffer, uint32 InSize, uint32 InPos = 0);
	~BufferedWriter();

	BYTE* GetBuffer() const { return Buffer; }
	uint32 GetSize() const { return Size; }
	uint32 GetReadSize() const { return Pos; }
	uint32 GetFreeSize() const { return Size - Pos; }

	template<typename T>
	bool Write(T* Src) { return Write(Src, sizeof(T)); }
	bool Write(void* Src, uint32 Len);

	template<typename T>
	T* Reserve();

	template<typename T>
	BufferedWriter& operator<<(T&& Src);

private:
	BYTE* Buffer = nullptr;
	uint32 Size = 0;
	uint32 Pos = 0;
};

template <typename T>
T* BufferedWriter::Reserve()
{
	if (GetFreeSize() < sizeof(T))
	{
		return nullptr;
	}

	T* Result = reinterpret_cast<T*>(&Buffer[Pos]);
	Pos += sizeof(T);
	return Result;
}

template <typename T>
BufferedWriter& BufferedWriter::operator<<(T&& Src)
{
	using DataType = std::remove_reference_t<T>;

	*reinterpret_cast<DataType*>(&Buffer[Pos]) = std::forward<DataType>(Src);
	Pos += sizeof(T);
	return *this;
}
