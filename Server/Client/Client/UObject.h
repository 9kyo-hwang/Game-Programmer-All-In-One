#pragma once
#include "FlipbookActor.h"

class UObject : public AFlipbook
{
	using Super = AFlipbook;

public:
	UObject();
	~UObject() override;

	void BeginPlay() override;
	void Tick(float DeltaTime) override;
	void Render(HDC DeviceContextHandle) override;

protected:
	virtual void OnTickIdle(float DeltaTime) {}
	virtual void OnTickMove(float DeltaTime) {}
	virtual void OnTickAttack(float DeltaTime) {}
	virtual void UpdateAnimation() {}

public:
	void TransitionTo(EObjectStates NewState);
	void RotateTo(EMovementDirection NewDirection);
	bool HasReachedDest() const;
	bool CanMoveTo(Vector2Int Dest);
	void MoveTo(Vector2Int Dest, bool bTeleport = false);

	EObjectStates GetCurrentState() { return Info.state(); }
	EMovementDirection GetCurrentDirection() { return Info.direction(); }
	Vector2Int GetCellPosition() const { return Vector2Int(Info.posx(), Info.posy()); }
	Vector2Int GetFrontCell() const;
	EMovementDirection FindLookAtDirection(Vector2Int TargetPosition) const;

protected:
	static const Vector2Int Offset[4];

public:
	// 이제부터 모든 스탯 데이터는 Packet으로부터 받아오도록 변경
	Protocol::ObjectInfo Info;
};

