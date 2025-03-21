#include "pch.h"
#include "ClientPacketHandler.h"
#include "BufferReader.h"
#include "DevelopmentScene.h"
#include "SceneManager.h"
#include "ServerSession.h"
#include "ALocalPlayer.h"

void ClientPacketHandler::HandlePacket(SessionRef Session, BYTE* Buffer, int32 Len)
{
	BufferedReader Reader(Buffer, Len);
	PacketHeader Header;

	Reader >> Header;

	switch (Header.ID)
	{
	case S_EnterGame:
		Incoming_S_EnterGame(Session, Buffer, Len);
		break;
	case S_LocalPlayer:
		Incoming_S_LocalPlayer(Session, Buffer, Len);
		break;
	case S_SpawnActor:
		Incoming_S_SpawnActor(Session, Buffer, Len);
		break;
	case S_DestroyActor:
		Incoming_S_DestroyActor(Session, Buffer, Len);
		break;
	case S_Move:
		Incoming_S_Move(Session, Buffer, Len);
		break;
	default:
		break;
	}
}

// 패킷 설계 TEMP
struct BuffData
{
	uint64 BuffId;
	float RemainTime;
};

struct S_TEST
{
	uint64 Id;
	uint32 Hp;
	uint16 Attack;

	// 가변 데이터(문자열, 바이트 배열(이미지), 리스트 등)
	vector<BuffData> Data;
};

void ClientPacketHandler::Incoming_S_TEST(SessionRef Session, BYTE* Buffer, int32 Len)
{
	PacketHeader* Header = reinterpret_cast<PacketHeader*>(Buffer);
	uint16 Size = Header->Size;

	Protocol::S_TEST Packet;
	/**
	 * PacketHeader 사이즈만큼이 각 인덱스 한 칸 크기
	 * [0]: 실제 헤더가 담긴 부분
	 * [1]~: 이후 데이터가 이어지는 부분
	 * 따라서 [1]부터의 주소를 (전체 패킷 사이즈 - 패킷 헤더 사이즈)만큼의 데이터 부분(직렬화된)으로 확인
	 */
	Packet.ParseFromArray(&Header[1], Size - sizeof(PacketHeader));

	uint64 Id = Packet.id();
	uint32 Hp = Packet.hp();
	uint16 Attack = Packet.attack();

	cout << "Id: " << Id << ", " << "Hp: " << Hp << ", " << "ATT: " << Attack << endl;

	for (int32 i = 0; i < Packet.buffs_size(); ++i)
	{
		const Protocol::BuffData& Data = Packet.buffs(i);
		cout << "Buff Info: " << Data.buffid() << " " << Data.remaintime() << endl;
	}

	// TODO: GameLogic
}

void ClientPacketHandler::Incoming_S_EnterGame(SessionRef Session, BYTE* Buffer, int32 Len)
{
	// 반복되는 파트를 따로 떼버리거나 툴로 만들면 참 좋을텐데...
	PacketHeader* Header = reinterpret_cast<PacketHeader*>(Buffer);
	uint16 Size = Header->Size;

	Protocol::S_EnterGame Packet;
	Packet.ParseFromArray(&Header[1], Size - sizeof(PacketHeader));

	bool bSuccess = Packet.success();
	uint64 AccountId = Packet.accountid();

	// 만약 서버에 즉시 회신하고 싶다면 아래와 같이 수행
	// Session->Send()
}

void ClientPacketHandler::Incoming_S_LocalPlayer(SessionRef Session, BYTE* Buffer, int32 Len)
{
	PacketHeader* Header = reinterpret_cast<PacketHeader*>(Buffer);
	uint16 Size = Header->Size;

	Protocol::S_LocalPlayer Packet;
	Packet.ParseFromArray(&Header[1], Size - sizeof(PacketHeader));

	const Protocol::ObjectInfo& Info = Packet.info();
	if (DevelopmentScene* Scene = SceneManager::Get()->GetDevelopmentScene())
	{
		// 전달받은 Packet 정보를 이용해 LocalPlayer를 클라이언트 씬에 배치
		ALocalPlayer* Player = Scene->NewObject<ALocalPlayer>(Vector2Int{ Info.posx(), Info.posy() });
		Player->Info = Info;
		SceneManager::Get()->SetLocalPlayer(Player);
	}
}

void ClientPacketHandler::Incoming_S_SpawnActor(SessionRef Session, BYTE* Buffer, int32 Len)
{
	PacketHeader* Header = reinterpret_cast<PacketHeader*>(Buffer);
	uint16 Size = Header->Size;

	Protocol::S_SpawnActor Packet;
	Packet.ParseFromArray(&Header[1], Size - sizeof(PacketHeader));

	if (DevelopmentScene* Scene = SceneManager::Get()->GetDevelopmentScene())
	{
		// 여러 클라이언트가 접근할 수 있어서, DevelopmentScene에 Packet을 처리하는 메서드 추가
		Scene->Handle_S_SpawnActor(Packet);
	}
}

void ClientPacketHandler::Incoming_S_DestroyActor(SessionRef Session, BYTE* Buffer, int32 Len)
{
	PacketHeader* Header = reinterpret_cast<PacketHeader*>(Buffer);
	uint16 Size = Header->Size;

	Protocol::S_DestroyActor Packet;
	Packet.ParseFromArray(&Header[1], Size - sizeof(PacketHeader));

	if (DevelopmentScene* Scene = SceneManager::Get()->GetDevelopmentScene())
	{
		// 여러 클라이언트가 접근할 수 있어서, DevelopmentScene에 Packet을 처리하는 메서드 추가
		Scene->Handle_S_DestroyActor(Packet);
	}
}

void ClientPacketHandler::Incoming_S_Move(SessionRef Session, BYTE* Buffer, int32 Len)
{
	PacketHeader* Header = reinterpret_cast<PacketHeader*>(Buffer);
	uint16 Size = Header->Size;

	Protocol::S_Move Packet;
	Packet.ParseFromArray(&Header[1], Size - sizeof(PacketHeader));

	const Protocol::ObjectInfo& Info = Packet.info();
	if (DevelopmentScene* Scene = SceneManager::Get()->GetDevelopmentScene())
	{
		uint64 LocalPlayerId = SceneManager::Get()->GetLocalPlayerId();
		if (Info.id() == LocalPlayerId)
		{
			return;
		}

		if (UObject* Object = Scene->FindObjectBy(Info.id()))
		{
			Object->RotateTo(Info.direction());
			Object->TransitionTo(Info.state());
			Object->MoveTo(Vector2Int{ Info.posx(), Info.posy() });
		}
	}
}

TSharedPtr<SendBuffer> ClientPacketHandler::Outgoing_C_Move()
{
	// 원래라면 플레이어 정보를 인자로 받아야 하지만, 클라는 매니저를 통해 정보를 쉽게 얻을 수 있어서 바로 작업
	Protocol::C_Move Packet;

	ALocalPlayer* Player = SceneManager::Get()->GetLocalPlayer();
	*Packet.mutable_info() = Player->Info;
	return MakeSendBuffer(Packet, C_Move);
}
