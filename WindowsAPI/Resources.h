#pragma once

class Resources
{
public:
	Resources();
	virtual ~Resources();

	virtual void Load(const wstring& Path);
	virtual void Save(const wstring& Path);
};

