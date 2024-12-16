#include "pch.h"
#include "SceneManager.h"
#include "DevelopmentScene.h"
#include "EditorScene.h"

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

void SceneManager::Render(HDC InDC)
{
	if (ActiveScene)
	{
		ActiveScene->Render(InDC);
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
