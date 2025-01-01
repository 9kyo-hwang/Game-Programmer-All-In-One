#pragma once

template <typename ClassType>
class Singleton
{
public:
	Singleton() = default;
	virtual ~Singleton() = default;
	Singleton(const Singleton&) = delete;
	Singleton(Singleton&&) = delete;
	Singleton& operator=(const Singleton&) = delete;
	Singleton& operator=(Singleton&&) = delete;

	static ClassType* Get()
	{
		static ClassType Instance;
		return &Instance;
	}
};