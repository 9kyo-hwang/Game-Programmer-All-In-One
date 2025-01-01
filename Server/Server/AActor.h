#pragma once
#include "../Client/Client/TimerManager.h"

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

	virtual void Initialize() {}
	virtual void Update() {}

public:
	void TransitionTo(EObjectStates NewState, bool Broadcast = false);
	void RotateTo(EMovementDirection NewDirection, bool Broadcast = false);
	bool CanMoveTo(Vector2Int Dest);
	void MoveTo(Vector2Int Dest, bool Broadcast = false);

	EObjectStates GetCurrentState() const { return Info.state(); }
	EMovementDirection GetCurrentDirection() const { return Info.direction(); }
	Vector2Int GetCellPosition() const { return Vector2Int(Info.posx(), Info.posy()); }
	Vector2Int GetFrontCell() const;
	EMovementDirection FindLookAtDirection(Vector2Int TargetPosition) const;

	void BroadcastMove() const;  // Client의 SynchronizeToServer에 대응되는 메서드

public:
	Protocol::ObjectInfo Info;
	TSharedPtr<class GameZone> EnteredZone;  // 현재 액터가 있는 존 정보를 들고 있는것이 좋음

protected:
	static const Vector2Int Offset[4];

private:
	inline static TAtomic<uint64> Guid = 1;  // Globally Unique Identifier
};

