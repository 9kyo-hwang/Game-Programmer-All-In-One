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

protected:
	Vector2Int CellPosition;  // float position to int position
	EObjectStates CurrentState = EObjectStates::Idle;
	EMovementDirection CurrentDirection = EMovementDirection::Down;
};

