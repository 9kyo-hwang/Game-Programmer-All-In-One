#include "pch.h"
#include "SceneManager.h"
#include "DevelopmentScene.h"
#include "GameplayScene.h"

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

void SceneManager::Render(HDC InHandleDeviceContext)
{
	if (ActiveScene)
	{
		ActiveScene->Render(InHandleDeviceContext);
	}
}

void SceneManager::LoadScene(ESceneType NewType)
{
	if (NewType == Type)
	{
		return;
	}

	Scene* NewScene = nullptr;

	switch (NewType) {
	case ESceneType::None:
		break;
	case ESceneType::Development:
		NewScene = new DevelopmentScene();
		break;
	case ESceneType::Gameplay:
		NewScene = new GameplayScene();
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
