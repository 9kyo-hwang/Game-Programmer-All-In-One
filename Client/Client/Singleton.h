#pragma once

template <typename ClassType>
class Singleton
{
public:
	static ClassType* Get()
	{
		static ClassType Instance;
		return &Instance;
	}
};