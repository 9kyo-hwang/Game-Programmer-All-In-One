#include "pch.h"
#include "ALocalPlayer.h"

#include "CameraComponent.h"
#include "ClientPacketHandler.h"
#include "InputManager.h"
#include "NetworkManager.h"

ALocalPlayer::ALocalPlayer()
{
	// ������ �����ϴ� �� �÷��̾ ī�޶� �ٿ��� ��
	AddComponent(new CameraComponent());
}

ALocalPlayer::~ALocalPlayer()
{
}

void ALocalPlayer::BeginPlay()
{
	Super::BeginPlay();
}

void ALocalPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	SynchronizeToServer();  // �� �����Ӹ��� DirtyFlag üũ
}

void ALocalPlayer::Render(HDC DeviceContextHandle)
{
	Super::Render(DeviceContextHandle);
}

void ALocalPlayer::Input()
{
	bKeyPressed = true;
	if (InputManager::Get()->GetButton(EKeyCode::W))
	{
		RotateTo(EMovementDirection::Up);
	}
	else if (InputManager::Get()->GetButton(EKeyCode::S))
	{
		RotateTo(EMovementDirection::Down);
	}
	else if (InputManager::Get()->GetButton(EKeyCode::A))
	{
		RotateTo(EMovementDirection::Left);
	}
	else if (InputManager::Get()->GetButton(EKeyCode::D))
	{
		RotateTo(EMovementDirection::Right);
	}
	else
	{
		bKeyPressed = false;
	}

	// ���� �ٲٴ� ���� �ϴ��� ����
	if (InputManager::Get()->GetButtonDown(EKeyCode::Alpha1))
	{
		ChangeWeapon(EWeapons::Sword);
	}
	else if (InputManager::Get()->GetButtonDown(EKeyCode::Alpha2))
	{
		ChangeWeapon(EWeapons::Bow);
	}
	else if (InputManager::Get()->GetButtonDown(EKeyCode::Alpha3))
	{
		ChangeWeapon(EWeapons::Staff);
	}

	if (InputManager::Get()->GetButton(EKeyCode::Space))
	{
		TransitionTo(EObjectStates::Attack);
	}
}

bool ALocalPlayer::Move()
{
	if (!bKeyPressed)
	{
		return false;
	}

	// ���� ����� Ŭ���̾�Ʈ�� ���� �̵��ϰ� ������ ���뺸
	Vector2Int FrontCell = GetFrontCell();
	if (CanMoveTo(FrontCell))
	{
		MoveTo(FrontCell);
		TransitionTo(EObjectStates::Move);
	}
	return true;
}

void ALocalPlayer::OnTickIdle(float DeltaTime)
{
	Input();  // �Է°� ���� ��ȯ�� ����
	Move();  // ������ �̵�
}

void ALocalPlayer::OnTickMove(float DeltaTime)
{
	Super::OnTickMove(DeltaTime);
}

void ALocalPlayer::OnTickAttack(float DeltaTime)
{
	Super::OnTickAttack(DeltaTime);
}

void ALocalPlayer::SynchronizeToServer() const
{
	// ���� ��Ÿ�ϸ��� ������ ��ũ �ֱ⸦ ã�ƾ� ��
	// DirtyFlag�� True�� ������ �������� ����ȭ
	if (GetDirtyFlag())
	{
		TSharedPtr<SendBuffer> Buffer = ClientPacketHandler::Outgoing_C_Move();
		NetworkManager::Get()->SendPacket(Buffer);
	}
}
