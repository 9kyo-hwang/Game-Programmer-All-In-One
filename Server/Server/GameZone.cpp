#include "pch.h"
#include "GameZone.h"

#include <ranges>

#include "AActor.h"
#include "APlayer.h"
#include "AMonster.h"
#include "GameSession.h"

// 보통은 Zone들을 관리할 Manager를 두지만, 우리는 심플하게 하나만...
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

	// 세션 - 플레이어 연결
	EnterSession->Zone = GetZone();  // Shared -> Weak는 = 연산자로 바로 할당 가능
	EnterSession->Player = NewPlayer;
	NewPlayer->Session = EnterSession;
	NewPlayer->Info.set_posx(5);
	NewPlayer->Info.set_posy(5);

	// 아래 2가지는 새로운 패킷을 설계해서 구현

	// 새로 들어온 클라이언트에게 필요한 정보(자신이 누구인지) 넘겨주기
	{
		EnterSession->Send(ServerPacketHandler::Outgoing_S_LocalPlayer(NewPlayer->Info));  // 패킷 생성 후 반환받은 직렬화된 SendBuffer를 Session에 Send
	}

	// 주변 클라이언트들에게 새 플레이어가 입장했음을 알려줘야 함
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
		// Check Validation(핵 방지)
		Actor->Info.set_state(Packet.info().state());
		Actor->Info.set_direction(Packet.info().direction());
		Actor->Info.set_posx(Packet.info().posx());
		Actor->Info.set_posy(Packet.info().posy());

		// Client의 변화를 적용했으므로 나머지 클라이언트들에게 S_Move 패킷 전달
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

	// TODO: 신규 액터 정보 전송
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

		// TODO: 액터 삭제 알림
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
