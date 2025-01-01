#include "pch.h"
#include "AActor.h"

#include "GameZone.h"

const Vector2Int AActor::Offset[]{ {0, -1}, {0, 1}, {-1, 0}, {1, 0} };


AActor::AActor()
{
	
}

AActor::~AActor()
{
	
}

void AActor::TransitionTo(EObjectStates NewState, bool Broadcast)
{
	if (Info.state() == NewState)
	{
		return;
	}

	Info.set_state(NewState);
	if (Broadcast)
	{
		BroadcastMove();
	}
}

void AActor::RotateTo(EMovementDirection NewDirection, bool Broadcast)
{
	Info.set_direction(NewDirection);
	if (Broadcast)
	{
		BroadcastMove();
	}
}

bool AActor::CanMoveTo(Vector2Int Dest)
{
	if (EnteredZone)
	{
		return EnteredZone->CanMoveTo(Dest);
	}

	return false;
}

void AActor::MoveTo(Vector2Int Dest, bool Broadcast)
{
	Info.set_posx(Dest.X);
	Info.set_posy(Dest.Y);

	if (Broadcast)
	{
		// 서버에서는 스르륵 움직이는 처리가 필요없음
		BroadcastMove();
	}
}

Vector2Int AActor::GetFrontCell() const
{
	switch (Info.direction())
	{
	case EMovementDirection::Up:
		return GetCellPosition() + Offset[static_cast<int32>(EMovementDirection::Up)];
	case EMovementDirection::Down:
		return GetCellPosition() + Offset[static_cast<int32>(EMovementDirection::Down)];
	case EMovementDirection::Left:
		return GetCellPosition() + Offset[static_cast<int32>(EMovementDirection::Left)];
	case EMovementDirection::Right:
		return GetCellPosition() + Offset[static_cast<int32>(EMovementDirection::Right)];
	}

	return GetCellPosition();
}

EMovementDirection AActor::FindLookAtDirection(Vector2Int TargetPosition) const
{
	Vector2Int Direction = TargetPosition - GetCellPosition();
	if (Direction.X < 0)
	{
		return EMovementDirection::Left;
	}
	else if (Direction.X > 0)
	{
		return EMovementDirection::Right;
	}
	else if (Direction.Y < 0)
	{
		return EMovementDirection::Up;
	}
	else if (Direction.Y > 0)
	{
		return EMovementDirection::Down;
	}
}

void AActor::BroadcastMove() const
{
	if (EnteredZone)
	{
		// Zone에 S_Move 패킷을 만들어 브로드캐스트
		EnteredZone->Broadcast(ServerPacketHandler::Outgoing_S_Move(Info));
	}
}
