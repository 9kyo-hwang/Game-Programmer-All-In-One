#pragma once

class AActor : public TSharedFromThis<AActor>
{
public:
	AActor();
	virtual ~AActor();

	template<typename T>
	static TSharedPtr<T> SpawnActor() requires std::is_base_of_v<AActor, T>
	{
		TSharedPtr<T> Actor = make_shared<T>();
		Actor->Info.set_id(Guid++);
		Actor->Info.set_type(T::Type);

		return Actor;
	}

public:
	Protocol::ObjectInfo Info;
	TSharedPtr<class GameZone> EnteredZone;  // 현재 액터가 있는 존 정보를 들고 있는것이 좋음

private:
	inline static TAtomic<uint64> Guid = 1;  // Globally Unique Identifier
};

