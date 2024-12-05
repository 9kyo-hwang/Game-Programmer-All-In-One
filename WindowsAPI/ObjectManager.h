#pragma once
#include "Singleton.h"

class Object;

class ObjectManager : public Singleton<ObjectManager>
{
public:
	~ObjectManager();

	void Add(Object* NewObject);
	void Remove(Object* TargetObject);
	void Clear();

	const vector<Object*>& GetObjects() { return Objects; }

	template<typename ObjectType>
	ObjectType* NewObject()
	{
		// Type Trait: 템플릿으로 들어온 타입을 Object 타입으로 제한
		static_assert(std::is_convertible_v<ObjectType*, Object*>);

		ObjectType* Object = new ObjectType();
		Object->Initialize();

		return Object;
	}

private:
	vector<Object*> Objects;
};

