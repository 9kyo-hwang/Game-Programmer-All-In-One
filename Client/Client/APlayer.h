#pragma once
#include "FlipbookActor.h"

class BoxCollider;

enum class EPlayerState : uint8
{
	Idle,
	Move,
	Attack,
	END
};

class APlayer : public FlipbookActor
{
	using Super = FlipbookActor;

public:
	APlayer();
	~APlayer() override;

	void BeginPlay() override;
	void Tick(float DeltaTime) override;
	void Render(HDC DeviceContextHandle) override;

protected:
	virtual void OnIdle(float DeltaTime);
	virtual void OnMove(float DeltaTime);
	virtual void OnAttack(float DeltaTime);

private:
	void TransitionTo(EPlayerState NewState);
	void RotateTo(EMovementDirection NewDirection);

	void UpdateAnimation();

	bool HasReachedDest() const;
	bool CanMoveTo(Vector2Int Dest);
	void MoveTo(Vector2Int Dest, bool bTeleport = false);

private:
	vector<vector<Flipbook*>> Flipbooks;

	Vector2Int CellPosition;  // float position to int position
	EPlayerState CurrentState = EPlayerState::Idle;
	EMovementDirection CurrentDirection = EMovementDirection::Down;
	uint8 bKeyPressed = false;

	static const Vector2Int Offset[4];
};

