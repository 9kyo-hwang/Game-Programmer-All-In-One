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
	bool GetDirtyFlag() const { return bDirty; }
	void SetDirtyFlag(bool InDirty) { bDirty = InDirty; }

	static const Vector2Int Offset[4];
	bool bDirty = false;  // ��ȭ�� ���� �� Flip�Ǵ� �÷���

public:
	// �������� ��� ���� �����ʹ� Packet���κ��� �޾ƿ����� ����
	Protocol::ObjectInfo Info;
};

