#include "pch.h"
#include "SceneManager.h"
#include "DevelopmentScene.h"
#include "EditorScene.h"
#include "ALocalPlayer.h"

void SceneManager::Initialize()
{
}

void SceneManager::Update()
{
	if (ActiveScene)
	{
		ActiveScene->Update();
	}
}

void SceneManager::Render(HDC InDeviceContextHandle)
{
	if (ActiveScene)
	{
		ActiveScene->Render(InDeviceContextHandle);
	}
}

void SceneManager::LoadScene(ESceneType NewType)
{
	if (NewType == Type)
	{
		return;
	}

	Scene* NewScene = nullptr;
	switch (NewType)
	{
	case ESceneType::Development:
		NewScene = new DevelopmentScene();
		break;
	case ESceneType::Editor:
		NewScene = new EditorScene();
		break;
	case ESceneType::None:
		break;
	}

	SAFE_DELETE(ActiveScene);

	ActiveScene = NewScene;
	Type = NewType;

	NewScene->Initialize();
}

void SceneManager::Clear()
{
	SAFE_DELETE(ActiveScene);
}

DevelopmentScene* SceneManager::GetDevelopmentScene() const
{
	return dynamic_cast<DevelopmentScene*>(GetActiveScene());
}

uint64 SceneManager::GetLocalPlayerId() const
{
	return Player ? Player->Info.id() : 0;
}
