#include "pch.h"
#include "GameZone.h"

#include <ranges>

#include "AActor.h"
#include "APlayer.h"
#include "AMonster.h"
#include "GameSession.h"

// ������ Zone���� ������ Manager�� ������, �츮�� �����ϰ� �ϳ���...
TSharedPtr<GameZone> GZone = make_shared<GameZone>();

GameZone::GameZone()
{
}

GameZone::~GameZone()
{
}

void GameZone::Initialize()
{
	TSharedPtr<AMonster> Monster = AActor::SpawnActor<AMonster>();
	Monster->Info.set_posx(8);
	Monster->Info.set_posy(8);

	AddActor(Monster);
}

void GameZone::Update()
{
}

void GameZone::Enter(SessionRef EnterSession)
{
	TSharedPtr<APlayer> NewPlayer = AActor::SpawnActor<APlayer>();

	// ���� - �÷��̾� ����
	EnterSession->Zone = GetZone();  // Shared -> Weak�� = �����ڷ� �ٷ� �Ҵ� ����
	EnterSession->Player = NewPlayer;
	NewPlayer->Session = EnterSession;
	NewPlayer->Info.set_posx(5);
	NewPlayer->Info.set_posy(5);

	// �Ʒ� 2������ ���ο� ��Ŷ�� �����ؼ� ����

	// ���� ���� Ŭ���̾�Ʈ���� �ʿ��� ����(�ڽ��� ��������) �Ѱ��ֱ�
	{
		EnterSession->Send(ServerPacketHandler::Outgoing_S_LocalPlayer(NewPlayer->Info));  // ��Ŷ ���� �� ��ȯ���� ����ȭ�� SendBuffer�� Session�� Send
	}

	// �ֺ� Ŭ���̾�Ʈ�鿡�� �� �÷��̾ ���������� �˷���� ��
	{
		Protocol::S_SpawnActor Packet;
		for (const auto& Player : Players | views::values)
		{
			*Packet.add_actors() = Player->Info;
		}

		for (const auto& Monster : Monsters | views::values)
		{
			*Packet.add_actors() = Monster->Info;
		}

		EnterSession->Send(ServerPacketHandler::Outgoing_S_SpawnActor(Packet));
	}

	AddActor(NewPlayer);
}

void GameZone::Exit(SessionRef ExitSession)
{
	if (ExitSession)
	{
		if (TSharedPtr<APlayer> Player = ExitSession->Player.lock())
		{
			RemoveActor(Player->Info.id());
		}
	}
}

TSharedPtr<AActor> GameZone::FindActorBy(uint64 Id)
{
	if (Players.contains(Id))
	{
		return Players[Id];
	}

	if (Monsters.contains(Id))
	{
		return Monsters[Id];
	}

	return nullptr;
}

void GameZone::Incoming_C_Move(const Protocol::C_Move& Packet)
{
	if (auto Actor = FindActorBy(Packet.info().id()))
	{
		// Check Validation(�� ����)
		Actor->Info.set_state(Packet.info().state());
		Actor->Info.set_direction(Packet.info().direction());
		Actor->Info.set_posx(Packet.info().posx());
		Actor->Info.set_posy(Packet.info().posy());

		// Client�� ��ȭ�� ���������Ƿ� ������ Ŭ���̾�Ʈ�鿡�� S_Move ��Ŷ ����
		TSharedPtr<SendBuffer> Buffer = ServerPacketHandler::Outgoing_S_Move(Packet.info());
		Broadcast(Buffer);
	}
}

void GameZone::AddActor(TSharedPtr<AActor> New)
{
	uint64 Id = New->Info.id();
	switch (New->Info.type())
	{
	case Protocol::Player:
		Players[Id] = static_pointer_cast<APlayer>(New);
		break;
	case Protocol::Monster:
		Monsters[Id] = static_pointer_cast<AMonster>(New);
		break;
	default:
		return;
	}

	// TODO: �ű� ���� ���� ����
	New->EnteredZone = GetZone();

	Protocol::S_SpawnActor Packet;
	*Packet.add_actors() = New->Info;
	Broadcast(ServerPacketHandler::Outgoing_S_SpawnActor(Packet));
}

void GameZone::RemoveActor(uint64 Id)
{
	if (TSharedPtr<AActor> Actor = FindActorBy(Id))
	{
		switch (Actor->Info.type())
		{
		case Protocol::Player:
			Players.erase(Id);
			break;
		case Protocol::Monster:
			Monsters.erase(Id);
			break;
		default:
			return;
		}

		Actor->EnteredZone = nullptr;

		// TODO: ���� ���� �˸�
		Protocol::S_DestroyActor Packet;
		Packet.add_ids(Id);
		Broadcast(ServerPacketHandler::Outgoing_S_DestroyActor(Packet));
	}
}

void GameZone::Broadcast(const TSharedPtr<SendBuffer>& SendBuffer)
{
	for (const auto& Player : Players | views::values)
	{
		Player->Session->Send(SendBuffer);
	}
}
